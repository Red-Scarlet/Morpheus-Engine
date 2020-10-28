#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

namespace Morpheus {

	class VulkanSynchronization
	{
	public:
		VulkanSynchronization();
		~VulkanSynchronization();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		bool Render();

	private:
		void CreateSynchronization();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanCommand> m_Command;

		vk::Semaphore m_PresentCompleteSemaphore;
		vk::Semaphore m_RenderCompleteSemaphore;
		Vector<vk::Fence> m_WaitFences;
		uint32 m_CurrentBuffer = 0;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanSynchronization> Create();
	};

}
