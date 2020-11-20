#include "Morppch.h"
#include "VulkanShader.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"

#include "Morpheus/Utilities/FileLoader.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"
#include "VulkanBindingChain.h"

namespace Morpheus {

	VulkanShader::VulkanShader(const ShaderAttributeLayout& _Layout, const String& _VertexPath, const String& _FragmentPath)
		: m_Layout(_Layout), m_VertexFile(_VertexPath), m_FragmentFile(_FragmentPath)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_ID = VulkanMemoryManager::GetInstance()->GetShaderCache().Count();

		CreateShader();
		MORP_CORE_WARN("[VULKAN] Shader #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanShader::~VulkanShader()
	{
		DestoryDescriptorLayout();
		DestoryPipeline();
		DestoryUniformBuffer();
		MORP_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}
	
	void VulkanShader::Bind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetShader(m_ID);

		uint32 FrameBufferID = Chain.GetFrameBufferID();
		if (FrameBufferID != uint32_max)
			SetupPipeline(FrameBufferID);

		uint32 VertexBufferID = Chain.GetVertexArrayID();
		if (VertexBufferID != uint32_max) {
			Ref<VulkanVertexArray> VertexArray = VulkanMemoryManager::GetInstance()->GetVertexArrayCache().Get(VertexBufferID);
			if(VertexArray->GetUniformBufferID() == uint32_max) {
				uint32 UniformBufferIndex = SetupUniform();
				VertexArray->SetUniformBuffer(UniformBufferIndex);
			}
		}
	}

