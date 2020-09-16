// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanDevice.h"
	
#include "VulkanInstance.h"

namespace Morpheus {

	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{
		PickPhysicalDevice();
		MORP_CORE_WARN("[VULKAN] Physical Device Was Created!");
	}

	bool VulkanPhysicalDevice::IsDeviceSuitable(VkPhysicalDevice _Device)
	{
		m_VulkanObject.Indices = FindQueueFamilies(_Device);
		bool ExtensionsSupported = CheckDeviceExtensionSupport(_Device);

		bool SwapChainAdequate = false;

		if (ExtensionsSupported) {
			m_VulkanObject.SupportDetails = QuerySwapChainSupport(_Device);
			SwapChainAdequate = !m_VulkanObject.SupportDetails.Formats.empty() && !m_VulkanObject.SupportDetails.PresentModes.empty();
		}

		return m_VulkanObject.Indices.IsComplete() && ExtensionsSupported && SwapChainAdequate;
	}

	bool VulkanPhysicalDevice::CheckDeviceExtensionSupport(VkPhysicalDevice _Device)
	{
		uint32 ExtensionCount;
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionCount, nullptr);

		Vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionCount, AvailableExtensions.data());

		std::set<String> RequiredExtensions(m_VulkanObject.Extensions.begin(), m_VulkanObject.Extensions.end());

		for (const auto& Extension : AvailableExtensions)
			RequiredExtensions.erase(Extension.extensionName);
		return RequiredExtensions.empty();
	}

	QueueFamilyIndices VulkanPhysicalDevice::FindQueueFamilies(VkPhysicalDevice _Device)
	{
		QueueFamilyIndices Indices;

		uint32 QueueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, nullptr);

		Vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, QueueFamilies.data());

		uint32 Index = 0;
		for (const auto& QueueFamily : QueueFamilies) {
			if (QueueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT)
				Indices.GraphicsFamily = Index;
			VkBool32 PresentSupport = false;
			VkSurfaceKHR Surface = VulkanInstance::GetInstance()->m_VulkanObject.Presentation->GetSurface();
			vkGetPhysicalDeviceSurfaceSupportKHR(_Device, Index, Surface, &PresentSupport);

			if (PresentSupport)
				Indices.PresentFamily = Index;
			if (Indices.IsComplete())
				break;
			Index++;
		}

		return Indices;
	}

	SwapchainSupportDetails VulkanPhysicalDevice::QuerySwapChainSupport(VkPhysicalDevice _Device)
	{
		SwapchainSupportDetails Details;		

		VkSurfaceKHR Surface = VulkanInstance::GetInstance()->m_VulkanObject.Presentation->GetSurface();
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, Surface, &Details.Capabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, Surface, &formatCount, nullptr);

		if (formatCount != 0) {
			Details.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, Surface, &formatCount, Details.Formats.data());
		}

		uint32 presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, Surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			Details.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, Surface, &presentModeCount, Details.PresentModes.data());
		}

		return Details;
	}

	void VulkanPhysicalDevice::PickPhysicalDevice()
	{
		uint32 DeviceCount = 0;
		vkEnumeratePhysicalDevices(VulkanInstance::GetInstance()->m_VulkanObject.Vulkan, &DeviceCount, nullptr);

		if (DeviceCount != 0) {
			Vector<VkPhysicalDevice> Devices(DeviceCount);
			vkEnumeratePhysicalDevices(VulkanInstance::GetInstance()->m_VulkanObject.Vulkan, &DeviceCount, Devices.data());

			// Check For Suitable Devices
			for (const auto& device : Devices) {
				if (IsDeviceSuitable(device)) {
					m_VulkanObject.Device = device;
					break;
				}
			}

			if(m_VulkanObject.Device == nullptr)
				MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Unable to find suitable GPU!");
			return;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Unable to find suitable GPU(s) with Vulkan Support");
	}

	VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice* m_pDevice)
	{
		m_VulkanObject.pDevice = m_pDevice;
		CreateLogicalDevice();
		MORP_CORE_WARN("[VULKAN] Logical Device Was Created!");
	}

	VulkanLogicalDevice::~VulkanLogicalDevice()
	{
		vkDestroyDevice(m_VulkanObject.lDevice, nullptr);
	}

	void VulkanLogicalDevice::Wait()
	{
		vkDeviceWaitIdle(m_VulkanObject.lDevice);
	}

	void VulkanLogicalDevice::CreateLogicalDevice()
	{
		QueueFamilyIndices Indices = m_VulkanObject.pDevice->m_VulkanObject.Indices;

		Vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
		std::set<uint32> UniqueQueueFamilies = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };

		float QueuePriority = 1.00f;
		for (uint32 QueueFamily : UniqueQueueFamilies) {
			VkDeviceQueueCreateInfo QueueCreateInfo {};
			QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueCreateInfo.queueFamilyIndex = QueueFamily;
			QueueCreateInfo.queueCount = 1;
			QueueCreateInfo.pQueuePriorities = &QueuePriority;
			QueueCreateInfos.push_back(QueueCreateInfo);
		}

		VkPhysicalDeviceFeatures DeviceFeatures {};

		VkDeviceCreateInfo CreateInfo {};
		{
			CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			CreateInfo.queueCreateInfoCount = QueueCreateInfos.size();
			CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
			CreateInfo.pEnabledFeatures = &DeviceFeatures;
			CreateInfo.enabledExtensionCount = m_VulkanObject.pDevice->m_VulkanObject.Extensions.size();
			CreateInfo.ppEnabledExtensionNames = m_VulkanObject.pDevice->m_VulkanObject.Extensions.data();

			if (VulkanInstance::GetInstance()->GetDebug()) {
				CreateInfo.enabledLayerCount = VulkanInstance::GetInstance()->m_VulkanDebug.ValidationLayer->GetValidationSize();
				CreateInfo.ppEnabledLayerNames = VulkanInstance::GetInstance()->m_VulkanDebug.ValidationLayer->GetValidationData();
			}
			else CreateInfo.enabledLayerCount = 0;
		}

		VkResult result = vkCreateDevice(m_VulkanObject.pDevice->m_VulkanObject.Device, &CreateInfo, nullptr, &m_VulkanObject.lDevice);
		MORP_CORE_ASSERT(result, "Failed to create Logical Device!");

		vkGetDeviceQueue(m_VulkanObject.lDevice, Indices.GraphicsFamily.value(), 0, &m_VulkanObject.GraphicsQueue);
		vkGetDeviceQueue(m_VulkanObject.lDevice, Indices.PresentFamily.value(), 0, &m_VulkanObject.PresentQueue);
	}

}

