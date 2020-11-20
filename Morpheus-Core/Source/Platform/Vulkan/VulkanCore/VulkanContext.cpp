#include "Morppch.h"
#include "VulkanContext.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanContext::VulkanContext()
	{
	}

	void VulkanContext::Init()
	{
		m_Instance = VulkanInstance::Make();

		m_Device = VulkanDevice::Make();
		m_Swapchain = VulkanSwapchain::Make();

		m_CommandSystem = VulkanCommandSystem::Make();
		m_Queue = VulkanQueue::Make();
	}

	void VulkanContext::Destory()
	{
		m_Device->Wait();
	}

	void VulkanContext::End()
	{
		m_Queue->Flush();
	}

}