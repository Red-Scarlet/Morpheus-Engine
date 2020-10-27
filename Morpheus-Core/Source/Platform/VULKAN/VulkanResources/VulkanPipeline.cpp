#include "Morppch.h"
#include "VulkanPipeline.h"

#include "VulkanVertexBuffer.h"
#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

namespace Morpheus {

	VulkanPipeline::VulkanPipeline()
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_Swapchain = VulkanResourceCache::GetInstance()->Get<VulkanSwapchain>(VulkanResourceType::Swapchain);
		m_Shader = VulkanResourceCache::GetInstance()->Get<VulkanShader>(VulkanResourceType::Shader);
		m_DescriptorPool = VulkanResourceCache::GetInstance()->Get<VulkanDescriptorPool>(VulkanResourceType::DescriptorPool);
		m_Renderpass = VulkanResourceCache::GetInstance()->Get<VulkanRenderpass>(VulkanResourceType::Renderpass);

		VertexInputData();
		CreatePipeline();

		MORP_CORE_WARN("[VULKAN] Pipeline Was Created!");
	}

	VulkanPipeline::~VulkanPipeline()
	{
		MORP_CORE_WARN("[VULKAN] Pipeline Was Destoryed!");
	}

	void VulkanPipeline::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyPipelineCache(m_PipelineCache);
		Device.destroyPipeline(m_Pipeline);
		Device.destroyPipelineLayout(m_PipelineLayout);
	}

	void VulkanPipeline::VertexInputData()
	{
		// Vertex input binding
		m_InputBinding.binding = 0;
		m_InputBinding.stride = sizeof(VertexData);
		m_InputBinding.inputRate = vk::VertexInputRate::eVertex;

		// Inpute attribute binding describe shader attribute locations and memory layouts
		// These match the following shader layout (see assets/shaders/triangle.vert):
		//	layout (location = 0) in vec3 inPos;
		//	layout (location = 1) in vec3 inColor;
		m_InputAttributes.resize(2);
		// Attribute location 0: Position
		m_InputAttributes[0].binding = 0;
		m_InputAttributes[0].location = 0;
		m_InputAttributes[0].format = vk::Format::eR32G32B32Sfloat;
		m_InputAttributes[0].offset = offsetof(VertexData, Position);
		// Attribute location 1: Color
		m_InputAttributes[1].binding = 0;
		m_InputAttributes[1].location = 1;
		m_InputAttributes[1].format = vk::Format::eR32G32B32Sfloat;
		m_InputAttributes[1].offset = offsetof(VertexData, Color);

		// Assign to the vertex input state used for pipeline creation
		m_InputState.flags = vk::PipelineVertexInputStateCreateFlags();
		m_InputState.vertexBindingDescriptionCount = 1;
		m_InputState.pVertexBindingDescriptions = &m_InputBinding;
		m_InputState.vertexAttributeDescriptionCount = (uint32)m_InputAttributes.size();
		m_InputState.pVertexAttributeDescriptions = m_InputAttributes.data();
	}

	void VulkanPipeline::CreatePipeline()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::ShaderModule VertexShader = m_Shader->GetVertexShader();
		vk::ShaderModule PixelShader = m_Shader->GetPixelShader();

		vk::Rect2D RenderArea = m_Swapchain->GetRenderArea();
		vk::Viewport Viewport = m_Swapchain->GetViewport();

		vk::RenderPass Renderpass = m_Renderpass->GetRenderpass();

		m_PipelineCache = Device.createPipelineCache(vk::PipelineCacheCreateInfo());

		// Move into Descriptor Possible Later!
		vk::DescriptorSetLayout Layout = m_DescriptorPool->GetLayoutSet();
		uint32 LayoutCount = 1;
		m_PipelineLayout = Device.createPipelineLayout(
			vk::PipelineLayoutCreateInfo(
				vk::PipelineLayoutCreateFlags(),
				LayoutCount,
				&Layout,
				0,
				nullptr
			)
		);

		Vector<vk::PipelineShaderStageCreateInfo> pipelineShaderStages = {
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eVertex,
				VertexShader,
				"main",
				nullptr
			),
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eFragment,
				PixelShader,
				"main",
				nullptr
			)
		};

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

		// Dept and Stencil state for primative compare/test operations

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
		Vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments =
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
			(uint32)colorBlendAttachments.size(),
			colorBlendAttachments.data()
		);

		Vector<vk::DynamicState> dynamicStates =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo pdy(
			vk::PipelineDynamicStateCreateFlags(),
			(uint32)dynamicStates.size(),
			dynamicStates.data()
		);

		vk::GraphicsPipelineCreateInfo CreateInfo {};
		{
			CreateInfo.flags = vk::PipelineCreateFlags();
			CreateInfo.stageCount = (uint32)pipelineShaderStages.size();
			CreateInfo.pStages = pipelineShaderStages.data();
			CreateInfo.pVertexInputState = &pvi;
			CreateInfo.pInputAssemblyState = &pia;
			CreateInfo.pTessellationState = nullptr;
			CreateInfo.pViewportState = &pv;
			CreateInfo.pRasterizationState = &pr;
			CreateInfo.pMultisampleState = &pm;
		
			CreateInfo.pDepthStencilState = &pds;
			CreateInfo.pColorBlendState = &pbs;
			CreateInfo.pDynamicState = &pdy;
		
			CreateInfo.layout = m_PipelineLayout;
			CreateInfo.renderPass = Renderpass;
		
			CreateInfo.basePipelineIndex = 0;
		}
		
		auto ThePipeline = Device.createGraphicsPipeline(
			m_PipelineCache,
			vk::GraphicsPipelineCreateInfo(
				vk::PipelineCreateFlags(),
				static_cast<uint32_t>(pipelineShaderStages.size()),
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

		m_Pipeline = ThePipeline;

	}

	Ref<VulkanPipeline> VulkanPipeline::VulkanCreate()
	{
		Ref<VulkanPipeline> s_VulkanPipeline = CreateRef<VulkanPipeline>();
		VulkanResourceCache::GetInstance()->Submit<VulkanPipeline>(VulkanResourceType::Pipeline, s_VulkanPipeline);
		return s_VulkanPipeline;
	}

}