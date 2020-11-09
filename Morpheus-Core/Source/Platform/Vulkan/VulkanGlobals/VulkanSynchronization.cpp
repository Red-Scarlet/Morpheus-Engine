#include "Morppch.h"
#include "VulkanSynchronization.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanSynchronization::VulkanSynchronization()
		: VulkanGlobal(VulkanGlobalTypes::VulkanSynchronization)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanSynchronization));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Synchronization Was Created!");
	}

	VulkanSynchronization::~VulkanSynchronization()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Synchronization Was Destoryed!");
	}

	void VulkanSynchronization::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 SwapchainSize = m_Swapchain->GetSize();

		m_ImageSemaphores.resize(m_TotalNumberOfFrames);
		m_RenderSemaphores.resize(m_TotalNumberOfFrames);

		m_InFlightFences.resize(m_TotalNumberOfFrames);
		m_ImagesInFlight.resize(m_TotalNumberOfFrames, VK_NULL_HANDLE);

		VkSemaphoreCreateInfo SemaphoreInfo{};
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo FenceInfo{};
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32 i = 0; i < m_TotalNumberOfFrames; i++) {
			VkResult result = vkCreateSemaphore(Device, &SemaphoreInfo, nullptr, &m_ImageSemaphores[i]);
			MORP_CORE_ASSERT(result, "Failed to create Image Semaphore!");

			VkResult result_2 = vkCreateSemaphore(Device, &SemaphoreInfo, nullptr, &m_RenderSemaphores[i]);
			MORP_CORE_ASSERT(result_2, "Failed to create Render Semaphore!");

			VkResult result_3 = vkCreateFence(Device, &FenceInfo, nullptr, &m_InFlightFences[i]);
			MORP_CORE_ASSERT(result_3, "Failed to create Render Fence!");
		}
	}

	void VulkanSynchronization::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		for (uint32 i = 0; i < m_TotalNumberOfFrames; i++) {
			vkDestroySemaphore(Device, m_RenderSemaphores[i], nullptr);
			vkDestroySemaphore(Device, m_ImageSemaphores[i], nullptr);
			vkDestroyFence(Device, m_InFlightFences[i], nullptr);
		}
	}

	bool VulkanSynchronization::Render()
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
			SubmitInfo.commandBufferCount = m_Commands.size();
			SubmitInfo.pCommandBuffers = m_Commands.data();
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
		m_Commands.clear();

		return false;
	}

	void VulkanSynchronization::Append(const vk::CommandBuffer& _Command)
	{
		m_Commands.push_back(_Command);
	}


	Ref<VulkanSynchronization> VulkanSynchronization::Make()
	{
		Ref<VulkanSynchronization> s_VulkanSynchronization = CreateRef<VulkanSynchronization>();
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit(s_VulkanSynchronization);
		return s_VulkanSynchronization;
	}

}
