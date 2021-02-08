#include "Morppch.h"
#include "VulkanDevice.h"
#include "VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	VulkanDevice::VulkanDevice(const VkInstance& _Instance, const VkSurfaceKHR& _Surface)
		: m_Instance(_Instance), m_Surface(_Surface)
	{
		PickPhysicalDevice();
		MakeLogicalDevice();	
		VULKAN_CORE_WARN("[VULKAN] Device Was Created!");
	}

	VulkanDevice::~VulkanDevice()
	{
		VULKAN_CORE_WARN("[VULKAN] Device Was Destroyed!");
	}

	void VulkanDevice::CreateSwapchain()
	{
		if(m_Swapchain == nullptr)
			m_Swapchain = VulkanSwapchain::Create(m_lDevice, VulkanInstance::GetInstance()->GetSurface()->GetSurface(),
				m_SwapchainSupport, m_Indices);
		m_Swapchain->Reset();
	}

	uint32 VulkanDevice::FindMemoryType(const uint32& _TypeFilter, const VkMemoryPropertyFlags& _Properties)
	{
		MORP_PROFILE_FUNCTION();

		VkPhysicalDeviceMemoryProperties MemProperties;
		vkGetPhysicalDeviceMemoryProperties(m_pDevice, &MemProperties);

		uint32 Index = 0;
		for (uint32 i = 0; i < MemProperties.memoryTypeCount; i++)
			if ((_TypeFilter & (1 << i)) && (MemProperties.memoryTypes[i].propertyFlags & _Properties) == _Properties) {
				Index = i;
				VULKAN_CORE_TRACE("[VULKAN] Memory Index Found at " + ToString(Index));
				return Index;
			}

		VULKAN_CORE_ASSERT(true, "[VULKAN] Failed to find suitable memory type!");
	}

	VkFormat VulkanDevice::FindFormatType(const Vector<VkFormat>& _Formats, const VkImageTiling& _Tiling, const VkFormatFeatureFlags& _Features)
	{
		for (VkFormat format : _Formats) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_pDevice, format, &props);
			if (_Tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & _Features) == _Features) 
				return format;
			else if (_Tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & _Features) == _Features) 
				return format;
		}

		VULKAN_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find support format!");
	}

	const uint32& VulkanDevice::GetQueueIndices(const QueueType& _QueueType)
	{
		MORP_PROFILE_FUNCTION();

		switch (_QueueType)
		{
			case QueueType::VULKAN_QUEUE_PRESENT: return m_Indices.PresentFamily.value();
			case QueueType::VULKAN_QUEUE_GRAPHICS: return m_Indices.GraphicsFamily.value();
			case QueueType::VULKAN_QUEUE_COMPUTE: return m_Indices.TransferFamily.value();
			case QueueType::VULKAN_QUEUE_TRANSFER: return m_Indices.TransferFamily.value();
		}
	}

	const Ref<VulkanQueue> VulkanDevice::GetQueue(const QueueType& _QueueType)
	{
		MORP_PROFILE_FUNCTION();

		auto Iter = m_Queues.find(_QueueType);
		if (Iter != m_Queues.end())
			return Iter->second;
		return nullptr;
	}

	void VulkanDevice::PickPhysicalDevice()
	{
		MORP_PROFILE_FUNCTION();

		uint32 DeviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &DeviceCount, nullptr);
		VULKAN_CORE_ASSERT(DeviceCount == 0, "[VULKAN] Failed to find GPUs with Vulkan support!");

		Vector<VkPhysicalDevice> Devices(DeviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &DeviceCount, Devices.data());

		for (const auto& Device : Devices)
			if (IsDeviceSuitable(Device)) {
				m_pDevice = Device; break;
			}

		VULKAN_CORE_ASSERT(m_pDevice == VK_NULL_HANDLE, "[VULKAN] Failed to find a suitable GPU!");
	}

	bool VulkanDevice::IsDeviceSuitable(const VkPhysicalDevice& _Device)
	{
		MORP_PROFILE_FUNCTION();

		uint32 QueueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, nullptr);

		Vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_Device, &QueueFamilyCount, QueueFamilies.data());
		
		for (const auto& QueueFamily : QueueFamilies) {
			VULKAN_CORE_INFO("[VULKAN] Queue Number: " + ToString(QueueFamily.queueCount));
			String QueueSupported = "";
			if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				QueueSupported += " GRAPHICS ";
			if (QueueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
				QueueSupported += " COMPUTE ";
			if (QueueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
				QueueSupported += " TRANSFER ";
			if (QueueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
				QueueSupported += " SPARSE_BINDING ";
			if (QueueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT)
				QueueSupported += " PROTECTED ";
			VULKAN_CORE_INFO("[VULKAN] Queue Flags: " + QueueSupported);
		}

		uint32 Index = 0;
		for (const auto& QueueFamily : QueueFamilies) {
			if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				m_Indices.GraphicsFamily = Index;
			if (QueueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
				m_Indices.TransferFamily = Index;
			//if (QueueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
			//	m_Indices.ComputeFamily = Index;

			VkBool32 PresentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_Device, Index, m_Surface, &PresentSupport);
			if (PresentSupport)
				m_Indices.PresentFamily = Index;
			if (m_Indices.IsComplete())
				break;

			Index++;
		}

		uint32 ExtensionCount;
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionCount, nullptr);

		Vector<VkExtensionProperties> AvailableExtensions(ExtensionCount);
		vkEnumerateDeviceExtensionProperties(_Device, nullptr, &ExtensionCount, AvailableExtensions.data());

		#ifdef VULKAN_CORE_LOG_EXT
		for (VkExtensionProperties ext : AvailableExtensions)
			VULKAN_CORE_INFO("Found Device Extension: " + String(ext.extensionName));
		#endif

		std::set<String> RequiredExtensions(m_Extensions.begin(), m_Extensions.end());
		for (const auto& Extension : AvailableExtensions)
			RequiredExtensions.erase(Extension.extensionName);

		bool ExtensionsSupported = RequiredExtensions.empty();
		bool SwapChainAdequate = false;
		if (ExtensionsSupported) {
			SwapchainSupportDetails Details = {};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_Device, m_Surface, &Details.Capabilities);

			uint32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, m_Surface, &formatCount, nullptr);

			if (formatCount != 0) {
				Details.Formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(_Device, m_Surface, &formatCount, Details.Formats.data());
			}

			uint32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, m_Surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				Details.PresentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(_Device, m_Surface, &presentModeCount, Details.PresentModes.data());
			}

			m_SwapchainSupport = Details;
			SwapChainAdequate = !m_SwapchainSupport.Formats.empty() && !m_SwapchainSupport.PresentModes.empty();
		}

		return m_Indices.IsComplete() && ExtensionsSupported && SwapChainAdequate;
	}

	void VulkanDevice::MakeLogicalDevice()
	{
		MORP_PROFILE_FUNCTION();

		Vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
		Set<uint32> UniqueQueueFamilies = { m_Indices.GraphicsFamily.value(), m_Indices.PresentFamily.value(), m_Indices.TransferFamily.value() };

		float32 QueuePriority = 1.00f;
		for (uint32 QueueFamily : UniqueQueueFamilies) {
			VkDeviceQueueCreateInfo QueueCreateInfo{};
			QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueCreateInfo.queueFamilyIndex = QueueFamily;
			QueueCreateInfo.queueCount = 1;
			QueueCreateInfo.pQueuePriorities = &QueuePriority;
			QueueCreateInfos.push_back(QueueCreateInfo);
		}

		VkPhysicalDeviceFeatures DeviceFeatures = {};
		VkDeviceCreateInfo CreateInfo = {};
		{
			CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			CreateInfo.queueCreateInfoCount = QueueCreateInfos.size();
			CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
			CreateInfo.pEnabledFeatures = &DeviceFeatures;
			CreateInfo.enabledExtensionCount = m_Extensions.size();
			CreateInfo.ppEnabledExtensionNames = m_Extensions.data();
			CreateInfo.enabledLayerCount = 0;

			#ifdef MORP_DEBUG
			CreateInfo.enabledLayerCount = VulkanInstance::GetInstance()->GetValidationLayer()->GetValidationSize();
			CreateInfo.ppEnabledLayerNames = VulkanInstance::GetInstance()->GetValidationLayer()->GetValidationData();
			#endif
		}

		VkResult result = vkCreateDevice(m_pDevice, &CreateInfo, nullptr, &m_lDevice);
		VULKAN_CORE_ASSERT(result, "Failed to create Logical Device!");


		m_Queues[QueueType::VULKAN_QUEUE_GRAPHICS] = VulkanQueue::Create(m_lDevice, 0, m_Indices.GraphicsFamily.value());
		m_Queues[QueueType::VULKAN_QUEUE_PRESENT] = VulkanQueue::Create(m_lDevice, 0, m_Indices.PresentFamily.value());
		m_Queues[QueueType::VULKAN_QUEUE_TRANSFER] = VulkanQueue::Create(m_lDevice, 0, m_Indices.TransferFamily.value());
	}

	void VulkanDevice::DestroyLogicalDevice()
	{
		MORP_PROFILE_FUNCTION();

		VulkanSwapchain::Destroy(m_Swapchain);
		vkDeviceWaitIdle(m_lDevice);
		vkDestroyDevice(m_lDevice, nullptr);
	}

	Ref<VulkanDevice> VulkanDevice::Create(const VkInstance& _Instance, const VkSurfaceKHR& _Surface)
	{
		Ref<DeviceCache> s_Cache = VulkanCache<VulkanDevice>::Get(VULKAN_CACHE_DEVICE_TYPE);
		Ref<VulkanDevice> s_Device = CreateRef<VulkanDevice>(_Instance, _Surface);
		s_Cache->Add(s_Cache->GetCount(), s_Device);
		return s_Device;
	}

	void VulkanDevice::Destroy(const Ref<VulkanDevice>& _Device)
	{
		_Device->DestroyLogicalDevice();
	}

}}