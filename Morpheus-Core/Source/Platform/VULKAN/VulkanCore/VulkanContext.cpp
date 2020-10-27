#include "Morppch.h"
#include "VulkanContext.h"

#include "VulkanResource.h"

namespace Morpheus {

	VulkanContext::VulkanContext()
	{
	}

	void VulkanContext::Init()
	{
		m_Instance = VulkanInstance::Create();
		m_Device = VulkanDevice::Create(m_Instance);
		VulkanResourceCache::GetInstance()->Submit<VulkanDevice>(VulkanResourceType::Device, m_Device);

		m_Swapchain = VulkanSwapchain::Create();
	}

	void VulkanContext::Destory()
	{
		m_Device->Wait();

		m_Swapchain->Destory();
		m_Device->Destory();
		m_Instance->Destroy();
	}

}