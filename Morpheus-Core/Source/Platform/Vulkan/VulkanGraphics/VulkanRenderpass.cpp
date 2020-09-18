// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	VulkanRenderpass::VulkanRenderpass()
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanCore.lDevice = Instance->GetLogicalDevice();
		m_VulkanCore.Presentation = Instance->GetPresentation();

		CreateRenderpass();
		MORP_CORE_WARN("[VULKAN] Renderpass Was Created!");
		CreateFramebuffer();
		MORP_CORE_WARN("[VULKAN] Framebuffer Was Created!");
	}

	VulkanRenderpass::~VulkanRenderpass()
	{
		for (auto Framebuffer : m_VulkanObject.Framebuffers)
			vkDestroyFramebuffer(m_VulkanCore.lDevice->GetDevice(), Framebuffer, nullptr);
		vkDestroyRenderPass(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.Renderpass, nullptr);
	}

	const VkFramebuffer& VulkanRenderpass::GetFramebuffer(const uint32& _Index)
	{
		return m_VulkanObject.Framebuffers[_Index];
	}

	const VkRenderPassBeginInfo& VulkanRenderpass::GetBeginInfo(const VkClearValue& _Color, const VkExtent2D& _Extent, const uint32& _Index)
	{
		VkRenderPassBeginInfo BeginInfo {};
		{
			BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			BeginInfo.renderPass = m_VulkanObject.Renderpass;
			BeginInfo.framebuffer = m_VulkanObject.Framebuffers[_Index];
			BeginInfo.renderArea.offset = { 0, 0 };
			BeginInfo.renderArea.extent = _Extent;
			BeginInfo.clearValueCount = 1;
			BeginInfo.pClearValues = &_Color;
		}
		return BeginInfo;
	}

	void VulkanRenderpass::CreateRenderpass()
	{
		VkAttachmentDescription ColorAttachment {};
		{
			ColorAttachment.format = m_VulkanCore.Presentation->GetFormat();
			ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		}

		VkAttachmentReference ColorAttachmentRef{};
		{
			ColorAttachmentRef.attachment = 0;
			ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		VkSubpassDescription Subpass {};
		{
			Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			Subpass.colorAttachmentCount = 1;
			Subpass.pColorAttachments = &ColorAttachmentRef;
		}	

		VkSubpassDependency Dependency {};
		{
			Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			Dependency.dstSubpass = 0;
			Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			Dependency.srcAccessMask = 0;
			Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}

		VkRenderPassCreateInfo CreateInfo{};
		{
			CreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			CreateInfo.attachmentCount = 1;
			CreateInfo.pAttachments = &ColorAttachment;
			CreateInfo.subpassCount = 1;
			CreateInfo.pSubpasses = &Subpass;
			CreateInfo.dependencyCount = 1;
			CreateInfo.pDependencies = &Dependency;
		}

		VkResult result = vkCreateRenderPass(m_VulkanCore.lDevice->GetDevice(), &CreateInfo, nullptr, &m_VulkanObject.Renderpass);
		MORP_CORE_ASSERT(result, "Failed to create Renderpass!")
	}

	void VulkanRenderpass::CreateFramebuffer()
	{
		VkFramebufferCreateInfo FramebufferInfo {};
		{
			FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferInfo.renderPass = m_VulkanObject.Renderpass;
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