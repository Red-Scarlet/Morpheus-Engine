#include "Morppch.h"
#include "VulkanQueue.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanQueue::VulkanQueue()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Queue Was Created!");
	}

	VulkanQueue::~VulkanQueue()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Queue Was Destoryed!");
	}

	void VulkanQueue::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 SwapchainSize = m_Swapchain->GetSize();

		m_ImageSemaphores.resize(SwapchainSize);
		m_RenderSemaphores.resize(SwapchainSize);

		m_InFlightFences.resize(SwapchainSize);
		m_ImagesInFlight.resize(SwapchainSize, VK_NULL_HANDLE);

		VkSemaphoreCreateInfo SemaphoreInfo{};
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo FenceInfo{};
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32 i = 0; i < SwapchainSize; i++) {
			VkResult result = vkCreateSemaphore(Device, &SemaphoreInfo, nullptr, &m_ImageSemaphores[i]);
			MORP_CORE_ASSERT(result, "Failed to create Image Semaphore!");

			VkResult result_2 = vkCreateSemaphore(Device, &SemaphoreInfo, nullptr, &m_RenderSemaphores[i]);
			MORP_CORE_ASSERT(result_2, "Failed to create Render Semaphore!");

			VkResult result_3 = vkCreateFence(Device, &FenceInfo, nullptr, &m_InFlightFences[i]);
			MORP_CORE_ASSERT(result_3, "Failed to create Render Fence!");
		}
	}

	void VulkanQueue::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 SwapchainSize = m_Swapchain->GetSize();

		for (uint32 i = 0; i < SwapchainSize; i++) {
			vkDestroySemaphore(Device, m_RenderSemaphores[i], nullptr);
			vkDestroySemaphore(Device, m_ImageSemaphores[i], nullptr);
			vkDestroyFence(Device, m_InFlightFences[i], nullptr);
		}
	}

	Boolean VulkanQueue::Flush()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();
		vk::SwapchainKHR Swapchain = m_Swapchain->GetSwapchain();

		vkWaitForFences(Device, 1, &m_InFlightFences[m_CurrentFrame],
			VK_TRUE, UINT64_MAX);

		vkAcquireNextImageKHR(Device, Swapchain,
			UINT64_MAX, m_ImageSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &m_ImageIndex);

		if (m_ImagesInFlight[m_ImageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(Device, 1, &m_ImagesInFlight[m_ImageIndex],
				VK_TRUE, UINT64_MAX);
		m_ImagesInFlight[m_ImageIndex] = m_InFlightFences[m_CurrentFrame];

		VkSemaphore WaitSemaphores[] = { m_ImageSemaphores[m_CurrentFrame] };
		VkSemaphore SignalSemaphores[] = { m_RenderSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo SubmitInfo{};
		{
			SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			SubmitInfo.waitSemaphoreCount = 1;
			SubmitInfo.pWaitSemaphores = WaitSemaphores;
			SubmitInfo.pWaitDstStageMask = WaitStages;
			SubmitInfo.signalSemaphoreCount = 1;
			SubmitInfo.pSignalSemaphores = SignalSemaphores;
			SubmitInfo.commandBufferCount = m_Queue.size();
			SubmitInfo.pCommandBuffers = m_Queue.data();
		}

		vkResetFences(Device, 1, &m_InFlightFences[m_CurrentFrame]);

		VkResult result = vkQueueSubmit(Queue, 1,
			&SubmitInfo, m_InFlightFences[m_CurrentFrame]);
		MORP_CORE_ASSERT(result, "Failed to create Render Semaphore!");

		VkSwapchainKHR SwapChains[] = { Swapchain };

		VkPresentInfoKHR PresentInfo{};
		{
			PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			PresentInfo.waitSemaphoreCount = 1;
			PresentInfo.pWaitSemaphores = SignalSemaphores;
			PresentInfo.swapchainCount = 1;
			PresentInfo.pSwapchains = SwapChains;
			PresentInfo.pImageIndices = &m_ImageIndex;
		}

		vkQueuePresentKHR(Queue, &PresentInfo);
		m_CurrentFrame = (m_CurrentFrame + 1) % 2;

		m_Queue.clear();
		return false;
	}

	void VulkanQueue::Submit(const VkCommandBuffer& _CommmandBuffer, const QueueCommandFlags& _Flags)
	{
		m_Queue.push_back(_CommmandBuffer);
	}

	Ref<VulkanQueue> VulkanQueue::Make()
	{
		Ref<VulkanQueue> s_VulkanQueue = CreateRef<VulkanQueue>();
		VulkanMemoryManager::GetInstance()->SetVulkanQueue(s_VulkanQueue);
		return s_VulkanQueue;
	}

}

