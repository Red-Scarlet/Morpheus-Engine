#include "Morppch.h"
#include "VulkanContext.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanContext::VulkanContext()
	{
	}

	void VulkanContext::Init()
	{
		m_Instance = VulkanInstance::Create();
		m_Device = VulkanDevice::VulkanCreate(m_Instance);
		m_Swapchain = VulkanSwapchain::Create();

		m_Sync = VulkanSynchronization::Create();
		m_RenderQueue = VulkanRenderQueue::VulkanCreate();
	}

	void VulkanContext::Destory()
	{
		m_Device->Wait();

		m_Sync->Destory();
		m_Swapchain->Destory();
		m_Device->Destory();
		m_Instance->Destroy();
	}

	void VulkanContext::Render()
	{
		m_RenderQueue->Flush(false);
		m_Sync->Next();
	}

}