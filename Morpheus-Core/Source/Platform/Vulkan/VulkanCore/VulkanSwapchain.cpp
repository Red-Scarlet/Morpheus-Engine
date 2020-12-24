#include "Morppch.h"
#include "VulkanSwapchain.h"

namespace Morpheus { namespace Vulkan {

	VulkanSwapchain::VulkanSwapchain(const VkDevice& _Device, const VkSurfaceKHR& _Surface,
		const SwapchainSupportDetails& _SupportDetails, const QueueFamilyIndices& _Indices)
		: m_Device(_Device), m_Surface(_Surface), m_SupportDetails(_SupportDetails), m_Indices(_Indices)
	{
		CreateSwapchain();
		VULKAN_CORE_WARN("[VULKAN] Swapchain was Created!");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		DestroySwapchain();
		VULKAN_CORE_WARN("[VULKAN] Swapchain was Destroyed!");
	}

	void VulkanSwapchain::Reset()
	{
		// RecreateSwapchain if Asked!
	}

	VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const Vector<VkSurfaceFormatKHR>& AvailableFormats)
	{
		for (const auto& AvailableFormat : AvailableFormats)
			if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return AvailableFormat;
		return AvailableFormats[0];
	}

	VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const Vector<VkPresentModeKHR>& AvailablePresentModes)
	{
		for (const auto& AvailablePresentMode : AvailablePresentModes)
				if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return AvailablePresentMode;
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
	{
		if (Capabilities.currentExtent.width != uint32_max)
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
		MORP_PROFILE_FUNCTION();

		VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(m_SupportDetails.Formats);
		VkPresentModeKHR PresentMode = ChooseSwapPresentMode(m_SupportDetails.PresentModes);
		m_Extent = ChooseSwapExtent(m_SupportDetails.Capabilities);

		m_ImageCount = m_SupportDetails.Capabilities.minImageCount;
		if (m_SupportDetails.Capabilities.maxImageCount > 0 && m_ImageCount > m_SupportDetails.Capabilities.maxImageCount)
			m_ImageCount = m_SupportDetails.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		CreateInfo.surface = m_Surface;
		CreateInfo.minImageCount = m_ImageCount;
		CreateInfo.imageFormat = SurfaceFormat.format;
		CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
		CreateInfo.imageExtent = m_Extent;
		CreateInfo.imageArrayLayers = 1;
		CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		CreateInfo.preTransform = m_SupportDetails.Capabilities.currentTransform;
		CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		CreateInfo.presentMode = PresentMode;
		CreateInfo.clipped = VK_TRUE;
		CreateInfo.oldSwapchain = VK_NULL_HANDLE;

		uint32 QueueFamilyIndices[] = { m_Indices.GraphicsFamily.value(), m_Indices.PresentFamily.value() };

		if (m_Indices.GraphicsFamily != m_Indices.PresentFamily) {
			CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			CreateInfo.queueFamilyIndexCount = 2;
			CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
			MORP_CORE_INFO("[VULKAN] Swapchain: ImageQueue Seperate!");
		} else {
			CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			MORP_CORE_INFO("[VULKAN] Swapchain: ImageQueue Same!");
		}

		VkResult result = vkCreateSwapchainKHR(m_Device, &CreateInfo, nullptr, &m_Swapchain);
		MORP_CORE_ASSERT(result, "Failed to create Swap Chain!");

		m_Format = SurfaceFormat.format;
		m_Viewport = VkViewport({ 0.0f, 0.0f, (float32)m_Extent.width, (float32)m_Extent.height, 0, 1.0f });
		m_Scissor = VkRect2D({ VkOffset2D(), m_Extent });

		CreateImages();
	}

	void VulkanSwapchain::CreateImageViews()
	{
		MORP_PROFILE_FUNCTION();

		m_ImageViews.resize(m_ImageCount);
		for (uint32 i = 0; i < m_ImageCount; i++) {
			VkImageViewCreateInfo CreateInfo = {};
			CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			CreateInfo.image = m_Images[i];
			CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			CreateInfo.format = m_Format;
			CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			CreateInfo.subresourceRange.baseMipLevel = 0;
			CreateInfo.subresourceRange.levelCount = 1;
			CreateInfo.subresourceRange.baseArrayLayer = 0;
			CreateInfo.subresourceRange.layerCount = 1;
			VkResult result = vkCreateImageView(m_Device, &CreateInfo, nullptr, &m_ImageViews[i]);
			MORP_CORE_ASSERT(result, "Failed to create Imageview!");
		}

	}

	void VulkanSwapchain::CreateImages()
	{
		MORP_PROFILE_FUNCTION();

		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &m_ImageCount, m_Images.data());
		CreateImageViews();
	}

	void VulkanSwapchain::DestroySwapchain()
	{
		MORP_PROFILE_FUNCTION();

		for (VkImageView imageview : m_ImageViews)
			vkDestroyImageView(m_Device, imageview, nullptr);
		vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
	}

	Ref<VulkanSwapchain> VulkanSwapchain::Create(const VkDevice& _Device, const VkSurfaceKHR& _Surface, const SwapchainSupportDetails& _SupportDetails, const QueueFamilyIndices& _Indices)
	{
		return CreateRef<VulkanSwapchain>(_Device, _Surface, _SupportDetails, _Indices);
	}

}}