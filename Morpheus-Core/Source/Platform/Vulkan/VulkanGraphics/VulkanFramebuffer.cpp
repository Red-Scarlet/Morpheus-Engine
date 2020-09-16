// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanFramebuffer.h"

namespace Morpheus {

	VulkanFramebuffer::VulkanFramebuffer(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation, VulkanRenderpass* _Renderpass)
		: m_VulkanCore({ _lDevice, _Presentation })
	{
		m_VulkanObject.Renderpass = _Renderpass;
		CreateFramebuffer();
		MORP_CORE_WARN("[VULKAN] Framebuffer Was Created!");
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		for (auto Framebuffer : m_VulkanObject.Framebuffers)
			vkDestroyFramebuffer(m_VulkanCore.lDevice->GetDevice(), Framebuffer, nullptr);
	}

	void VulkanFramebuffer::CreateFramebuffer()
	{
		VkFramebufferCreateInfo FramebufferInfo{};
		{
			FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferInfo.renderPass = m_VulkanObject.Renderpass->GetRenderpass();
			FramebufferInfo.attachmentCount = 1;
			FramebufferInfo.width = m_VulkanCore.Presentation->GetExtent().width;
			FramebufferInfo.height = m_VulkanCore.Presentation->GetExtent().height;
			FramebufferInfo.layers = 1;
		}

		uint32 FramebufferSize = m_VulkanCore.Presentation->GetSize();
		m_VulkanObject.Framebuffers.resize(FramebufferSize);
		for (uint32 i = 0; i < FramebufferSize; i++) {
			VkImageView attachments[] = { m_VulkanCore.Presentation->GetImageview(i) };
			FramebufferInfo.pAttachments = attachments;

			VkResult result = vkCreateFramebuffer(m_VulkanCore.lDevice->GetDevice(), &FramebufferInfo, nullptr, &m_VulkanObject.Framebuffers[i]);
			MORP_CORE_ASSERT(result, "Failed to create Framebuffer!");

		}
	}

}