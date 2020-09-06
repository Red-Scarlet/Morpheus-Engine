// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSurface.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain(VulkanInstance* Instance, VulkanDevice* Device, VulkanSurface* Surface);
		~VulkanSwapchain();

		VkSwapchainKHR GetSwapchain() { return m_SwapChain; }
		Vector<VkImage> GetImageView() { return m_SwapChainImages; }
		VkFormat GetImageFormat() { return m_SwapChainImageFormat; }
		VkExtent2D GetSwapExtent() { return m_SwapChainExtent; }

	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const Vector<VkSurfaceFormatKHR>& AvailableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const Vector<VkPresentModeKHR>& AvailablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);
		void CreateSwapChain();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;
		VulkanSurface* m_VulkanSurface;

		VkSwapchainKHR m_SwapChain;
		Vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

	};

}