// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanPresentation.h"

#include "Morpheus/Core/Application.h"
#include "VulkanInstance.h"
#include <GLFW/glfw3.h>

namespace Morpheus {

	// ! -------------- SURFACE -------------------------------------------------------------------------------------------->

	VulkanSurface::VulkanSurface()
	{
		CreateSurface();
		MORP_CORE_WARN("[VULKAN] Surface Was Created!");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(VulkanInstance::GetInstance()->GetVulkan(), m_VulkanObject.Surface, nullptr);
	}

	void VulkanSurface::CreateSurface()
	{
		Window& GLFW = Application::Get().GetWindow();

		VkResult result = glfwCreateWindowSurface(VulkanInstance::GetInstance()->GetVulkan(),
			(GLFWwindow*)GLFW.GetWindowCore(), nullptr, &m_VulkanObject.Surface);
		MORP_CORE_ASSERT(result, "Failed to create Window Surface!");
	}

	// ! -------------- SWAPCHAIN ------------------------------------------------------------------------------------------>

	VulkanSwapchain::VulkanSwapchain(VulkanLogicalDevice* _lDevice, VulkanPhysicalDevice* _pDevice, VulkanSurface* _Surface)
		: m_VulkanDevice({ _pDevice, _lDevice, _Surface })
	{
		CreateSwapchain();
		MORP_CORE_WARN("[VULKAN] Swapchain Was Created!");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(m_VulkanDevice.lDevice->GetDevice(), m_VulkanObject.Swapchain, nullptr);
	}

	const VkSurfaceFormatKHR& VulkanSwapchain::ChooseSwapSurfaceFormat(const Vector<VkSurfaceFormatKHR>& AvailableFormats)
	{
		for (const auto& AvailableFormat : AvailableFormats) 
			if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
				return AvailableFormat;
		return AvailableFormats[0];
	}

	const VkPresentModeKHR& VulkanSwapchain::ChooseSwapPresentMode(const Vector<VkPresentModeKHR>& AvailablePresentModes)
	{
		for (const auto& AvailablePresentMode : AvailablePresentModes) 
			if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
				return AvailablePresentMode;
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	const VkExtent2D& VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
	{
		if (Capabilities.currentExtent.width != UINT32_MAX) 
			return Capabilities.currentExtent;	
		else {
			VkExtent2D ActualExtent = { 1280, 720 };	//TODO: CHANGE TO WINDOW SIZE!
			ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
			ActualExtent.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));
			return ActualExtent;
		}
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		SwapchainSupportDetails SwapchainSupport = m_VulkanDevice.pDevice->m_VulkanObject.SupportDetails;
		VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SwapchainSupport.Formats);
		VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SwapchainSupport.PresentModes);
		VkExtent2D Extent = ChooseSwapExtent(SwapchainSupport.Capabilities);

		uint32 ImageCount = SwapchainSupport.Capabilities.minImageCount + 1;
		if (SwapchainSupport.Capabilities.maxImageCount > 0 && ImageCount > SwapchainSupport.Capabilities.maxImageCount)
			ImageCount = SwapchainSupport.Capabilities.maxImageCount;
		
		VkSwapchainCreateInfoKHR createInfo {};
		{
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = m_VulkanDevice.Surface->m_VulkanObject.Surface;
			createInfo.minImageCount = ImageCount;
			createInfo.imageFormat = SurfaceFormat.format;
			createInfo.imageColorSpace = SurfaceFormat.colorSpace;
			createInfo.imageExtent = Extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		
			//todo
			QueueFamilyIndices Indices = m_VulkanDevice.pDevice->m_VulkanObject.Indices;
			uint32 QueueFamilyIndicesArray[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };
			if (Indices.GraphicsFamily != Indices.PresentFamily) {
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = QueueFamilyIndicesArray;
			}
			else createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

			createInfo.preTransform = SwapchainSupport.Capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = PresentMode;
			createInfo.clipped = VK_TRUE;
			createInfo.oldSwapchain = VK_NULL_HANDLE;
		}

		VkResult result = vkCreateSwapchainKHR(m_VulkanDevice.lDevice->GetDevice(), &createInfo, nullptr, &m_VulkanObject.Swapchain);
		MORP_CORE_ASSERT(result, "Failed to create Swap Chain!");

		vkGetSwapchainImagesKHR(m_VulkanDevice.lDevice->GetDevice(), m_VulkanObject.Swapchain, &ImageCount, nullptr);
		m_VulkanObject.Images.resize(ImageCount);
		vkGetSwapchainImagesKHR(m_VulkanDevice.lDevice->GetDevice(), m_VulkanObject.Swapchain, &ImageCount, m_VulkanObject.Images.data());
		m_VulkanObject.Format = SurfaceFormat.format;
		m_VulkanObject.Extent = Extent;
	}

	// ! -------------- Imageview --------------------------------------------------------------------------------------->

	VulkanImageview::VulkanImageview(VulkanLogicalDevice* _lDevice, VulkanSwapchain* _Swapchain)
		: m_VulkanCore({ _lDevice, _Swapchain })
	{
		CreateImageview();
		MORP_CORE_WARN("[VULKAN] Imageview Was Created!");
	}

	VulkanImageview::~VulkanImageview()
	{
		for (auto ImageView : m_VulkanObject.Imageviews)
			vkDestroyImageView(m_VulkanCore.lDevice->GetDevice(), ImageView, nullptr);
	}

	void VulkanImageview::CreateImageview()
	{
		uint32 ImageviewSize = m_VulkanCore.Swapchain->m_VulkanObject.Images.size();
		m_VulkanObject.Imageviews.resize(ImageviewSize);

		for (uint32 i = 0; i < ImageviewSize; i++) {
			VkImageViewCreateInfo createInfo {};
			{
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_VulkanCore.Swapchain->m_VulkanObject.Images[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_VulkanCore.Swapchain->m_VulkanObject.Format;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;
			}

			VkResult result = vkCreateImageView(m_VulkanCore.lDevice->GetDevice(), &createInfo, nullptr, &m_VulkanObject.Imageviews[i]);
			MORP_CORE_ASSERT(result, "Failed to create Imageview!");
		}
	}

	// ! -------------- PRESENTATION --------------------------------------------------------------------------------------->

	VulkanPresentation::~VulkanPresentation()
	{
		delete m_VulkanImageview;
		delete m_VulkanSwapchain;
		delete m_VulkanSurface;
	}

	void VulkanPresentation::SetupSurface()
	{
		m_VulkanSurface = new VulkanSurface();
	}

	void VulkanPresentation::SetupSwapchain()
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanSwapchain = new VulkanSwapchain(Instance->GetLogicalDevice(), Instance->GetPhysicalDevice(), m_VulkanSurface);

	}

	void VulkanPresentation::SetupImageview()
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanImageview = new VulkanImageview(Instance->GetLogicalDevice(), m_VulkanSwapchain);
	}

}