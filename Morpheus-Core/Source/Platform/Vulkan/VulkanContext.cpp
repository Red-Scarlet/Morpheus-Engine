// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"
#include "VulkanContext.h"

namespace Morpheus {

	VulkanContext::VulkanContext()
	{
		m_VulkanInstance = VulkanInstance::GetInstance();
		m_VulkanSurface = new VulkanSurface(m_VulkanInstance);
		m_VulkanDevice = new VulkanDevice(m_VulkanInstance, m_VulkanSurface);
		m_VulkanSwapchain = new VulkanSwapchain(m_VulkanInstance, m_VulkanDevice, m_VulkanSurface);
		m_VulkanImageview = new VulkanImageview(m_VulkanInstance, m_VulkanDevice, m_VulkanSwapchain);
		m_VulkanRenderpass = new VulkanRenderpass(m_VulkanInstance, m_VulkanDevice, m_VulkanSwapchain);
		m_VulkanGraphicsPipeline = new VulkanGraphicsPipeline(m_VulkanInstance, m_VulkanDevice, m_VulkanSwapchain, m_VulkanRenderpass);
		m_VulkanFramebuffer = new VulkanFramebuffer(m_VulkanInstance, m_VulkanDevice, m_VulkanImageview, m_VulkanRenderpass);
		m_VulkanCommandPool = new VulkanCommandPool(m_VulkanInstance, m_VulkanDevice);
			
		m_VulkanCommandBuffer = new VulkanCommandBuffer(m_VulkanInstance, m_VulkanDevice, m_VulkanFramebuffer, m_VulkanCommandPool,
			m_VulkanSwapchain, m_VulkanRenderpass, m_VulkanGraphicsPipeline);

		SetupSemaphores();
	}

	VulkanContext::~VulkanContext()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(m_VulkanDevice->GetLogicalDevice(), m_RenderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(m_VulkanDevice->GetLogicalDevice(), m_ImageAvailableSemaphores[i], nullptr);
		}
		delete m_VulkanCommandBuffer;
		delete m_VulkanCommandPool;
		delete m_VulkanFramebuffer;
		delete m_VulkanGraphicsPipeline;
		delete m_VulkanRenderpass;
		delete m_VulkanImageview;
		delete m_VulkanSwapchain;
		delete m_VulkanDevice;
		delete m_VulkanSurface;
	}

	void VulkanContext::Draw()
	{
		UINT32 imageIndex;
		size_t currentFrame = 0;
		vkAcquireNextImageKHR(m_VulkanDevice->GetLogicalDevice(), m_VulkanSwapchain->GetSwapchain(),
			UINT64_MAX, m_ImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);


		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		auto& cm = m_VulkanCommandBuffer->GetCommandBuffer();
		submitInfo.pCommandBuffers = &cm[imageIndex];

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkResult result = vkQueueSubmit(m_VulkanDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		MORP_CORE_ASSERT(result, "Failed to create Render Semaphore!");

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;

		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		m_VulkanRenderpass->GetRenderpassInfo().dependencyCount = 1;
		m_VulkanRenderpass->GetRenderpassInfo().pDependencies = &dependency;

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_VulkanSwapchain->GetSwapchain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional

		vkQueuePresentKHR(m_VulkanDevice->GetPresentQueue(), &presentInfo);

		//D
		vkQueueWaitIdle(m_VulkanDevice->GetPresentQueue());
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanContext::Stop()
	{
		vkDeviceWaitIdle(m_VulkanDevice->GetLogicalDevice());
	}

	void VulkanContext::SetupSemaphores()
	{
		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{ };
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VkResult result = vkCreateSemaphore(m_VulkanDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]);
			MORP_CORE_ASSERT(result, "Failed to create Image Semaphore!");

			VkResult result_2 = vkCreateSemaphore(m_VulkanDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]);
			MORP_CORE_ASSERT(result_2, "Failed to create Render Semaphore!");
		}
	}

}