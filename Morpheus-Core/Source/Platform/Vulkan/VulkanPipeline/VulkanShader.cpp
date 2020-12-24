#include "Morppch.h"
#include "VulkanShader.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"
#include "Morpheus/Utilities/FileLoader.h"

namespace Morpheus {

	static const VkFormat& ShaderAttributeTypeToVulkan(const ShaderAttributeType& _Type)
	{
		switch (_Type)
		{
			case ShaderAttributeType::Float:		return VK_FORMAT_R32_SFLOAT;
			case ShaderAttributeType::Float2:		return VK_FORMAT_R32G32_SFLOAT;
			case ShaderAttributeType::Float3:		return VK_FORMAT_R32G32B32_SFLOAT;
			case ShaderAttributeType::Float4:		return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderAttributeType::Mat3:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderAttributeType::Mat4:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderAttributeType::Int:			return VK_FORMAT_R32_SINT; 
			case ShaderAttributeType::Int2:			return VK_FORMAT_R32G32_SINT; 
			case ShaderAttributeType::Int3:			return VK_FORMAT_R32G32B32_SINT; 
			case ShaderAttributeType::Int4:			return VK_FORMAT_R32G32B32A32_SINT; 
			case ShaderAttributeType::Bool:			return VK_FORMAT_R32_SINT;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
		return VK_FORMAT_UNDEFINED;
	}

	VulkanShader::VulkanShader(const Ref<Renderpass>& _Renderpass, const ShaderAttributeLayout& _ShaderLayout,
		const String& _VertexPath, const String& _FragmentPath, const Ref<ShaderCache>& _Cache)
		: m_Renderpass(_Renderpass), m_ShaderLayout(_ShaderLayout), m_VertexFile(_VertexPath), m_FragmentFile(_FragmentPath), m_Cache(_Cache)
	{
		MORP_PROFILE_FUNCTION();

		m_Device = VulkanInstance::GetInstance()->GetLogicalDevice();
		m_ID = m_Cache->GetCount();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Shader #" + ToString(GetID()) + " Was Created!");
	}

	VulkanShader::~VulkanShader()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::Bind()
	{
		m_Cache->SetBoundID(GetID());
		PipelineCreate();
	}

	void VulkanShader::Unbind()
	{
		m_Cache->SetBoundID(uint32_max);
		PipelineDestory();
	}

	void VulkanShader::SetInt(const String& _Name, const uint32& _Value, const Boolean& _Flags)
	{
		m_PushCache[_Name] = _Value;
	}

	void VulkanShader::SetFloat(const String& _Name, const float32& _Value, const Boolean& _Flags)
	{
		m_PushCache[_Name] = _Value;
	}

	void VulkanShader::SetFloat3(const String& _Name, const Vector3& _Value, const Boolean& _Flags)
	{
		m_PushCache[_Name] = _Value;
	}

	void VulkanShader::SetFloat4(const String& _Name, const Vector4& _Value, const Boolean& _Flags)
	{
		m_PushCache[_Name] = _Value;
	}

	void VulkanShader::SetMat4(const String& _Name, const Matrix4& _Matrix, const Boolean& _Flags)
	{
		m_PushCache[_Name] = _Matrix;
	}

	void VulkanShader::VulkanCompile(const uint32& _Index)
	{
		m_Descriptor->UpdateDescriptorWrites(m_DescriptorWrites[_Index]);
	}

	void VulkanShader::UpdateTextureBuffer(const Ref<VulkanTextureBuffer>& _TextureBuffer, const uint32& _Index)
	{
		MORP_PROFILE_FUNCTION();

		if (_TextureBuffer->IsCompiled() == false) {
			VkWriteDescriptorSet DescriptorSet = _TextureBuffer->UpdateDescriptorSet(m_Descriptor->GetDescriptorSet(_Index));
			if (_Index < m_DescriptorWrites.size()) {
				m_DescriptorWrites[_Index].push_back(DescriptorSet);
			}
			else {
				Vector<VkWriteDescriptorSet> newArray;
				newArray.push_back(DescriptorSet);
				m_DescriptorWrites.push_back(newArray);
			}
		}
	}

	void VulkanShader::UpdateUniformBuffer(const Ref<VulkanUniformBuffer>& _UniformBuffer, const uint32& _Index)
	{
		MORP_PROFILE_FUNCTION();

		if (_UniformBuffer->IsCompiled() == false) {
			VkWriteDescriptorSet DescriptorSet = _UniformBuffer->UpdateDescriptorSet(m_Descriptor->GetDescriptorSet(_Index));
			if (_Index < m_DescriptorWrites.size()) {
				m_DescriptorWrites[_Index].push_back(DescriptorSet);
			}
			else {
				Vector<VkWriteDescriptorSet> newArray;
				newArray.push_back(DescriptorSet);
				m_DescriptorWrites.push_back(newArray);
			}
		}
	}

	const VkDescriptorSet& VulkanShader::GetDescriptorSet(const uint32& _Index)
	{
		return m_Descriptor->GetDescriptorSet(_Index);
	}

	void VulkanShader::VulkanCreate()
	{
		m_Descriptor = VulkanDescriptor::Make();

		Vector<float8> vertShaderCode = FileLoader::ReadFile(m_VertexFile);
		Vector<float8> fragShaderCode = FileLoader::ReadFile(m_FragmentFile);

		VkShaderModuleCreateInfo VertexCreateInfo = {};
		VertexCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		VertexCreateInfo.codeSize = vertShaderCode.size();
		VertexCreateInfo.pCode = (uint32*)vertShaderCode.data();
		VkResult result = vkCreateShaderModule(m_Device->GetDevice(), &VertexCreateInfo, nullptr, &m_VertModule);
		MORP_CORE_ASSERT(result, "[VULKAN] Vertex Shader Creation Failure!");

		VkShaderModuleCreateInfo FragmentCreateInfo = {};
		FragmentCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		FragmentCreateInfo.codeSize = fragShaderCode.size();
		FragmentCreateInfo.pCode = (uint32*)fragShaderCode.data();
		result = vkCreateShaderModule(m_Device->GetDevice(), &FragmentCreateInfo, nullptr, &m_FragModule);
		MORP_CORE_ASSERT(result, "[VULKAN] Fragment Shader Creation Failure!");
	}

	void VulkanShader::VulkanDestory()
	{
		PipelineDestory();
		MORP_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::PipelineCreate()
	{
		MORP_PROFILE_FUNCTION();

		if (m_PipelineCreated)
			return;

		Ref<VulkanSwapchain> Swapchain = VulkanInstance::GetInstance()->GetSurface()->GetSwapchain();
		VkViewport Viewport = Swapchain->GetViewport();
		VkRect2D Scissor = Swapchain->GetScissor();
		VkRenderPass Renderpass = std::dynamic_pointer_cast<VulkanRenderpass>(m_Renderpass)->GetRenderpass();

		VkPipelineCacheCreateInfo CacheCreateInfo = {};
		CacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VkResult result = vkCreatePipelineCache(m_Device->GetDevice(), &CacheCreateInfo, nullptr, &m_PipelineCache);
		MORP_CORE_ASSERT(result, "[VULKAN] PipelineCache Creation Failure!");

		VkPipelineLayoutCreateInfo LayoutCreateInfo = {};
		LayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		LayoutCreateInfo.setLayoutCount = 1;
		LayoutCreateInfo.pSetLayouts = &m_Descriptor->GetDescriptorSetLayout();

		result = vkCreatePipelineLayout(m_Device->GetDevice(), &LayoutCreateInfo, nullptr, &m_PipelineLayout);
		MORP_CORE_ASSERT(result, "[VULKAN] PipelineLayout Creation Failure!");

		VkPipelineShaderStageCreateInfo VertexShaderStage = {};
		VertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		VertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		VertexShaderStage.module = m_VertModule;
		VertexShaderStage.pName = "main";

		VkPipelineShaderStageCreateInfo FragmentShaderStage = {};
		FragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		FragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		FragmentShaderStage.module = m_FragModule;
		FragmentShaderStage.pName = "main";

		Vector<VkPipelineShaderStageCreateInfo> PipelineShaderStages = { VertexShaderStage, FragmentShaderStage };

		VkVertexInputBindingDescription InputBinding;
		InputBinding.binding = 0;
		InputBinding.stride = m_ShaderLayout.GetStride();
		InputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		uint32 Index = 0;
		Vector<VkVertexInputAttributeDescription> InputAttributes;
		for (const ShaderAttributeElement& Element : m_ShaderLayout.GetElements()) {
			VkVertexInputAttributeDescription Attribute = {};
			Attribute.binding = 0;
			Attribute.location = Index;
			Attribute.format = ShaderAttributeTypeToVulkan(Element.Type);
			Attribute.offset = Element.Offset;
			InputAttributes.push_back(Attribute);
			Index++;
		}

		VkPipelineVertexInputStateCreateInfo InputState;
		InputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		InputState.pNext = nullptr;
		InputState.flags = VkPipelineVertexInputStateCreateFlags();
		InputState.vertexBindingDescriptionCount = 1;
		InputState.pVertexBindingDescriptions = &InputBinding;
		InputState.vertexAttributeDescriptionCount = InputAttributes.size();
		InputState.pVertexAttributeDescriptions = InputAttributes.data();
		
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateCreateInfo = {};
		InputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		InputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		VkPipelineViewportStateCreateInfo ViewportStateCreateInfo = {};
		ViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		ViewportStateCreateInfo.viewportCount = 1;
		ViewportStateCreateInfo.pViewports = &Viewport;
		ViewportStateCreateInfo.scissorCount = 1;
		ViewportStateCreateInfo.pScissors = &Scissor;

		VkPipelineRasterizationStateCreateInfo RasterizationStateCreateInfo = {};
		RasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		RasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
		RasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
		RasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		RasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;// VK_CULL_MODE_NONE; // VK_CULL_MODE_BACK_BIT;
		RasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // VK_FRONT_FACE_COUNTER_CLOCKWISE;
		RasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
		RasterizationStateCreateInfo.depthBiasConstantFactor = 0;
		RasterizationStateCreateInfo.depthBiasClamp = 0;
		RasterizationStateCreateInfo.depthBiasSlopeFactor = 0;
		RasterizationStateCreateInfo.lineWidth = 1.0f;

		VkPipelineMultisampleStateCreateInfo MultisampleStateCreateInfo = {};
		MultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		MultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo = {};
		DepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		DepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
		DepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
		DepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		DepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
		DepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
		DepthStencilStateCreateInfo.front = VkStencilOpState();
		DepthStencilStateCreateInfo.back = VkStencilOpState();
		DepthStencilStateCreateInfo.minDepthBounds = 0;
		DepthStencilStateCreateInfo.maxDepthBounds = 0;

		VkPipelineColorBlendAttachmentState ColorBlendAttachmentState = {};
		ColorBlendAttachmentState.blendEnable = VK_FALSE;
		ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		ColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		ColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
		ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		Vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachments = { ColorBlendAttachmentState };
		VkPipelineColorBlendStateCreateInfo ColorBlendStateCreateInfo = {};
		ColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		ColorBlendStateCreateInfo.logicOpEnable = 0;
		ColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_CLEAR;
		ColorBlendStateCreateInfo.attachmentCount = ColorBlendAttachments.size();
		ColorBlendStateCreateInfo.pAttachments = ColorBlendAttachments.data();

		Vector<VkDynamicState> DynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo DynamicStateCreateInfo = {};
		DynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		DynamicStateCreateInfo.dynamicStateCount = DynamicStates.size();
		DynamicStateCreateInfo.pDynamicStates = DynamicStates.data();

		VkGraphicsPipelineCreateInfo PipelineCreateInfo = {};
		PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		PipelineCreateInfo.stageCount = PipelineShaderStages.size();
		PipelineCreateInfo.pStages = PipelineShaderStages.data();
		PipelineCreateInfo.pVertexInputState = &InputState;
		PipelineCreateInfo.pInputAssemblyState = &InputAssemblyStateCreateInfo;
		PipelineCreateInfo.pTessellationState = nullptr;
		PipelineCreateInfo.pViewportState = &ViewportStateCreateInfo;
		PipelineCreateInfo.pRasterizationState = &RasterizationStateCreateInfo;
		PipelineCreateInfo.pMultisampleState = &MultisampleStateCreateInfo;
		PipelineCreateInfo.pDepthStencilState = &DepthStencilStateCreateInfo;
		PipelineCreateInfo.pColorBlendState = &ColorBlendStateCreateInfo;
		PipelineCreateInfo.pDynamicState = &DynamicStateCreateInfo;
		PipelineCreateInfo.layout = m_PipelineLayout;
		PipelineCreateInfo.renderPass = Renderpass;
		PipelineCreateInfo.subpass = 0;

		result = vkCreateGraphicsPipelines(m_Device->GetDevice(), m_PipelineCache, 1, &PipelineCreateInfo, nullptr, &m_Pipeline);
		MORP_CORE_ASSERT(result, "[VULKAN] Pipeline Creation Failure!");

		m_PipelineCreated = true;
		MORP_CORE_TRACE("[VULKAN] Shader #" + ToString(m_ID) + " Pipeline has been Created!");
	}

	void VulkanShader::PipelineDestory()
	{
		vkDestroyPipeline(m_Device->GetDevice(), m_Pipeline, nullptr);
		vkDestroyPipelineLayout(m_Device->GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyPipelineCache(m_Device->GetDevice(), m_PipelineCache, nullptr);
	}

	Ref<VulkanShader> VulkanShader::Make(const Ref<Renderpass>& _Renderpass, const ShaderAttributeLayout& _ShaderLayout,
		const String& _VertexPath, const String& _FragmentPath)
	{
		Ref<ShaderCache> s_Cache = VulkanCache<VulkanShader>::Get(VULKAN_SHADER_CACHE_TYPE);
		Ref<VulkanShader> s_VulkanShader = CreateRef<VulkanShader>(_Renderpass, _ShaderLayout, _VertexPath, _FragmentPath, s_Cache);
		s_Cache->Add(s_VulkanShader->GetID(), s_VulkanShader);
		return s_VulkanShader;
	}

}