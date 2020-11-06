#include "Morppch.h"
#include "VulkanDevice.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanDevice::VulkanDevice(const Ref<VulkanInstance>& _Instance)
		: m_Instance(_Instance)
	{
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanDevice));

		CreateDevice();
		MORP_CORE_WARN("[VULKAN] Device Was Created!");
	}
	
	VulkanDevice::~VulkanDevice()
	{
		MORP_CORE_WARN("[VULKAN] Device Was Destoryed!");
	}

	void VulkanDevice::Wait()
	{
		m_LogicalDevice.waitIdle();
	}

	void VulkanDevice::Destory()
	{
		m_LogicalDevice.destroy();
		m_Instance->GetInstance().destroySurfaceKHR(m_Surface->GetStruct().Surface);
	}

	const uint32& VulkanDevice::GetQueueIndex(vk::PhysicalDevice& _PhysicalDevice, vk::QueueFlagBits _Flags)
	{
		Vector<vk::QueueFamilyProperties> queueProps = m_PhysicalDevice.getQueueFamilyProperties();
		for (uint32 i = 0; i < queueProps.size(); ++i)
			if (queueProps[i].queueFlags & _Flags)
				return i;
		return 0;
	}

	Vector<Extention> VulkanDevice::GetExtensions(const Vector<vk::ExtensionProperties>& _Installed, const Vector<Extention>& _Wanted)
	{
		Vector<Extention> Out;
		for (Extention const& w : _Wanted)
			for (vk::ExtensionProperties const& i : _Installed) {
				String s = i.extensionName;
				if (s.compare(w) == 0) {
					Out.push_back(w);
					break;
				}
			}
		return Out;
	}

	void VulkanDevice::CreateDevice()
	{
		Vector<vk::PhysicalDevice> physicalDevices = m_Instance->GetInstance().enumeratePhysicalDevices();
		m_PhysicalDevice = physicalDevices[0];
		m_QueueFamilyIndex = GetQueueIndex(m_PhysicalDevice, vk::QueueFlagBits::eGraphics);

		// Create Surface
		m_Surface = VulkanSurface::Create(m_Instance->GetInstance(), m_PhysicalDevice, m_QueueFamilyIndex);

		vk::DeviceQueueCreateInfo QueueCreateInfo;
		QueueCreateInfo.setQueueFamilyIndex(m_QueueFamilyIndex);
		QueueCreateInfo.setQueueCount(1);
		float QueuePriority = 0.5f;
		QueueCreateInfo.setPQueuePriorities(&QueuePriority);

		Vector<vk::ExtensionProperties> installedDeviceExtensions = m_PhysicalDevice.enumerateDeviceExtensionProperties();
		Vector<Extention> wantedDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		Vector<Extention> DeviceExtensions = GetExtensions(installedDeviceExtensions, wantedDeviceExtensions);

		vk::DeviceCreateInfo DeviceCreateInfo;
		DeviceCreateInfo.setPQueueCreateInfos(&QueueCreateInfo);
		DeviceCreateInfo.setQueueCreateInfoCount(1);
		DeviceCreateInfo.setPpEnabledExtensionNames(DeviceExtensions.data());
		DeviceCreateInfo.setEnabledExtensionCount((uint32)DeviceExtensions.size());
		m_LogicalDevice = m_PhysicalDevice.createDevice(DeviceCreateInfo);

		m_Queue = m_LogicalDevice.getQueue(m_QueueFamilyIndex, 0);

		m_Surface->SetupColorFormats();
	}

	Ref<VulkanDevice> VulkanDevice::VulkanCreate(const Ref<VulkanInstance>& _Instance)
	{
		Ref<VulkanDevice> s_VulkanDevice = CreateRef<VulkanDevice>(_Instance);
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit<VulkanDevice>(VulkanGlobalTypes::VulkanDevice, s_VulkanDevice);
		return s_VulkanDevice;
	}

}