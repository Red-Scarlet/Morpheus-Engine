#include "Morppch.h"
#include "VulkanFramebuffer.h"

namespace Morpheus {

	VulkanFramebuffer::VulkanFramebuffer(VulkanInstance* Instance, VulkanDevice* Device, VulkanImageview* Imageview, VulkanRenderpass* Renderpass)
		: m_VulkanInstance(Instance), m_VulkanDevice(Device), m_VulkanImageview(Imageview), m_VulkanRenderpass(Renderpass)
	{
		CreateFramebuffer();
		MORP_CORE_WARN("[VULKAN] Vulkan Framebuffer Was Created!");
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		for (auto framebuffer : m_SwapChainFramebuffers) {
			vkDestroyFramebuffer(m_VulkanDevice->GetLogicalDevice(), framebuffer, nullptr);
		}
	}

	void VulkanFramebuffer::CreateFramebuffer()
	{
		UINT32 FramebufferSize = m_VulkanImageview->GetImageViews().size();

		m_SwapChainFramebuffers.resize(FramebufferSize);
		for (size_t i = 0; i < FramebufferSize; i++) {
			VkImageView attachments[] = {
				m_VulkanImageview->GetImageViews()[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_VulkanRenderpass->GetRenderpass();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_VulkanImageview->GetSwapchainExtent().width;
			framebufferInfo.height = m_VulkanImageview->GetSwapchainExtent().height;
			framebufferInfo.layers = 1;

			VkResult result = vkCreateFramebuffer(m_VulkanDevice->GetLogicalDevice(), &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]);
			MORP_CORE_ASSERT(result, "Failed to create Framebuffer!");

		}
	}

}