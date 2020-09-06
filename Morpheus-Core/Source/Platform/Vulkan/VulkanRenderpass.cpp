// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"
#include "VulkanRenderpass.h"

namespace Morpheus {

	VulkanRenderpass::VulkanRenderpass(VulkanInstance* Instance, VulkanDevice* Device, VulkanSwapchain* Swapchain)
		: m_VulkanInstance(Instance), m_VulkanDevice(Device), m_VulkanSwapchain(Swapchain)
	{
		CreateRenderpass();
		MORP_CORE_WARN("[VULKAN] Vulkan Renderpass Was Created!");

	}

	VulkanRenderpass::~VulkanRenderpass()
	{
		vkDestroyRenderPass(m_VulkanDevice->GetLogicalDevice(), m_RenderPass, nullptr);
	}

	void VulkanRenderpass::CreateRenderpass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_VulkanSwapchain->GetImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo RenderPassInfo {};
		RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		RenderPassInfo.attachmentCount = 1;
		RenderPassInfo.pAttachments = &colorAttachment;
		RenderPassInfo.subpassCount = 1;
		RenderPassInfo.pSubpasses = &subpass;

		m_RenderPassInfo = RenderPassInfo;

		VkResult result = vkCreateRenderPass(m_VulkanDevice->GetLogicalDevice(), &m_RenderPassInfo, nullptr, &m_RenderPass);
		MORP_CORE_ASSERT(result, "Failed to create Renderpass!")
	}

}