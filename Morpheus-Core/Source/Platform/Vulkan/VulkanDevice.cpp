// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"
#include "VulkanDevice.h"

#include <set>
#include <cstdint>
#include <algorithm>

namespace Morpheus{

	VulkanDevice::VulkanDevice(VulkanInstance* Instance, VulkanSurface* Surface)
        : m_VulkanInstance(Instance), m_Surface(Surface)
	{
        PickPhysicalDevice();
        CreateLogicalDevice();
        MORP_CORE_WARN("[VULKAN] Vulkan Device Was Created!");
	}

	VulkanDevice::~VulkanDevice()
	{        
        vkDestroyDevice(m_LogicalDevice, nullptr);
	}

	void VulkanDevice::PickPhysicalDevice()
	{
        UINT32 deviceCount = 0;
        auto instance = m_VulkanInstance->GetVulkan();
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
            MORP_CORE_ASSERT(true, "Failed to find GPU(s) with Vulkan support!");

        Vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices)
            if (IsDeviceSuitable(device)) {
                m_PhysicalDevice = device; break;
            }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
            MORP_CORE_ASSERT(true, "Failed to find a suitable GPU!");
	}

    bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice _Device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(_Device);

        bool extensionsSupported = CheckDeviceExtensionSupport(_Device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_Device);
            swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
        }

        return indices.IsComplete() && extensionsSupported && swapChainAdequate;
    }

    bool VulkanDevice::CheckDeviceExtensionSupport(VkPhysicalDevice _Device)
    {
        UINT32 extensionCount;
        vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionCount, nullptr);

        Vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(_Device, nullptr, &extensionCount, availableExtensions.data());

        std::set<String> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

        for (const auto& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);
        return requiredExtensions.empty();
    }

    QueueFamilyIndices VulkanDevice::FindQueueFamilies(VkPhysicalDevice _Device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(_Device, &queueFamilyCount, queueFamilies.data());

        UINT32 i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.GraphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(_Device, i, m_Surface->GetSurface(), &presentSupport);

            if (presentSupport)
                indices.PresentFamily = i;
            if (indices.IsComplete())
                break;
            i++;
        }

        return indices;
    }

    void VulkanDevice::CreateLogicalDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<UINT32> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

        float queuePriority = 1.00f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures {};

        VkDeviceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<UINT32>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<UINT32>(m_DeviceExtensions.size());
        createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

        if (m_VulkanInstance->GetValidationState()) {
            createInfo.enabledLayerCount = static_cast<UINT32>(m_VulkanInstance->GetValidationSize());
            createInfo.ppEnabledLayerNames = m_VulkanInstance->GetValidationData();
        }
        else createInfo.enabledLayerCount = 0;

        VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice);
        MORP_CORE_ASSERT(result, "Failed to create Logical Device!");

        vkGetDeviceQueue(m_LogicalDevice, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_LogicalDevice, indices.PresentFamily.value(), 0, &m_PresentQueue);
    }

    SwapChainSupportDetails VulkanDevice::QuerySwapChainSupport(VkPhysicalDevice Device)
    {
        SwapChainSupportDetails details;
    
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, m_Surface->GetSurface(), &details.Capabilities);
    
        UINT32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(Device, m_Surface->GetSurface(), &formatCount, nullptr);
    
        if (formatCount != 0) {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(Device, m_Surface->GetSurface(), &formatCount, details.Formats.data());
        }
    
        UINT32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(Device, m_Surface->GetSurface(), &presentModeCount, nullptr);
    
        if (presentModeCount != 0) {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(Device, m_Surface->GetSurface(), &presentModeCount, details.PresentModes.data());
        }
    
        return details;
    }

}