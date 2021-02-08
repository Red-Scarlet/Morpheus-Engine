#include "Morppch.h"
#include "VulkanRenderGraph.h"
#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderBufferInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderTaskInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererRenderPipelineInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"

namespace Morpheus { namespace Vulkan {

	VulkanRenderGraph::VulkanRenderGraph(const AnyData& _Data, const Resource& _Resource)
	{
		m_Device = VulkanInstance::GetInstance()->GetDevice(0);
		m_Layout = AnyCast<RenderGraphLayout>(_Data);
		m_Resource = _Resource;

		VULKAN_CORE_WARN("[VULKAN] RenderGraph Was Created!");
	}

	VulkanRenderGraph::~VulkanRenderGraph()
	{
		VULKAN_CORE_WARN("[VULKAN] RenderGraph Was Destroyed!");
	}

	void VulkanRenderGraph::VulkanUpdate()
	{
		if (ResourceCommand::ContainsComponent<RendererResourceInfo>(m_Resource) == true) {
			RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(m_Resource);
			if (ResourceInfo.Changed & 0x01)
				BuildRenderBuffers();
			if (ResourceInfo.Changed & 0x02)
				BuildRenderPass();
			if (ResourceInfo.Changed & 0x02 | 0x01)
				BuildFrameBuffer();
			if (ResourceInfo.Changed & 0x04)
				BuildPipelines();
		}
	}

	void VulkanRenderGraph::VulkanDestory()
	{
		if (m_FrameBufferCompiled)
			vkDestroyFramebuffer(m_Device->GetLogical(), m_FrameBuffer, nullptr);
		if (m_RenderPassCompiled)
			vkDestroyRenderPass(m_Device->GetLogical(), m_RenderPass, nullptr);
		for (Ref<VulkanRenderBuffer> buffer : m_RenderBuffers)
			VulkanRenderBuffer::Destroy(buffer);
		for (Ref<VulkanPipeline> pipeline : m_Pipelines)
			VulkanPipeline::Destroy(pipeline);
	}

	void VulkanRenderGraph::BuildRenderBuffers()
	{
		if (ResourceCommand::ContainsComponent<RendererRenderBufferInfo>(m_Resource) == true) {
			RendererRenderBufferInfo& RenderBufferInfo = ResourceCommand::GetComponent<RendererRenderBufferInfo>(m_Resource);
			for (Ref<VulkanRenderBuffer> buffer : m_RenderBuffers)
				VulkanRenderBuffer::Destroy(buffer);
			for (RenderBufferElement element : RenderBufferInfo.RenderBuffer)
				m_RenderBuffers.push_back(VulkanRenderBuffer::Create(element));
		}
	}

	void VulkanRenderGraph::BuildRenderPass()
	{
		MORP_PROFILE_FUNCTION();

		if (ResourceCommand::ContainsComponent<RendererRenderTaskInfo>(m_Resource) == true) {
			RendererRenderTaskInfo& RenderTaskInfo = ResourceCommand::GetComponent<RendererRenderTaskInfo>(m_Resource);

			Vector<VkAttachmentDescription> Attachments;
			Vector<VkSubpassDependency> Dependencies;
			Vector<VkSubpassDescription> Descriptions;
			Vector<Tuple<String, Vector<VkAttachmentReference>>> InputReferences, ColorReferences, DepthReferences;

			for (Ref<VulkanRenderBuffer> Buffer : m_RenderBuffers) {
				VkAttachmentDescription Attachment = {};
				Attachment.format = GetVulkanFormatFromEngine(Buffer->GetInfo().Format);
				Attachment.samples = VK_SAMPLE_COUNT_1_BIT;
				Attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				Attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				Attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				Attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				Attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				Attachment.finalLayout = GetVulkanLayoutFromEngine(Buffer->GetInfo().Layout);
				Attachments.push_back(Attachment);
			}

			uint32 SubpassIndex = 0;
			for (RenderGraphElement GraphElement : m_Layout) {
				for (Tuple<String, RenderTaskLayout> TaskLayout : RenderTaskInfo.RenderTasks)
					if (std::get<String>(TaskLayout) == GraphElement.Name) {
						Vector<VkAttachmentReference> Input, Color, Depth;
						for (RenderTaskElement BufferElement : std::get<RenderTaskLayout>(TaskLayout)) {
							for (Ref<VulkanRenderBuffer> Buffer : m_RenderBuffers) {
								if (Buffer->GetInfo().Name == BufferElement.Name) {
									VkAttachmentReference Reference = {};
									Reference.attachment = Buffer->GetInfo().AttachmentIndex;
									Reference.layout = GetVulkanLayoutFromEngine(BufferElement.Attachment);
									switch (BufferElement.State)
									{
									case RenderBufferState::RENDER_OUT:		Color.push_back(Reference);
										break;
									case RenderBufferState::RENDER_IN:		Input.push_back(Reference);
										break;
									case RenderBufferState::RENDER_DEPTH:	Depth.push_back(Reference);
										break;
									}
								}
							}
						}
						// Then Set Information to References
						InputReferences.push_back({ GraphElement.Name, Input });
						ColorReferences.push_back({ GraphElement.Name, Color });
						DepthReferences.push_back({ GraphElement.Name, Depth });
					}
				// Create Subpass
				VkSubpassDescription Description = {};
				Description.flags = 0;
				Description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				Description.inputAttachmentCount = std::get<Vector<VkAttachmentReference>>(InputReferences.at(SubpassIndex)).size();
				Description.pInputAttachments = std::get<Vector<VkAttachmentReference>>(InputReferences.at(SubpassIndex)).data();
				Description.colorAttachmentCount = std::get<Vector<VkAttachmentReference>>(ColorReferences.at(SubpassIndex)).size();
				Description.pColorAttachments = std::get<Vector<VkAttachmentReference>>(ColorReferences.at(SubpassIndex)).data();
				Description.pResolveAttachments = nullptr;
				Description.pDepthStencilAttachment = std::get<Vector<VkAttachmentReference>>(DepthReferences.at(SubpassIndex)).data();
				Description.preserveAttachmentCount = 0;
				Description.pPreserveAttachments = nullptr;
				Descriptions.push_back(Description);

				VkSubpassDependency Dependency = {};
				if (SubpassIndex == 0) Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				else Dependency.srcSubpass = 0;
				Dependency.dstSubpass = SubpassIndex;
				Dependency.srcStageMask = 
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | 
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | 
					VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				Dependency.dstStageMask = 
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | 
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | 
					VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				Dependency.srcAccessMask =
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | 
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				Dependency.dstAccessMask = 
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | 
					VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
				Dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				Dependencies.push_back(Dependency);
				SubpassIndex++;
			}
			VkRenderPassCreateInfo RenderPassInfo = {};
			RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassInfo.pAttachments = Attachments.data();
			RenderPassInfo.attachmentCount = Attachments.size();
			RenderPassInfo.subpassCount = Descriptions.size();
			RenderPassInfo.pSubpasses = Descriptions.data();
			RenderPassInfo.dependencyCount = Dependencies.size();
			RenderPassInfo.pDependencies = Dependencies.data();
			VkResult result = vkCreateRenderPass(m_Device->GetLogical(), &RenderPassInfo, nullptr, &m_RenderPass);
			VULKAN_CORE_ASSERT(result, "[VULKAN] RenderPass Creation Failure!");
			VULKAN_CORE_WARN("[VULKAN] RenderPass was Created!");
			m_RenderPassCompiled = true;
		}
	}	

