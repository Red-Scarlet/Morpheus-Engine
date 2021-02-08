#include "Morppch.h"
#include "VulkanPipeline.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Morpheus/Utilities/FileLoader.h"

namespace Morpheus { namespace Vulkan {

	VulkanPipeline::VulkanPipeline(const VulkanPipelineInfo& _PipelineInfo)
	{
		m_Device = VulkanInstance::GetInstance()->GetDevice(0);
		m_Struct = _PipelineInfo;

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Pipeline Was Created!");
	}

	VulkanPipeline::~VulkanPipeline()
	{
		VULKAN_CORE_WARN("[VULKAN] Pipeline Was Destoryed!");
	}

	void VulkanPipeline::VulkanCreate()
	{
		MORP_PROFILE_FUNCTION();
		m_Descriptor = VulkanDescriptor::Create(m_Struct.Struct.DescriptorLayout);

		VkGraphicsPipelineCreateInfo PipelineCreateInfo = {};

		VkPipelineDepthStencilStateCreateInfo DepthStencilStateCreateInfo = {};
		Vector<VkPipelineColorBlendAttachmentState> Attachments;
		for (uint32 i = 0; i < m_Struct.ColorAttachments; i++)
		{
			VkPipelineColorBlendAttachmentState ColorBlendAttachmentState = {};
			ColorBlendAttachmentState.blendEnable = VK_FALSE;
			ColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			ColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
			ColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
			ColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			ColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			ColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
			ColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			Attachments.push_back(ColorBlendAttachmentState);
		}

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

		VkPipelineColorBlendStateCreateInfo ColorBlendStateCreateInfo = {};
		ColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		ColorBlendStateCreateInfo.logicOpEnable = 0;
		ColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_CLEAR;
		ColorBlendStateCreateInfo.attachmentCount = Attachments.size();
		ColorBlendStateCreateInfo.pAttachments = Attachments.data();

		PipelineCreateInfo.pDepthStencilState = &DepthStencilStateCreateInfo;
		PipelineCreateInfo.pColorBlendState = &ColorBlendStateCreateInfo;

		Vector<float8> VertShaderCode = FileLoader::ReadFile(m_Struct.Struct.VertexFile);
		Vector<float8> FragShaderCode = FileLoader::ReadFile(m_Struct.Struct.FragmentFile);

		VkShaderModule VertexModule = {};
		VkShaderModuleCreateInfo VertexCreateInfo = {};
		VertexCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		VertexCreateInfo.codeSize = VertShaderCode.size();
		VertexCreateInfo.pCode = (uint32*)VertShaderCode.data();
		VkResult result = vkCreateShaderModule(m_Device->GetLogical(), &VertexCreateInfo, nullptr, &VertexModule);
		VULKAN_CORE_ASSERT(result, "[VULKAN] Vertex Shader Creation Failure!");

		VkShaderModule FragmentModule = {};
		VkShaderModuleCreateInfo FragmentCreateInfo = {};
		FragmentCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		FragmentCreateInfo.codeSize = FragShaderCode.size();
		FragmentCreateInfo.pCode = (uint32*)FragShaderCode.data();
		result = vkCreateShaderModule(m_Device->GetLogical(), &FragmentCreateInfo, nullptr, &FragmentModule);

		VULKAN_CORE_ASSERT(result, "[VULKAN] Fragment Shader Creation Failure!");
		VkPipelineCacheCreateInfo CacheCreateInfo = {};
		CacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		result = vkCreatePipelineCache(m_Device->GetLogical(), &CacheCreateInfo, nullptr, &m_PipelineCache);
		VULKAN_CORE_ASSERT(result, "[VULKAN] PipelineCache Creation Failure!");

		VkPipelineLayoutCreateInfo LayoutCreateInfo = {};
		LayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		LayoutCreateInfo.setLayoutCount = 1;
		LayoutCreateInfo.pSetLayouts = &m_Descriptor->GetDescriptorLayout();
		result = vkCreatePipelineLayout(m_Device->GetLogical(), &LayoutCreateInfo, nullptr, &m_PipelineLayout);
		VULKAN_CORE_ASSERT(result, "[VULKAN] PipelineLayout Creation Failure!");

		VkPipelineShaderStageCreateInfo VertexShaderStage = {};
		VertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		VertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		VertexShaderStage.module = VertexModule;
		VertexShaderStage.pName = "main";

		VkPipelineShaderStageCreateInfo FragmentShaderStage = {};
		FragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		FragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		FragmentShaderStage.module = FragmentModule;
		FragmentShaderStage.pName = "main";

		Vector<VkPipelineShaderStageCreateInfo> PipelineShaderStages = { VertexShaderStage, FragmentShaderStage };

		VkVertexInputBindingDescription InputBinding;
		InputBinding.binding = 0;
		InputBinding.stride = m_Struct.Struct.AttributeLayout.GetStride();
		InputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		uint32 Index = 0;
		Vector<VkVertexInputAttributeDescription> InputAttributes;
		for (const PipelineAttributeElement& element : m_Struct.Struct.AttributeLayout) {
			VkVertexInputAttributeDescription Attribute = {};
			Attribute.binding = 0;
			Attribute.location = Index;
			Attribute.format = GetVulkanShaderAttributeFromEngine(element.Type);
			Attribute.offset = element.Offset;
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

		// TODO: REMOVE RPSS w/ FBO.
		// TODO: ADD VIEWPORT, SCISSOR TO FBO.

		VkViewport viewport = {};
		viewport.width = 1280;
		viewport.height = 720;

		VkRect2D scissor = {};
		scissor.extent = VkExtent2D({ 1280, 720 });

		VkPipelineViewportStateCreateInfo ViewportStateCreateInfo = {};
		ViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		ViewportStateCreateInfo.viewportCount = 1;
		ViewportStateCreateInfo.pViewports = &viewport;
		ViewportStateCreateInfo.scissorCount = 1;
		ViewportStateCreateInfo.pScissors = &scissor;

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

		Vector<VkDynamicState> DynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo DynamicStateCreateInfo = {};
		DynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		DynamicStateCreateInfo.dynamicStateCount = DynamicStates.size();
		DynamicStateCreateInfo.pDynamicStates = DynamicStates.data();

		PipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		PipelineCreateInfo.stageCount = PipelineShaderStages.size();
		PipelineCreateInfo.pStages = PipelineShaderStages.data();
		PipelineCreateInfo.pVertexInputState = &InputState;
		PipelineCreateInfo.pInputAssemblyState = &InputAssemblyStateCreateInfo;
		PipelineCreateInfo.pTessellationState = nullptr;
		PipelineCreateInfo.pViewportState = &ViewportStateCreateInfo;
		PipelineCreateInfo.pRasterizationState = &RasterizationStateCreateInfo;
		PipelineCreateInfo.pMultisampleState = &MultisampleStateCreateInfo;
		PipelineCreateInfo.pDynamicState = &DynamicStateCreateInfo;
		PipelineCreateInfo.layout = m_PipelineLayout;
		PipelineCreateInfo.renderPass = m_Struct.RenderPass;
		PipelineCreateInfo.subpass = m_Struct.SubpassIndex;

		result = vkCreateGraphicsPipelines(m_Device->GetLogical(), m_PipelineCache, 1, &PipelineCreateInfo, nullptr, &m_Pipeline);
		VULKAN_CORE_ASSERT(result, "[VULKAN] Pipeline Creation Failure!");

		vkDestroyShaderModule(m_Device->GetLogical(), VertexModule, nullptr);
		vkDestroyShaderModule(m_Device->GetLogical(), FragmentModule, nullptr);
	}

	void VulkanPipeline::VulkanDestory()
	{
		VulkanDescriptor::Destroy(m_Descriptor);
	}

	Ref<VulkanPipeline> VulkanPipeline::Create(const VulkanPipelineInfo& _PipelineInfo)
	{
		return CreateRef<VulkanPipeline>(_PipelineInfo);
	}

	void VulkanPipeline::Destroy(const Ref<VulkanPipeline>& _Pipeline)
	{
		_Pipeline->VulkanDestory();
	}

}}