#include "Morppch.h"
#include "VulkanShader.h"

#include "Morpheus/Utilities/FileLoader.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanShader::VulkanShader(const ShaderAttributeLayout& _ShaderLayout, const ShaderAttributeLayout& _UniformLayout, 
		const String& _VertexPath, const String& _FragmentPath)
		: m_ShaderLayout(_ShaderLayout), m_UniformLayout(_UniformLayout), m_VertexFile(_VertexPath), m_FragmentFile(_FragmentPath)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();

		VulkanCreate();
		m_ID = VulkanMemoryManager::GetInstance()->GetShaderCache().Count();
		VULKAN_CORE_WARN("[VULKAN] Shader #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanShader::~VulkanShader()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::Bind(const uint32& _Slot)
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		uint32 FrameBufferID = Chain.GetFrameBufferID();
		if (FrameBufferID != uint32_max)
			PipelineCreate(FrameBufferID);

		//uint32 VertexBufferID = Chain.GetVertexArrayID();
		//if (VertexBufferID != uint32_max) {
		//}

		Chain.SetShader(m_ID);
	}

	void VulkanShader::Unbind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		//PipelineDestory();

		Chain.SetShader(uint32_max);
	}

	void VulkanShader::SetInt(const String& _Name, const uint32& _Value, const Boolean& _Flags)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanShader::SetFloat(const String& _Name, const float32& _Value, const Boolean& _Flags)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanShader::SetFloat3(const String& _Name, const Vector3& _Value, const Boolean& _Flags)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanShader::SetFloat4(const String& _Name, const Vector4& _Value, const Boolean& _Flags)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanShader::SetMat4(const String& _Name, const Matrix4& _Matrix, const Boolean& _Flags)
	{
		m_UniformCache[_Name] = _Matrix;
	}

	void VulkanShader::SetSampler(const String& _Name, const uint32& _Value, const Boolean& _Flags)
	{

	}

	void VulkanShader::VulkanCompile(const uint32& _Index)
	{
		m_Descriptor->UpdateDescriptorWrites(m_DescriptorWrites[_Index]);
	}

	void VulkanShader::UpdateTextureBuffer(const Ref<VulkanTextureBuffer>& _TextureBuffer, const uint32& _Index)
	{
		if(_TextureBuffer->HasUpdated() == false)  {
			Vector<vk::WriteDescriptorSet> VertexArrayWriteDescriptor;
			if (_Index < m_DescriptorWrites.size()) {
					m_DescriptorWrites[_Index].push_back(_TextureBuffer->UpdateDescriptorSet(m_Descriptor->GetDescriptorSet(_Index)));
			}
			else {
				m_DescriptorWrites.push_back(VertexArrayWriteDescriptor);
			}
		}
	}

	void VulkanShader::UpdateUniformBuffer(const Ref<VulkanUniformBuffer>& _UniformBuffer)
	{
		// HOTTEST CODE @ 3-12-20

		if (m_UniformCache.empty())
			return;

		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 Size = m_UniformLayout.GetStride();

		VulkanUniformMessage DataMessage;
		Vector<ShaderAttributeElement> Elements = m_UniformLayout.GetElements();

		for (auto& Element : Elements) {
			auto it = m_UniformCache.find(Element.Name);
			if (it != m_UniformCache.end()) {
				switch (Element.Type)
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

		_UniformBuffer->UpdateBufferData(DataMessage);
	}

	void VulkanShader::DeallocateUniformBuffer(const Ref<VulkanUniformBuffer>& _UniformBuffer)
	{
		// TODO: Delete and manage Descriptor Indexing 
	}

	Ref<VulkanUniformBuffer> VulkanShader::AllocateUniformBuffer(const uint32& _Index)
	{
		Ref<VulkanUniformBuffer> UniformBuffer = VulkanUniformBuffer::Make(m_UniformLayout, m_Descriptor->GetDescriptorSet(_Index));

		Vector<vk::WriteDescriptorSet> VertexArrayWriteDescriptor;
		if (_Index > m_DescriptorWrites.size())
			VertexArrayWriteDescriptor = m_DescriptorWrites[_Index];
		VertexArrayWriteDescriptor.push_back(UniformBuffer->UpdateDescriptorSet());
		m_DescriptorWrites.push_back(VertexArrayWriteDescriptor);
		m_UniformBufferCount++;

		return UniformBuffer;
	}

	void VulkanShader::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		// Descriptor
		m_Descriptor = VulkanDescriptor::Make();

		// Shader
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
	}

	void VulkanShader::VulkanDestory()
	{
		PipelineDestory();
		MORP_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::PipelineCreate(const uint32& _FrameBufferID)
	{
		if (m_PipelineCreated)
			return;

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
				&m_Descriptor->GetDescriptorSetLayout(),
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
			InputBinding.stride = m_ShaderLayout.GetStride();
			InputBinding.inputRate = vk::VertexInputRate::eVertex;
		}

		uint32 Index = 0;
		for (const ShaderAttributeElement& Element : m_ShaderLayout) {
			vk::VertexInputAttributeDescription Attribute = {};
			Attribute.binding = 0;
			Attribute.location = Index;
			Attribute.format = vk::Format::eR32G32Sfloat;
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
		m_PipelineCreated = true;
		MORP_CORE_TRACE("[VULKAN] Shader #" + std::to_string(m_ID) + " Pipeline has been Created!");
	}

	void VulkanShader::PipelineDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyPipelineCache(m_PipelineCache);
		Device.destroyPipeline(m_Pipeline);
		Device.destroyPipelineLayout(m_PipelineLayout);
	}

	Ref<VulkanShader> VulkanShader::Make(const ShaderAttributeLayout& _ShaderLayout, const ShaderAttributeLayout& _UniformLayout, const String& _VertexPath, const String& _FragmentPath)
	{
		Ref<VulkanShader> s_VulkanShader = CreateRef<VulkanShader>(_ShaderLayout, _UniformLayout, _VertexPath, _FragmentPath);
		VulkanMemoryManager::GetInstance()->GetShaderCache().Add(s_VulkanShader->GetID(), s_VulkanShader);
		return s_VulkanShader;
	}

}