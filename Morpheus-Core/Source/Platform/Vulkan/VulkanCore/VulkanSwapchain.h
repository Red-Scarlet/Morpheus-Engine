#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanQueue.h"

namespace Morpheus { namespace Vulkan {

	struct SwapchainSupportDetails
	{
	public:
		VkSurfaceCapabilitiesKHR Capabilities;
		Vector<VkSurfaceFormatKHR> Formats;
		Vector<VkPresentModeKHR> PresentModes;
	};

	class VulkanSwapchain 
	{
	public:
		VulkanSwapchain(const VkDevice& _Device, const VkSurfaceKHR& _Surface, 
			const SwapchainSupportDetails& _SupportDetails, const QueueFamilyIndices& _Indices);
		virtual ~VulkanSwapchain();

		void Reset();
		const uint32& GetImageCount() const { return m_ImageCount; }

	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const Vector<VkSurfaceFormatKHR>& AvailableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const Vector<VkPresentModeKHR>& AvailablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

		void CreateSwapchain();
		void CreateImageViews();
		void CreateImages();
		void DestroySwapchain();

	private:
		VkDevice m_Device;
		VkSurfaceKHR m_Surface;
		SwapchainSupportDetails m_SupportDetails;
		QueueFamilyIndices m_Indices;

		VkSwapchainKHR m_Swapchain;
		VkExtent2D m_Extent;
		VkFormat m_Format;
		VkViewport m_Viewport;
		VkRect2D m_Scissor;
		
		Vector<VkImageView> m_ImageViews;
		Vector<VkImage> m_Images;
		uint32 m_ImageCount;

	public:
		static Ref<VulkanSwapchain> Create(const VkDevice& _Device, const VkSurfaceKHR& _Surface,
			const SwapchainSupportDetails& _SupportDetails, const QueueFamilyIndices& _Indices);
		static void Destroy(const Ref<VulkanSwapchain>& _Swapchain);
	};

}}