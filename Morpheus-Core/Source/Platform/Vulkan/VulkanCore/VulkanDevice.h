// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"
#include <vulkan/vulkan.h>

namespace Morpheus {

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		Vector<VkSurfaceFormatKHR> Formats;
		Vector<VkPresentModeKHR> PresentModes;
	};
	
	struct QueueFamilyIndices
	{
	public:
		Optional<uint32> GraphicsFamily;
		Optional<uint32> PresentFamily;
		bool IsComplete()
		{ return GraphicsFamily.has_value() && PresentFamily.has_value(); }
	};

	class VulkanPhysicalDevice
	{
	public:
		friend class VulkanLogicalDevice;
		friend class VulkanSwapchain;
		friend class VulkanCommandPool;

	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice() = default;

	private:
		bool IsDeviceSuitable(VkPhysicalDevice _Device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice _Device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _Device);
		SwapchainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _Device);	//REMOVE LATER OR CLEAN UP

		void PickPhysicalDevice();

	private:
		struct {
			VkPhysicalDevice Device;
			QueueFamilyIndices Indices;
			Vector<const float8*> Extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
			SwapchainSupportDetails SupportDetails;
		} m_VulkanObject;

	};

	class VulkanLogicalDevice
	{
	public:
		VulkanLogicalDevice(VulkanPhysicalDevice* m_pDevice);
		~VulkanLogicalDevice();

		const VkDevice& GetDevice() { return m_VulkanObject.lDevice; }
		const VkQueue& GetGraphicsQueue() { return m_VulkanObject.GraphicsQueue; }
		const VkQueue& GetPresentQueue() { return m_VulkanObject.PresentQueue; }

		void Wait();

	private:
		void CreateLogicalDevice();

	private:
		struct {
			VulkanPhysicalDevice* pDevice;
			VkDevice lDevice;
			VkQueue GraphicsQueue;
			VkQueue PresentQueue;
		} m_VulkanObject;
	};

}