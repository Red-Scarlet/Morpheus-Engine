#include "Morppch.h"
#include "VulkanRenderQueue.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanRenderQueue::VulkanRenderQueue()
	{
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanRenderQueue));
		m_Sync = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSynchronization>(VulkanGlobalTypes::VulkanSynchronization);
	}

	VulkanRenderQueue::~VulkanRenderQueue()
	{
	}

	void VulkanRenderQueue::Submit(const vk::CommandBuffer& _Commandbuffer0)
	{
		m_Queue.push_back(_Commandbuffer0);
	}

	void VulkanRenderQueue::SetCapacityMode()
	{
	}

	void VulkanRenderQueue::Flush(bool ClearBuffers)
	{
		//MORP_CORE_WARN("[QUEUE] FLUSH MADE SIZE: " + std::to_string(m_Queue.size()) + "!");

		if(!m_Queue.empty()) {
			m_Sync->Append(m_Queue[0]);
			m_Sync->Append(m_Queue[1]);
			m_Sync->Render();
			m_CurrentCMD++;
		}

		m_Queue.clear();

		if(m_CurrentCMD == 2) {
			//if (ClearBuffers)
			//m_Queue.clear();
			m_CurrentCMD = 0;
		}
	}

	Ref<VulkanRenderQueue> VulkanRenderQueue::VulkanCreate()
	{
		Ref<VulkanRenderQueue> s_VulkanRenderQueue = CreateRef<VulkanRenderQueue>();
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit<VulkanRenderQueue>(VulkanGlobalTypes::VulkanRenderQueue, s_VulkanRenderQueue);
		return s_VulkanRenderQueue;
	}

}

