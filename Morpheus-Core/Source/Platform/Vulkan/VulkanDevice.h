// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"

#include <vulkan/vulkan.h>
#include <optional>

namespace Morpheus {

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		Vector<VkSurfaceFormatKHR> Formats;
		Vector<VkPresentModeKHR> PresentModes;
	};

	struct QueueFamilyIndices 
	{
		std::optional<UINT32> GraphicsFamily;
		std::optional<UINT32> PresentFamily;

		bool IsComplete() 
		{ return GraphicsFamily.has_value() && PresentFamily.has_value(); }
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(VulkanInstance* Instance, VulkanSurface* Surface);
		~VulkanDevice();

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkDevice GetLogicalDevice() { return m_LogicalDevice; }
		SwapChainSupportDetails GetSwapchainSupportDetails() { return QuerySwapChainSupport(m_PhysicalDevice); }
		QueueFamilyIndices GetQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
		
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() { return m_PresentQueue; }

	private:
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice _Device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device);
		void CreateLogicalDevice();

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice Device);

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanSurface* m_Surface;

		FVector8 m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
	};

}