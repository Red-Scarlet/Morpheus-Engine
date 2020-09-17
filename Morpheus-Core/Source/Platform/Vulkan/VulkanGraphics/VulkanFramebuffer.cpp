// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"

namespace Morpheus {


	VulkanFramebuffer::VulkanFramebuffer(const Ref<Renderpass>& _Renderpass)
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanCore.lDevice = Instance->GetLogicalDevice();
		m_VulkanCore.Presentation = Instance->GetPresentation();
		m_VulkanCore.Renderpass = _Renderpass;

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
		VkFramebufferCreateInfo FramebufferInfo {};
		{
			FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferInfo.renderPass = CastRef<VulkanRenderpass>(m_VulkanCore.Renderpass)->GetRenderpass();
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