	void VulkanRenderGraph::BuildFrameBuffer()
	{
		MORP_PROFILE_FUNCTION();
		
		Vector<VkImageView> Attachments;
		for (Ref<VulkanRenderBuffer>& buffer : m_RenderBuffers)
			Attachments.push_back(buffer->GetImageView());
		
		VkFramebufferCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		CreateInfo.pNext = NULL;
		CreateInfo.renderPass = m_RenderPass;
		CreateInfo.pAttachments = Attachments.data();
		CreateInfo.attachmentCount = Attachments.size();
		CreateInfo.width = 1280;
		CreateInfo.height = 720;
		CreateInfo.layers = 1;
		VkResult result = vkCreateFramebuffer(m_Device->GetLogical(), &CreateInfo, nullptr, &m_FrameBuffer);
		VULKAN_CORE_ASSERT(result, "[VULKAN] FrameBuffer Creation Failure!");
		VULKAN_CORE_WARN("[VULKAN] FrameBuffer was Created!");
		m_FrameBufferCompiled = true;
	}

	void VulkanRenderGraph::BuildPipelines()
	{
		if (ResourceCommand::ContainsComponent<RendererRenderPipelineInfo>(m_Resource) == true) {
			RendererRenderPipelineInfo& RenderPipelineInfo = ResourceCommand::GetComponent<RendererRenderPipelineInfo>(m_Resource);
			for (Ref<VulkanPipeline> pipeline : m_Pipelines)
				VulkanPipeline::Destroy(pipeline);
			uint32 SubpassIndex = 0;
			for (RenderGraphElement GraphElement : m_Layout) {
				for (Tuple<String, RenderPipelineStruct> PipelineStruct : RenderPipelineInfo.RenderPipelines)
					if (std::get<String>(PipelineStruct) == GraphElement.Name) {
						
						uint32 ColorBuffers = 0, DepthBuffers = 0;
						for (Ref<VulkanRenderBuffer> Buffer : m_RenderBuffers)
							if (Buffer->GetInfo().Layout == RendererImageLayout::USAGE_DEPTH_STENCIL_ATTACHMENT)
								DepthBuffers++;
							else ColorBuffers++;
						
						VulkanPipelineInfo Struct = {};
						Struct.Struct = std::get<RenderPipelineStruct>(PipelineStruct);
						Struct.RenderPass = m_RenderPass;
						Struct.ColorAttachments = ColorBuffers;
						Struct.DepthAttachments = DepthBuffers;
						Struct.SubpassIndex = SubpassIndex;
						m_Pipelines.push_back(VulkanPipeline::Create(Struct));
					}
				SubpassIndex++;
			}
		}
	}

	Ref<VulkanRenderGraph> VulkanRenderGraph::Create(const AnyData& _Data, const Resource& _Resource)
	{
		return CreateRef<VulkanRenderGraph>(_Data, _Resource);
	}

	void VulkanRenderGraph::Destroy(const Ref<VulkanRenderGraph>& _RenderGraph)
	{
		_RenderGraph->VulkanDestory();
	}

	void VulkanRenderGraph::Update(const Ref<VulkanRenderGraph>& _RenderGraph)
	{
		_RenderGraph->VulkanUpdate();
	}

}}