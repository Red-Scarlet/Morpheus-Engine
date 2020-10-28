#include "Morppch.h"
#include "VulkanSynchronization.h"

#include "Platform/Vulkan/VulkanResource.h"

namespace Morpheus {

	VulkanSynchronization::VulkanSynchronization()
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_Swapchain = VulkanResourceCache::GetInstance()->Get<VulkanSwapchain>(VulkanResourceType::Swapchain);
		m_Command = VulkanResourceCache::GetInstance()->Get<VulkanCommand>(VulkanResourceType::CommandSystem);

		CreateSynchronization();
		MORP_CORE_WARN("[VULKAN] Synchronization Was Created!");
	}

	VulkanSynchronization::~VulkanSynchronization()
	{
		MORP_CORE_WARN("[VULKAN] Synchronization Was Destoryed!");
	}

	void VulkanSynchronization::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Device.destroySemaphore(m_PresentCompleteSemaphore);
		Device.destroySemaphore(m_RenderCompleteSemaphore);
		for (vk::Fence& f : m_WaitFences)
			Device.destroyFence(f);
	}

	bool VulkanSynchronization::Render()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();
		vk::SwapchainKHR Swapchain = m_Swapchain->GetSwapchain();

		vk::Result result = Device.acquireNextImageKHR(Swapchain, UINT64_MAX, m_PresentCompleteSemaphore, nullptr, &m_CurrentBuffer);
		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
			return true;

		if (result == vk::Result::eErrorDeviceLost)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Driver Lost, This is a crash!");

		// Wait for Fences
		Device.waitForFences(1, &m_WaitFences[m_CurrentBuffer], VK_TRUE, UINT64_MAX);
		Device.resetFences(1, &m_WaitFences[m_CurrentBuffer]);

		vk::CommandBuffer Buffer = m_Command->GetCommandBuffer(m_CurrentBuffer);

		vk::SubmitInfo submitInfo;
		vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		submitInfo
			.setWaitSemaphoreCount(1)
			.setPWaitSemaphores(&m_PresentCompleteSemaphore)
			.setPWaitDstStageMask(&waitDstStageMask)
			.setCommandBufferCount(1)
			.setPCommandBuffers(&Buffer)
			.setSignalSemaphoreCount(1)
			.setPSignalSemaphores(&m_RenderCompleteSemaphore);
		result = Queue.submit(1, &submitInfo, m_WaitFences[m_CurrentBuffer]);

		if (result == vk::Result::eErrorDeviceLost)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Driver Lost, This is a crash!");
		

		result = Queue.presentKHR(
			vk::PresentInfoKHR(
				1,
				&m_RenderCompleteSemaphore,
				1,
				&Swapchain,
				&m_CurrentBuffer,
				nullptr
			)
		);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
			return true;

		if (m_Command.use_count() == 0)
			MORP_CORE_ASSERT(MORP_ERROR, "COMMAND BUFFER USAGE IS ABNORMAL!!");

		return false;
	}

	void VulkanSynchronization::CreateSynchronization()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 SwapchainSize = m_Swapchain->GetSize();

		m_PresentCompleteSemaphore = Device.createSemaphore(vk::SemaphoreCreateInfo());
		m_RenderCompleteSemaphore = Device.createSemaphore(vk::SemaphoreCreateInfo());

		m_WaitFences.resize(SwapchainSize);
		for (uint32 i = 0; i < m_WaitFences.size(); i++)
			m_WaitFences[i] = Device.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
	}

	Ref<VulkanSynchronization> VulkanSynchronization::Create()
	{
		Ref<VulkanSynchronization> s_VulkanSynchronization = CreateRef<VulkanSynchronization>();
		VulkanResourceCache::GetInstance()->Submit<VulkanSynchronization>(VulkanResourceType::Synchronization, s_VulkanSynchronization);
		return s_VulkanSynchronization;
	}

}
