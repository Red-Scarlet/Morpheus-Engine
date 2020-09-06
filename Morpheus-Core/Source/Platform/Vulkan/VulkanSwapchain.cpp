// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"
#include "VulkanSwapchain.h"

namespace Morpheus {

	VulkanSwapchain::VulkanSwapchain(VulkanInstance* Instance, VulkanDevice* Device, VulkanSurface* Surface)
        : m_VulkanInstance(Instance), m_VulkanDevice(Device), m_VulkanSurface(Surface)
	{       
        CreateSwapChain();
        MORP_CORE_WARN("[VULKAN] Vulkan Swapchain Was Created!");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
        vkDestroySwapchainKHR(m_VulkanDevice->GetLogicalDevice(), m_SwapChain, nullptr);
	}

    VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const Vector<VkSurfaceFormatKHR>& AvailableFormats)
    {
        for (const auto& availableFormat : AvailableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
    
        return AvailableFormats[0];
    }
    
    VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const Vector<VkPresentModeKHR>& AvailablePresentModes)
    {
        for (const auto& availablePresentMode : AvailablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }
    
        return VK_PRESENT_MODE_FIFO_KHR;
    }
    
    VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
    {
        if (Capabilities.currentExtent.width != UINT32_MAX) {
            return Capabilities.currentExtent;
        }
        else {
            VkExtent2D actualExtent = { 1280, 720 };
    
            actualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, actualExtent.height));
    
            return actualExtent;
        }
    }
    
    void VulkanSwapchain::CreateSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = m_VulkanDevice->GetSwapchainSupportDetails();
    
        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);
    
        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
            imageCount = swapChainSupport.Capabilities.maxImageCount;
        }
    
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_VulkanSurface->GetSurface();
    
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
        QueueFamilyIndices indices = m_VulkanDevice->GetQueueFamilies();
        uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };
    
        if (indices.GraphicsFamily != indices.PresentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }
    
        createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
    
        createInfo.oldSwapchain = VK_NULL_HANDLE;
    
        VkResult result = vkCreateSwapchainKHR(m_VulkanDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChain);
        MORP_CORE_ASSERT(result, "Failed to create Swap Chain!");
    
        vkGetSwapchainImagesKHR(m_VulkanDevice->GetLogicalDevice(), m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_VulkanDevice->GetLogicalDevice(), m_SwapChain, &imageCount, m_SwapChainImages.data());
    
        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
    }

}