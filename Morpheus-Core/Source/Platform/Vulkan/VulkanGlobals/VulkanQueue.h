#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

namespace Morpheus {

	enum class QueueCommandFlags : uint8
	{ None = 0, DeleteCommand, KeepRecycle };

	class VulkanQueue
	{
	public:
		VulkanQueue();
		virtual ~VulkanQueue();

	private:
		void VulkanCreate();
		void VulkanDestory();

	public:
		Boolean Flush();
		void Submit(const VkCommandBuffer& _CommmandBuffer, const QueueCommandFlags& _Flags);

		const uint32& GetCurrentFrame() const { return m_CurrentFrame; }
		const uint32& GetBufferCount() { return m_Swapchain->GetSize(); }

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		
		Vector<VkSemaphore> m_ImageSemaphores;
		Vector<VkSemaphore> m_RenderSemaphores;
		Vector<VkFence> m_InFlightFences;
		Vector<VkFence> m_ImagesInFlight;

		uint32 m_CurrentFrame = 0;
		uint32 m_ImageIndex = 0;

		Vector<VkCommandBuffer> m_Queue;

	public:
		static Ref<VulkanQueue> Make();

	};

}