	void VulkanShader::Unbind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetShader(uint32_max);
	}

	void VulkanShader::CreateShader()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Vector<float8> vertShaderCode = FileLoader::ReadFile(m_VertexFile);
		Vector<float8> fragShaderCode = FileLoader::ReadFile(m_FragmentFile);

		m_VertModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				vertShaderCode.size(),
				(uint32*)vertShaderCode.data()
			)
		);

		m_FragModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				fragShaderCode.size(),
				(uint32*)fragShaderCode.data()
			)
		);

		CreateDescriptorLayout(); 
		ExpandDescriptorArray();
	}

	void VulkanShader::CreateDescriptorLayout()		// TODO MAKE GENERIC (N)
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		vk::DescriptorSetLayoutBinding DescriptorSetLayoutBinding_Uniform = {};	
		{
			DescriptorSetLayoutBinding_Uniform.binding = 0;
			DescriptorSetLayoutBinding_Uniform.descriptorType = vk::DescriptorType::eUniformBuffer;
			DescriptorSetLayoutBinding_Uniform.descriptorCount = 1;
			DescriptorSetLayoutBinding_Uniform.stageFlags = vk::ShaderStageFlagBits::eVertex;
			DescriptorSetLayoutBinding_Uniform.pImmutableSamplers = nullptr;
		}

		vk::DescriptorSetLayoutBinding DescriptorSetLayoutBinding_Sampler = {};
		{
			DescriptorSetLayoutBinding_Sampler.binding = 1;
			DescriptorSetLayoutBinding_Sampler.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			DescriptorSetLayoutBinding_Sampler.descriptorCount = 1;
			DescriptorSetLayoutBinding_Sampler.stageFlags = vk::ShaderStageFlagBits::eFragment;
			DescriptorSetLayoutBinding_Sampler.pImmutableSamplers = nullptr;
		}

		Vector<vk::DescriptorSetLayoutBinding> Bindings = { DescriptorSetLayoutBinding_Uniform, DescriptorSetLayoutBinding_Sampler };

		vk::DescriptorSetLayoutCreateInfo LayoutSetCreateInfo = {};
		{
			LayoutSetCreateInfo.flags = vk::DescriptorSetLayoutCreateFlags();
			LayoutSetCreateInfo.bindingCount = Bindings.size();
			LayoutSetCreateInfo.pBindings = Bindings.data();
		}

		m_DescriptorLayout = Device.createDescriptorSetLayout(LayoutSetCreateInfo);
	}

	void VulkanShader::CreatePipeline(const uint32& _FrameBufferID)
	{
		// Dyamically Get Swapchain
		Ref<VulkanSwapchain> Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();
		Ref<VulkanFrameBuffer> Framebuffer;
			
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Rect2D RenderArea = Swapchain->GetRenderArea();
		vk::Viewport Viewport = Swapchain->GetViewport();

		if (_FrameBufferID != uint32_max)
			Framebuffer = VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Get(_FrameBufferID);
		else MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] FrameBuffer was not Found!")

		vk::RenderPass Renderpass = Framebuffer->GetRenderpass();
		m_PipelineCache = Device.createPipelineCache(vk::PipelineCacheCreateInfo());

		m_PipelineLayout = Device.createPipelineLayout(
			vk::PipelineLayoutCreateInfo(
				vk::PipelineLayoutCreateFlags(),
				1,
				&m_DescriptorLayout,
				0,
				nullptr
			)
		);

		Vector<vk::PipelineShaderStageCreateInfo> pipelineShaderStages = {
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eVertex,
				m_VertModule,
				"main",	// Change to VertexMain
				nullptr
			),
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eFragment,
				m_FragModule,
				"main",	// Change to FragmentMain
				nullptr
			)
		};

		vk::VertexInputBindingDescription InputBinding;
		Vector<vk::VertexInputAttributeDescription> InputAttributes;

		{
			InputBinding.binding = 0;
			InputBinding.stride = m_Layout.GetStride();
			InputBinding.inputRate = vk::VertexInputRate::eVertex;
		}

		uint32 Index = 0;
		for (const ShaderAttributeElement& Element : m_Layout) {
			vk::VertexInputAttributeDescription Attribute = {};
			Attribute.binding = 0;
			Attribute.location = Index;
			Attribute.format = vk::Format::eR32G32B32Sfloat;
			Attribute.offset = Element.Offset;
			InputAttributes.push_back(Attribute);
			Index++;
		}

		{
			m_InputState.flags = vk::PipelineVertexInputStateCreateFlags();
			m_InputState.vertexBindingDescriptionCount = 1;
			m_InputState.pVertexBindingDescriptions = &InputBinding;
			m_InputState.vertexAttributeDescriptionCount = InputAttributes.size();
			m_InputState.pVertexAttributeDescriptions = InputAttributes.data();
		}

		vk::PipelineVertexInputStateCreateInfo pvi = m_InputState;

		vk::PipelineInputAssemblyStateCreateInfo pia(
			vk::PipelineInputAssemblyStateCreateFlags(),
			vk::PrimitiveTopology::eTriangleList
		);

		vk::PipelineViewportStateCreateInfo pv(
			vk::PipelineViewportStateCreateFlagBits(),
			1,
			&Viewport,
			1,
			&RenderArea
		);

		vk::PipelineRasterizationStateCreateInfo pr(
			vk::PipelineRasterizationStateCreateFlags(),
			VK_FALSE,
			VK_FALSE,
			vk::PolygonMode::eFill,
			vk::CullModeFlagBits::eNone,
			vk::FrontFace::eCounterClockwise,
			VK_FALSE,
			0,
			0,
			0,
			1.0f
		);

		vk::PipelineMultisampleStateCreateInfo pm(
			vk::PipelineMultisampleStateCreateFlags(),
			vk::SampleCountFlagBits::e1
		);

		// Depth and Stencil state for primative compare/test operations

		vk::PipelineDepthStencilStateCreateInfo pds = vk::PipelineDepthStencilStateCreateInfo(
			vk::PipelineDepthStencilStateCreateFlags(),
			VK_TRUE,
			VK_TRUE,
			vk::CompareOp::eLessOrEqual,
			VK_FALSE,
			VK_FALSE,
			vk::StencilOpState(),
			vk::StencilOpState(),
			0,
			0
		);

		// Blend State - How two primatives should draw on top of each other.
		Vector<vk::PipelineColorBlendAttachmentState> ColorBlendAttachments =
		{
			vk::PipelineColorBlendAttachmentState(
				VK_FALSE,
				vk::BlendFactor::eZero,
				vk::BlendFactor::eOne,
				vk::BlendOp::eAdd,
				vk::BlendFactor::eZero,
				vk::BlendFactor::eZero,
				vk::BlendOp::eAdd,
				vk::ColorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
			)
		};


		vk::PipelineColorBlendStateCreateInfo pbs(
			vk::PipelineColorBlendStateCreateFlags(),
			0,
			vk::LogicOp::eClear,
			ColorBlendAttachments.size(),
			ColorBlendAttachments.data()
		);

		Vector<vk::DynamicState> dynamicStates =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo pdy(
			vk::PipelineDynamicStateCreateFlags(),
			dynamicStates.size(),
			dynamicStates.data()
		);

		auto pipeline = Device.createGraphicsPipeline(
			m_PipelineCache,
			vk::GraphicsPipelineCreateInfo(
				vk::PipelineCreateFlags(),
				pipelineShaderStages.size(),
				pipelineShaderStages.data(),
				&pvi,
				&pia,
				nullptr,
				&pv,
				&pr,
				&pm,
				&pds,
				&pbs,
				&pdy,
				m_PipelineLayout,
				Renderpass,
				0
			)
		);

		m_Pipeline = pipeline;
		MORP_CORE_TRACE("[VULKAN] Shader #" + std::to_string(m_ID) + " Pipeline has been Created!");
	}

	void VulkanShader::DestoryDescriptorLayout()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyDescriptorSetLayout(m_DescriptorLayout);
	}

	void VulkanShader::DestoryPipeline()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyPipelineCache(m_PipelineCache);
		Device.destroyPipeline(m_Pipeline);
		Device.destroyPipelineLayout(m_PipelineLayout);
	}

	void VulkanShader::UpdateDescriptorSet(const VulkanUniformBuffer& _Buffer)  // TODO MAKE GENERIC (N)
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		VkDescriptorBufferInfo BufferInfo = {};
		BufferInfo.buffer = VkBuffer(_Buffer.Buffer.Buffer);
		BufferInfo.offset = 0;
		BufferInfo.range = _Buffer.Size;

		VkDescriptorImageInfo ImageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		const vk::DescriptorBufferInfo& BufferInfo = _Buffer.DescriptorInfo;
		uint32 Index = _Buffer.DescriptorIndex;
		Vector<vk::WriteDescriptorSet> descriptorWrites =
		{ vk::WriteDescriptorSet(m_DescriptorSets[Index], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &BufferInfo, nullptr) };
		Device.updateDescriptorSets(descriptorWrites, nullptr);
		MORP_CORE_ERROR("[VULKAN] Update to DescriptorSet #" + std::to_string(Index) + " was made!");
	}

	void VulkanShader::CreateUniformBuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 Size = m_UniformLayout.GetStride();

		VulkanUniformBuffer UniformBuffer;
		vk::DescriptorBufferInfo UniformDescriptor;

		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
			CreateInfo.size = Size;
		}

		UniformBuffer.Buffer.Buffer = Device.createBuffer(CreateInfo);

		auto memReqs = Device.getBufferMemoryRequirements(UniformBuffer.Buffer.Buffer);
		vk::MemoryAllocateInfo AllocInfo = {};
		{
			AllocInfo.pNext = nullptr;
			AllocInfo.allocationSize = memReqs.size;
			AllocInfo.memoryTypeIndex = m_Device->GetMemoryTypeIndex(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		}

		UniformBuffer.Buffer.Memory = Device.allocateMemory(AllocInfo);
		Device.bindBufferMemory(UniformBuffer.Buffer.Buffer, UniformBuffer.Buffer.Memory, 0);

		VulkanUniformMessage DataMessage;
		auto Layout = m_UniformLayout.GetElements();
		for (uint32 i = 0; i < Layout.size(); i++) {
			switch (Layout[i].Type)
			{
			case ShaderAttributeType::Float:	DataMessage << float32(1.00f);
				break;
			case ShaderAttributeType::Float2:	DataMessage << Vector2(1.00f, 1.00f);
				break;
			case ShaderAttributeType::Float3:	DataMessage << Vector3(1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Float4:	DataMessage << Vector4(1.00f, 1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Mat3:		DataMessage << Matrix4(1.00f);
				break;
			case ShaderAttributeType::Mat4:		DataMessage << Matrix4(1.00f);
				break;
			case ShaderAttributeType::Int:		DataMessage << Vector2(1.00f, 1.00f);
				break;
			case ShaderAttributeType::Int2:		DataMessage << Vector2(1.00f, 1.00f);
				break;
			case ShaderAttributeType::Int3:		DataMessage << Vector3(1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Int4:		DataMessage << Vector4(1.00f, 1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Bool:		DataMessage << Boolean(true);
				break;
			}
		}

		void* pData = Device.mapMemory(UniformBuffer.Buffer.Memory, 0, Size);
		std::memcpy(pData, DataMessage.Data.data(), Size);
		Device.unmapMemory(UniformBuffer.Buffer.Memory);

		UniformBuffer.DescriptorIndex = m_DescriptorSize;
		m_UniformBuffers.push_back(UniformBuffer);
		m_UniformCount++;
	}

	void VulkanShader::UpdateUniformBuffer(const uint32& _Index)
	{
		if (m_UniformCache.empty())
			return;

		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 Size = m_UniformLayout.GetStride();

		VulkanUniformMessage DataMessage;
		auto LayoutElements = m_UniformLayout.GetElements();

		for (uint32 i = 0; i < LayoutElements.size(); i++) {
			auto it = m_UniformCache.find(LayoutElements[i].Name);
			if (it != m_UniformCache.end()) {
				switch (LayoutElements[i].Type)
				{
					case ShaderAttributeType::Float:	DataMessage << AnyCast<float32>(it->second);
						break;
					case ShaderAttributeType::Float2:	DataMessage << AnyCast<Vector2>(it->second);
						break;
					case ShaderAttributeType::Float3:	DataMessage << AnyCast<Vector2>(it->second);
						break;
					case ShaderAttributeType::Float4:	DataMessage << AnyCast<Vector3>(it->second);
						break;
					case ShaderAttributeType::Mat3:		DataMessage << AnyCast<Matrix4>(it->second);
						break;
					case ShaderAttributeType::Mat4:		DataMessage << AnyCast<Matrix4>(it->second);
						break;
					case ShaderAttributeType::Int:		DataMessage << AnyCast<int32>(it->second);
						break;
					case ShaderAttributeType::Int2:		DataMessage << AnyCast<Vector2>(it->second);
						break;
					case ShaderAttributeType::Int3:		DataMessage << AnyCast<Vector3>(it->second);
						break;
					case ShaderAttributeType::Int4:		DataMessage << AnyCast<Vector4>(it->second);
						break;
					case ShaderAttributeType::Bool:		DataMessage << AnyCast<bool>(it->second);
						break;
				}
			}
		}

		VulkanUniformBuffer& UniformBuffer = m_UniformBuffers[_Index];
		void* pData = Device.mapMemory(UniformBuffer.Buffer.Memory, 0, Size);
		std::memcpy(pData, DataMessage.Data.data(), Size);
		Device.unmapMemory(UniformBuffer.Buffer.Memory);

		m_UniformCache.clear();
	}

	void VulkanShader::DestoryUniformBuffer()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		
		for (uint32 i = 0; i < m_UniformBuffers.size(); i++) {
			Device.freeMemory(m_UniformBuffers[i].Buffer.Memory);
			Device.destroyBuffer(m_UniformBuffers[i].Buffer.Buffer);
		}
	}

	void VulkanShader::ExpandDescriptorArray() 		// TODO MAKE GENERIC (N)
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		vk::DescriptorPoolSize PoolSize_Uniform = {};
		{
			PoolSize_Uniform.type = vk::DescriptorType::eUniformBuffer;
			PoolSize_Uniform.descriptorCount = SHADER_MAX_DESCRIPTOR_POOLSIZE;
		}

		vk::DescriptorPoolSize PoolSize_Sampler = {};
		{
			PoolSize_Uniform.type = vk::DescriptorType::eCombinedImageSampler;
			PoolSize_Uniform.descriptorCount = SHADER_MAX_DESCRIPTOR_POOLSIZE;
		}

		Vector<vk::DescriptorPoolSize> DescriptorPoolSizes = { PoolSize_Uniform, PoolSize_Sampler };

		vk::DescriptorPoolCreateInfo PoolCreateInfo{};
		{
			PoolCreateInfo.flags = vk::DescriptorPoolCreateFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
			PoolCreateInfo.poolSizeCount = DescriptorPoolSizes.size();
			PoolCreateInfo.pPoolSizes = DescriptorPoolSizes.data();
			PoolCreateInfo.maxSets = SHADER_MAX_DESCRIPTOR_POOLSIZE;
		}
		vk::DescriptorPool pool = Device.createDescriptorPool(PoolCreateInfo);
		MORP_CORE_ERROR("[VULKAN] DescriptorPool #" + std::to_string(m_DescriptorArray.size()) + " x " + std::to_string(SHADER_MAX_DESCRIPTOR_POOLSIZE) + " was Created!");
		m_DescriptorArray.push_back(pool);
	}

	void VulkanShader::AllocateDescriptorSet()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		// Get Number of Descriptor in Current
		m_DescriptorSets.resize(m_DescriptorSets.size() + 1);
		m_DescriptorSize = m_DescriptorSets.size();
		
		if (m_DescriptorCount >= 16) {
			ExpandDescriptorArray();
			m_DescriptorCount = 0;
		}

		VkDescriptorSetAllocateInfo AllocateInfo {};
		{
			VkDescriptorSetLayout Layout = VkDescriptorSetLayout(m_DescriptorLayout);
			AllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			AllocateInfo.descriptorPool = m_DescriptorArray[m_DescriptorArray.size() - 1];
			AllocateInfo.descriptorSetCount = 1;
			AllocateInfo.pSetLayouts = &Layout;
			m_DescriptorLayout = vk::DescriptorSetLayout(Layout);
		}

		VkDescriptorSet DescriptorSet = VkDescriptorSet(m_DescriptorSets[m_DescriptorSets.size() - 1]);
		VkResult result = vkAllocateDescriptorSets(Device, &AllocateInfo, &DescriptorSet);
		MORP_CORE_ASSERT(result, "[VULKAN] DescriptorSet cannot be Allocated!");

		m_DescriptorSets[m_DescriptorSets.size() - 1] = vk::DescriptorSet(DescriptorSet);
		MORP_CORE_ERROR("[VULKAN] DescriptorPool #" + std::to_string(m_DescriptorArray.size() - 1) + ", DescriptorSet #" + std::to_string(m_DescriptorSets.size() - 1) + " was Allocated!");
		m_DescriptorCount++;
	}

	void VulkanShader::SetupPipeline(const uint32& _FrameBufferID)
	{
		if(!m_PipelineCreated)
			CreatePipeline(_FrameBufferID);
		m_PipelineCreated = true;
	}

	uint32 VulkanShader::SetupUniform()
	{
		CreateUniformBuffer();
		return m_UniformCount - 1;	
	}

	void VulkanShader::CompileUniform(const uint32& _Index)
	{
		if (_Index >= m_DescriptorSize) {
			AllocateDescriptorSet();
			UpdateDescriptorSet(m_UniformBuffers[_Index]);
		}

		UpdateUniformBuffer(_Index);
	}

	void VulkanShader::UploadUniformInt(const String& _Name, const int32& _Int)
	{
		m_UniformCache[_Name] = _Int;
	}

	void VulkanShader::UploadUniformFloat(const String& _Name, const float32& _Float)
	{
		m_UniformCache[_Name] = _Float;
	}

	void VulkanShader::UploadUniformFloat2(const String& _Name, const Vector2& _Float2)
	{
		m_UniformCache[_Name] = _Float2;
	}

	void VulkanShader::UploadUniformFloat3(const String& _Name, const Vector3& _Float3)
	{
		m_UniformCache[_Name] = _Float3;
	}

	void VulkanShader::UploadUniformFloat4(const String& _Name, const Vector4& _Float4)
	{
		m_UniformCache[_Name] = _Float4;
	}

	void VulkanShader::UploadUniformMat4(const String& _Name, const Matrix4& _Matrix)
	{
		m_UniformCache[_Name] = _Matrix;
	}

	void VulkanShader::SetInt(const String& _Name, const uint32& _Value)
	{
		UploadUniformInt(_Name, _Value);
	}
	
	void VulkanShader::SetFloat(const String& _Name, const float32& _Value)
	{
		UploadUniformFloat(_Name, _Value);
	}
	
	void VulkanShader::SetFloat3(const String& _Name, const Vector3& _Value)
	{
		UploadUniformFloat3(_Name, _Value);
	}
	
	void VulkanShader::SetFloat4(const String& _Name, const Vector4& _Value)
	{
		UploadUniformFloat4(_Name, _Value);
	}
	
	void VulkanShader::SetMat4(const String& _Name, const Matrix4& _Matrix)
	{
		UploadUniformMat4(_Name, _Matrix);
	}

	Ref<VulkanShader> VulkanShader::Make(const ShaderAttributeLayout& _Layout, const String& _VertexPath, const String& _FragmentPath)
	{
		Ref<VulkanShader> s_VulkanShader = CreateRef<VulkanShader>(_Layout, _VertexPath, _FragmentPath);
		VulkanMemoryManager::GetInstance()->GetShaderCache().Add(s_VulkanShader->GetID(), s_VulkanShader);
		return s_VulkanShader;
	}

}