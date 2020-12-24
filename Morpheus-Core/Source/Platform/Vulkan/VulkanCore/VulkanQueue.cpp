#include "Morppch.h"
#include "VulkanQueue.h"

namespace Morpheus { namespace Vulkan {

	VulkanQueue::VulkanQueue(const VkDevice& _Device, const uint32& _QueueIndex, const uint32& _QueueFamily)
		: m_Device(_Device), m_QueueIndex(_QueueIndex), m_QueueFamily(_QueueFamily)
	{
		CreateQueue();
		VULKAN_CORE_WARN("[VULKAN] Queue was Created!");
	}

	VulkanQueue::~VulkanQueue()
	{
		VULKAN_CORE_WARN("[VULKAN] Queue was Destroyed!");
	}

	const VkQueue& VulkanQueue::GetQueue() const
	{
		MORP_PROFILE_FUNCTION();
		return m_Queue;
	}

	void VulkanQueue::Submit(const VkSubmitInfo* _SubmitInfo, const VkFence& _Fence, const uint32& _Count)
	{
		MORP_PROFILE_FUNCTION();
	
		m_LockMutex.lock();
		vkQueueSubmit(m_Queue, _Count, _SubmitInfo, _Fence);
		m_LockMutex.unlock();
	}

	void VulkanQueue::Present(const VkPresentInfoKHR* _PresentInto)
	{
		MORP_PROFILE_FUNCTION();
		vkQueuePresentKHR(m_Queue, _PresentInto);
	}

	void VulkanQueue::Wait()
	{
		MORP_PROFILE_FUNCTION();
		vkQueueWaitIdle(m_Queue);
	}

	void VulkanQueue::CreateQueue()
	{
		MORP_PROFILE_FUNCTION();
		vkGetDeviceQueue(m_Device, m_QueueFamily, m_QueueIndex, &m_Queue);
	}

	Ref<VulkanQueue> VulkanQueue::Create(const VkDevice& _Device, const uint32& _QueueIndex, const uint32& _QueueFamily)
	{
		return CreateRef<VulkanQueue>(_Device, _QueueIndex, _QueueFamily);
	}

}}