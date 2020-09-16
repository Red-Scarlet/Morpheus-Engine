#include "Morppch.h"
#include "VulkanSynchronization.h"

namespace Morpheus {

	VulkanSynchronization::VulkanSynchronization(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation)
		: m_VulkanCore({ _lDevice, _Presentation })
	{
		CreateSynchronization();
		MORP_CORE_WARN("[VULKAN] Synchronization Was Created!");
	}

	VulkanSynchronization::~VulkanSynchronization()
	{
		for (uint32 i = 0; i < m_TotalNumberOfFrames; i++) {
			vkDestroySemaphore(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.RenderSemaphores[i], nullptr);
			vkDestroySemaphore(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.ImageSemaphores[i], nullptr);
			vkDestroyFence(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.InFlightFences[i], nullptr);
		}
	}

	VkSemaphore* VulkanSynchronization::GetWait(const uint32& _Index)
	{
		return &m_VulkanObject.ImageSemaphores[_Index];
	}

	VkSemaphore* VulkanSynchronization::GetSignal(const uint32& _Index)
	{
		return &m_VulkanObject.RenderSemaphores[_Index];
	}

	void VulkanSynchronization::Begin(VulkanCommandBuffer* _CommandBuffer)
	{
		vkWaitForFences(m_VulkanCore.lDevice->GetDevice(), 1, &m_VulkanObject.InFlightFences[m_VulkanObject.CurrentFrame], 
			VK_TRUE, UINT64_MAX);

		vkAcquireNextImageKHR(m_VulkanCore.lDevice->GetDevice(), m_VulkanCore.Presentation->GetSwapchain(),
			UINT64_MAX, m_VulkanObject.ImageSemaphores[m_VulkanObject.CurrentFrame], VK_NULL_HANDLE, &m_VulkanObject.ImageIndex);

		if (m_VulkanObject.ImagesInFlight[m_VulkanObject.ImageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(m_VulkanCore.lDevice->GetDevice(), 1, &m_VulkanObject.ImagesInFlight[m_VulkanObject.ImageIndex],
				VK_TRUE, UINT64_MAX);
		m_VulkanObject.ImagesInFlight[m_VulkanObject.ImageIndex] = m_VulkanObject.InFlightFences[m_VulkanObject.CurrentFrame];

		VkSemaphore WaitSemaphores[] = { m_VulkanObject.ImageSemaphores[m_VulkanObject.CurrentFrame] };
		VkSemaphore SignalSemaphores[] = { m_VulkanObject.RenderSemaphores[m_VulkanObject.CurrentFrame] };
		VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo SubmitInfo {};
		{
			SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			SubmitInfo.waitSemaphoreCount = 1;
			SubmitInfo.pWaitSemaphores = WaitSemaphores;
			SubmitInfo.pWaitDstStageMask = WaitStages;
			SubmitInfo.signalSemaphoreCount = 1;
			SubmitInfo.pSignalSemaphores = SignalSemaphores;
			SubmitInfo.commandBufferCount = 1;
			SubmitInfo.pCommandBuffers = &_CommandBuffer->GetCommandBuffer(m_VulkanObject.ImageIndex);
		}

		vkResetFences(m_VulkanCore.lDevice->GetDevice(), 1, &m_VulkanObject.InFlightFences[m_VulkanObject.CurrentFrame]);

		VkResult result = vkQueueSubmit(m_VulkanCore.lDevice->GetGraphicsQueue(), 1,
			&SubmitInfo, m_VulkanObject.InFlightFences[m_VulkanObject.CurrentFrame]);
		MORP_CORE_ASSERT(result, "Failed to create Render Semaphore!");
	}

	void VulkanSynchronization::End(VulkanRenderpass* _Renderpass)
	{
		VkSubpassDependency Dependency{};
		{
			Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			Dependency.dstSubpass = 0;
			Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			Dependency.srcAccessMask = 0;
			Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}
		_Renderpass->GetInfo().dependencyCount = 1;
		_Renderpass->GetInfo().pDependencies = &Dependency;
	}

	void VulkanSynchronization::Flush()
	{
		VkSwapchainKHR SwapChains[] = { m_VulkanCore.Presentation->GetSwapchain() };
		VkSemaphore SignalSemaphores[] = { m_VulkanObject.RenderSemaphores[m_VulkanObject.CurrentFrame] };

		VkPresentInfoKHR PresentInfo {};
		{
			PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			PresentInfo.waitSemaphoreCount = 1;
			PresentInfo.pWaitSemaphores = SignalSemaphores;
			PresentInfo.swapchainCount = 1;
			PresentInfo.pSwapchains = SwapChains;
			PresentInfo.pImageIndices = &m_VulkanObject.ImageIndex;
		}

		vkQueuePresentKHR(m_VulkanCore.lDevice->GetPresentQueue(), &PresentInfo);
		//vkQueueWaitIdle(m_VulkanCore.lDevice->GetPresentQueue());
		m_VulkanObject.CurrentFrame = (m_VulkanObject.CurrentFrame + 1) % 2;
	}

	void VulkanSynchronization::CreateSynchronization()
	{
		m_VulkanObject.ImageSemaphores.resize(m_TotalNumberOfFrames);
		m_VulkanObject.RenderSemaphores.resize(m_TotalNumberOfFrames);

		m_VulkanObject.InFlightFences.resize(m_TotalNumberOfFrames);
		m_VulkanObject.ImagesInFlight.resize(m_VulkanCore.Presentation->GetSize(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo SemaphoreInfo {};
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo FenceInfo {};
		FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32 i = 0; i < m_TotalNumberOfFrames; i++) {
			VkResult result = vkCreateSemaphore(m_VulkanCore.lDevice->GetDevice(), &SemaphoreInfo, nullptr, &m_VulkanObject.ImageSemaphores[i]);
			MORP_CORE_ASSERT(result, "Failed to create Image Semaphore!");

			VkResult result_2 = vkCreateSemaphore(m_VulkanCore.lDevice->GetDevice(), &SemaphoreInfo, nullptr, &m_VulkanObject.RenderSemaphores[i]);
			MORP_CORE_ASSERT(result_2, "Failed to create Render Semaphore!");

			VkResult result_3 = vkCreateFence(m_VulkanCore.lDevice->GetDevice(), &FenceInfo, nullptr, &m_VulkanObject.InFlightFences[i]);
			MORP_CORE_ASSERT(result_3, "Failed to create Render Fence!");

		}
	}

}