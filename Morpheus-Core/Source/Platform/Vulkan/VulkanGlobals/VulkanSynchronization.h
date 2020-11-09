#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "VulkanGlobal.h"

namespace Morpheus {

	class VulkanSynchronization: public VulkanGlobal
	{
	public:
		VulkanSynchronization();
		virtual ~VulkanSynchronization();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory()  override;

	public:
		bool Render();
		void Append(const vk::CommandBuffer& _Command);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Vector<VkCommandBuffer> m_Commands;

		vk::Semaphore m_PresentCompleteSemaphore;
		vk::Semaphore m_RenderCompleteSemaphore;
		Vector<vk::Fence> m_WaitFences;
		uint32 m_CurrentBuffer;

		Vector<VkSemaphore> m_ImageSemaphores;
		Vector<VkSemaphore> m_RenderSemaphores;
		Vector<VkFence> m_InFlightFences;
		Vector<VkFence> m_ImagesInFlight;

		uint32 m_CurrentFrame = 0;
		uint32 m_ImageIndex = 0;

		const uint32 m_TotalNumberOfFrames = 2;

	public:
		static Ref<VulkanSynchronization> Make();
	};

}
