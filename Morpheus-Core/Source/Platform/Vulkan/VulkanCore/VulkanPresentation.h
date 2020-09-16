// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanSurface
	{
	public:
		friend class VulkanPresentation;
		friend class VulkanSwapchain;

	public:
		VulkanSurface();
		~VulkanSurface();

	private:
		void CreateSurface();

	private:
		struct {
			VkSurfaceKHR Surface;
		} m_VulkanObject;
		
	};

	class VulkanSwapchain
	{
	public:
		friend class VulkanPresentation;
		friend class VulkanImageview;

	public:
		VulkanSwapchain(VulkanLogicalDevice* _lDevice, VulkanPhysicalDevice* _pDevice, VulkanSurface* _Surface);
		~VulkanSwapchain();	

	private:
		const VkSurfaceFormatKHR& ChooseSwapSurfaceFormat(const Vector<VkSurfaceFormatKHR>& AvailableFormats);
		const VkPresentModeKHR& ChooseSwapPresentMode(const Vector<VkPresentModeKHR>& AvailablePresentModes);
		const VkExtent2D& ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

		void CreateSwapchain();

	private:
		struct {
			VulkanPhysicalDevice* pDevice;
			VulkanLogicalDevice* lDevice;
			VulkanSurface* Surface;

		} m_VulkanDevice;

		struct {
			VkSwapchainKHR Swapchain;
			Vector<VkImage> Images;
			VkFormat Format;
			VkExtent2D Extent;
		} m_VulkanObject;
	};

	class VulkanImageview
	{
	public:
		friend class VulkanPresentation;

	public:
		VulkanImageview(VulkanLogicalDevice* _lDevice, VulkanSwapchain* _Swapchain);
		~VulkanImageview();

	private:
		void CreateImageview();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanSwapchain* Swapchain;
		} m_VulkanCore;

		struct {
			Vector<VkImageView> Imageviews;
		} m_VulkanObject;

	};

	class VulkanPresentation
	{
	public:
		VulkanPresentation() = default;
		~VulkanPresentation();

		const VkSurfaceKHR& GetSurface() { return m_VulkanSurface->m_VulkanObject.Surface; }

		const VkSwapchainKHR& GetSwapchain() { return m_VulkanSwapchain->m_VulkanObject.Swapchain; }
		const VkImage& GetImage(const uint32& _Index) { return m_VulkanSwapchain->m_VulkanObject.Images[_Index]; }
		const VkFormat& GetFormat() { return m_VulkanSwapchain->m_VulkanObject.Format; }
		const VkExtent2D& GetExtent() { return m_VulkanSwapchain->m_VulkanObject.Extent; }
		const VkImageView& GetImageview(const uint32& _Index) { return m_VulkanImageview->m_VulkanObject.Imageviews[_Index]; }
		const uint32& GetSize() { return m_VulkanSwapchain->m_VulkanObject.Images.size(); }

		void SetupSurface();
		void SetupSwapchain();
		void SetupImageview();

		void ResetSwapchain();
		void ResetImageview();

	private:
		VulkanSurface* m_VulkanSurface;
		VulkanSwapchain* m_VulkanSwapchain;
		VulkanImageview* m_VulkanImageview;
	};

}