// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	//VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation

	VulkanRenderpass::VulkanRenderpass()
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanCore.lDevice = Instance->GetLogicalDevice();
		m_VulkanCore.Presentation = Instance->GetPresentation();

		CreateRenderpass();
		MORP_CORE_WARN("[VULKAN] Renderpass Was Created!");
	}

	VulkanRenderpass::~VulkanRenderpass()
	{
		vkDestroyRenderPass(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.Renderpass, nullptr);
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

		VkRenderPassCreateInfo RenderPassInfo {};
		{
			RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassInfo.attachmentCount = 1;
			RenderPassInfo.pAttachments = &ColorAttachment;
			RenderPassInfo.subpassCount = 1;
			RenderPassInfo.pSubpasses = &Subpass;
		}

		m_VulkanObject.Info = RenderPassInfo;

		VkResult result = vkCreateRenderPass(m_VulkanCore.lDevice->GetDevice(), &m_VulkanObject.Info, nullptr, &m_VulkanObject.Renderpass);
		MORP_CORE_ASSERT(result, "Failed to create Renderpass!")
	}

}