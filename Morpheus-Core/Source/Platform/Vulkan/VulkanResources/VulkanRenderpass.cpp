#include "Morppch.h"
#include "VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanRenderpass::VulkanRenderpass(const RenderpassLayout& _Layout)
		: m_Layout(_Layout)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_Surface = VulkanMemoryManager::GetInstance()->GetSurface();
		m_ID = VulkanMemoryManager::GetInstance()->GetRenderpassCache().Count();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Renderpass #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanRenderpass::~VulkanRenderpass()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] Renderpass Was Destoryed!");
	}

	void VulkanRenderpass::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Vector<RenderpassElement> Elements = m_Layout.GetElements();
		VkFormat ColorFormat = VkFormat(m_Surface->GetColorFormat());
		VkFormat DepthFormat = VkFormat(m_Surface->GetDepthFormat());

		Vector<VkAttachmentDescription> Attachments;
		Vector<VkAttachmentReference> ColorReferences;
		Vector<VkAttachmentReference> DepthReferences;

		Vector<VkSubpassDependency> Dependencies;
		Vector<VkSubpassDescription> Descriptions;

		for (uint32 i = 0; i < Elements.size(); i++) {
			VkAttachmentLoadOp LoadType; VkAttachmentStoreOp StoreType;

			switch (Elements[i].LoadAttachment) {
				case RenderpassLoadAttachment::ATTACHMENT_LOAD:
					LoadType = VK_ATTACHMENT_LOAD_OP_LOAD;
					break;
				case RenderpassLoadAttachment::ATTACHMENT_CLEAR:
					LoadType = VK_ATTACHMENT_LOAD_OP_CLEAR;
					break;
				case RenderpassLoadAttachment::ATTACHMENT_DONTCARE:
					LoadType = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					break;
			}
			switch (Elements[i].StoreAttachment) {
				case RenderpassStoreAttachment::ATTACHMENT_STORE:
					StoreType = VK_ATTACHMENT_STORE_OP_STORE;
					break;
				case RenderpassStoreAttachment::ATTACHMENT_DONTCARE:
					StoreType = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					break;
			}

			if (Elements[i].Type == RenderpassTypes::ATTACHMENT_COLOR) {
				VkAttachmentDescription ColorAttachment = {};
				ColorAttachment.format = ColorFormat;
				ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
				ColorAttachment.loadOp = LoadType;
				ColorAttachment.storeOp = StoreType;
				ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				if (m_ID > 0) ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				else ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				Attachments.push_back(ColorAttachment);

				VkAttachmentReference ColorReference = {};
				ColorReference.attachment = 0;//Attachments.size();
				ColorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				ColorReferences.push_back(ColorReference);

				VkSubpassDependency Dependency = {};
				Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				Dependency.dstSubpass = 0;
				Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				Dependency.srcAccessMask = 0;
				Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				Dependencies.push_back(Dependency);
			}

			if (Elements[i].Type == RenderpassTypes::ATTACHMENT_DEPTH) {
				VkAttachmentDescription DepthAttachment = {};
				DepthAttachment.format = DepthFormat;
				DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
				DepthAttachment.loadOp = LoadType;
				DepthAttachment.storeOp = StoreType;
				DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				Attachments.push_back(DepthAttachment);

				VkAttachmentReference DepthReference = {};
				DepthReference.attachment = 1;// Attachments.size();
				DepthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				DepthReferences.push_back(DepthReference);

				VkSubpassDependency Dependency = {};
				Dependency.srcSubpass = 0;
				Dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				Dependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				Dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				Dependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				Dependencies.push_back(Dependency);
			}
		}

		VkSubpassDescription Description = {};
		Description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		Description.inputAttachmentCount = 0;
		Description.pInputAttachments = nullptr;
		Description.colorAttachmentCount = ColorReferences.size();
		Description.pColorAttachments = ColorReferences.data();
		Description.pPreserveAttachments = nullptr;
		Description.pDepthStencilAttachment = DepthReferences.data();
		Description.preserveAttachmentCount = 0;
		Description.pPreserveAttachments = nullptr;
		Descriptions.push_back(Description);

		VkRenderPassCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		CreateInfo.attachmentCount = Attachments.size();
		CreateInfo.pAttachments = Attachments.data();
		CreateInfo.subpassCount = Descriptions.size();
		CreateInfo.pSubpasses = Descriptions.data();
		CreateInfo.dependencyCount = Dependencies.size();
		CreateInfo.pDependencies = Dependencies.data();

		VkResult result = vkCreateRenderPass(Device, &CreateInfo, nullptr, &m_Renderpass);
	}

	void VulkanRenderpass::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyRenderPass(m_Renderpass);
	}

	Ref<VulkanRenderpass> VulkanRenderpass::Make(const RenderpassLayout& _Layout)
	{
		Ref<VulkanRenderpass> s_VulkanRenderpass = CreateRef<VulkanRenderpass>(_Layout);
		VulkanMemoryManager::GetInstance()->GetRenderpassCache().Add(s_VulkanRenderpass->GetID(), s_VulkanRenderpass);
		return s_VulkanRenderpass;
	}

}