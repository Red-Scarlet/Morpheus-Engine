#include "Morppch.h"
#include "VulkanQueue.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanQueue::VulkanQueue()
		: VulkanGlobal(VulkanGlobalTypes::VulkanQueue)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Queue Was Created!");
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanQueue));
	}

	VulkanQueue::~VulkanQueue()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Queue Was Destoryed!");
	}

	void VulkanQueue::VulkanCreate()
	{
		m_Sync = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSynchronization>(VulkanGlobalTypes::VulkanSynchronization);
	}

	void VulkanQueue::VulkanDestory()
	{
	}

	void VulkanQueue::Flush()
	{
		if (!m_Queue.empty()) {
			for (uint32 i = 0; i < m_Queue.size(); i++)
				m_Sync->Append(m_Queue[i]);
			m_Sync->Render();
			m_CurrentIndex++;
		}

		m_Queue.clear();

		if (m_CurrentIndex == 2) 
			m_CurrentIndex = 0;
	}

	void VulkanQueue::Submit(const vk::CommandBuffer& _Commandbuffer)
	{
		m_Queue.push_back(_Commandbuffer);
	}

	Ref<VulkanQueue> VulkanQueue::Make()
	{
		Ref<VulkanQueue> s_VulkanQueue = CreateRef<VulkanQueue>();
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit(s_VulkanQueue);
		return s_VulkanQueue;
	}

}

