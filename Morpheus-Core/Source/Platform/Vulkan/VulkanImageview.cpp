// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"
#include "VulkanImageview.h"

namespace Morpheus {

	VulkanImageview::VulkanImageview(VulkanInstance* Instance, VulkanDevice* Device, VulkanSwapchain* Swapchain)
		: m_VulkanInstance(Instance), m_VulkanDevice(Device), m_VulkanSwapchain(Swapchain)
	{
		CreateImageview();
		MORP_CORE_WARN("[VULKAN] Vulkan Imageview Was Created!");
	}

	VulkanImageview::~VulkanImageview()
	{
		for (auto ImageView : m_SwapChainImageViews)
			vkDestroyImageView(m_VulkanDevice->GetLogicalDevice(), ImageView, nullptr);
	}

	void VulkanImageview::CreateImageview()
	{
		UINT32 ImageviewSize = m_VulkanSwapchain->GetImageView().size();
		m_SwapChainImageViews.resize(ImageviewSize);

		for (UINT32 i = 0; i < ImageviewSize; i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_VulkanSwapchain->GetImageView()[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_VulkanSwapchain->GetImageFormat();
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(m_VulkanDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]);
			MORP_CORE_ASSERT(result, "Failed to create Imageview!");
		}
	}

}