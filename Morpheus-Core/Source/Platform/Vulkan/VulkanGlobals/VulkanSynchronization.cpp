#include "Morppch.h"
#include "VulkanSynchronization.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanSynchronization::VulkanSynchronization()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		//m_Command = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanCommand>(VulkanResourceTypes::VulkanCommandBuffer);
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanSwapchain));

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

	bool VulkanSynchronization::Next()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::SwapchainKHR Swapchain = m_Swapchain->GetSwapchain();

		vk::Result result = Device.acquireNextImageKHR(Swapchain, UINT64_MAX, m_PresentCompleteSemaphore, nullptr, &m_CurrentBuffer);
		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
			return true;

		if (result == vk::Result::eErrorDeviceLost)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Driver Lost, This is a crash!");

		return false;
	}

	bool VulkanSynchronization::Render()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();
		vk::SwapchainKHR Swapchain = m_Swapchain->GetSwapchain();

		// Wait for Fences
		Device.waitForFences(1, &m_WaitFences[m_CurrentBuffer], VK_TRUE, UINT64_MAX);
		Device.resetFences(1, &m_WaitFences[m_CurrentBuffer]);

		//MORP_CORE_TRACE("[QUEUE] FLUSH MADE CURRENT: " + std::to_string(m_CurrentBuffer) + "!");

		vk::SubmitInfo submitInfo;
		vk::PipelineStageFlags waitDstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		submitInfo
			.setWaitSemaphoreCount(1)
			.setPWaitSemaphores(&m_PresentCompleteSemaphore)
			.setPWaitDstStageMask(&waitDstStageMask)
			.setCommandBufferCount(m_Commands.size())
			.setPCommandBuffers(m_Commands.data())
			.setSignalSemaphoreCount(1)
			.setPSignalSemaphores(&m_RenderCompleteSemaphore);
		vk::Result result = Queue.submit(1, &submitInfo, m_WaitFences[m_CurrentBuffer]);

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

		m_Commands.clear();

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
			return true;

		return false;
	}

	void VulkanSynchronization::Append(const vk::CommandBuffer& _Command)
	{
		m_Commands.push_back(_Command);
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
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit<VulkanSynchronization>(VulkanGlobalTypes::VulkanSynchronization, s_VulkanSynchronization);
		return s_VulkanSynchronization;
	}

}
