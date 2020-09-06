// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanSwapchain.h"
#include "VulkanInstance.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanImageview
	{
	public:
		VulkanImageview(VulkanInstance* Instance, VulkanDevice* Device, VulkanSwapchain* Swapchain);
		~VulkanImageview();

		Vector<VkImageView> GetImageViews() { return m_SwapChainImageViews; }
		VkExtent2D GetSwapchainExtent() { return m_VulkanSwapchain->GetSwapExtent(); }

	private:
		void CreateImageview();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;
		VulkanSwapchain* m_VulkanSwapchain;
		Vector<VkImageView> m_SwapChainImageViews;
	};

}