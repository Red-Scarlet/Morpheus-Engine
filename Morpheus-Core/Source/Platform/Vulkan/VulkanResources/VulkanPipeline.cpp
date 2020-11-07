#include "Morppch.h"
#include "VulkanPipeline.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanPipeline::VulkanPipeline(const VulkanPipelineInput& _Input, const vk::PipelineVertexInputStateCreateInfo& _InputState)
		: m_ShaderData(_Input), m_InputState(_InputState)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		m_DescriptorPool = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanDescriptorPool>(VulkanResourceTypes::VulkanDescriptor);
		m_Renderpass = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanRenderpass>(VulkanResourceTypes::VulkanRenderpass);

		CreatePipeline();

		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanPipeline));
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

	void VulkanPipeline::CreatePipeline()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
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
				m_ShaderData.VertModule,
				"main",	// Change to VertexMain
				nullptr
			),
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eFragment,
				m_ShaderData.FragModule,
				"main",	// Change to FragmentMain
				nullptr
			)
		};

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

		vk::PipelineColorBlendStateCreateInfo BlendStateCreateInfo = {};
		{
			BlendStateCreateInfo.flags = vk::PipelineColorBlendStateCreateFlags();
			BlendStateCreateInfo.logicOpEnable = 0;
			BlendStateCreateInfo.logicOp = vk::LogicOp::eClear;
			BlendStateCreateInfo.attachmentCount = ColorBlendAttachments.size();
			BlendStateCreateInfo.pAttachments = ColorBlendAttachments.data();
		}

		Vector<vk::DynamicState> DynamicStates =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo DynamicStateCreateInfo = {};
		{
			DynamicStateCreateInfo.flags = vk::PipelineDynamicStateCreateFlags();
			DynamicStateCreateInfo.dynamicStateCount = DynamicStates.size();
			DynamicStateCreateInfo.pDynamicStates = DynamicStates.data();
		}

		vk::GraphicsPipelineCreateInfo CreateInfo {};
		{
			CreateInfo.flags = vk::PipelineCreateFlags();
			CreateInfo.stageCount = (uint32)pipelineShaderStages.size();
			CreateInfo.pStages = pipelineShaderStages.data();
			CreateInfo.pVertexInputState = &m_InputState;
			CreateInfo.pInputAssemblyState = &pia;
			CreateInfo.pTessellationState = nullptr;
			CreateInfo.pViewportState = &pv;
			CreateInfo.pRasterizationState = &pr;
			CreateInfo.pMultisampleState = &pm;
		
			CreateInfo.pDepthStencilState = &pds;
			CreateInfo.pColorBlendState = &BlendStateCreateInfo;
			CreateInfo.pDynamicState = &DynamicStateCreateInfo;

			CreateInfo.layout = m_PipelineLayout;
			CreateInfo.renderPass = Renderpass;
			CreateInfo.basePipelineIndex = 0;
		}
		
		auto Pipeline = Device.createGraphicsPipeline(m_PipelineCache, CreateInfo);
		m_Pipeline = Pipeline;
	}

	Ref<VulkanPipeline> VulkanPipeline::VulkanCreate(const VulkanPipelineInput& _Input, const vk::PipelineVertexInputStateCreateInfo& _InputState)
	{
		Ref<VulkanPipeline> s_VulkanPipeline = CreateRef<VulkanPipeline>(_Input, _InputState);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanPipeline>(VulkanResourceTypes::VulkanPipeline, s_VulkanPipeline);
		return s_VulkanPipeline;
	}

}