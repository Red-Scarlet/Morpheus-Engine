#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSynchronization.h"

#include "VulkanGlobal.h"

namespace Morpheus {

	class VulkanQueue : public VulkanGlobal
	{
	public:
		VulkanQueue();
		virtual ~VulkanQueue();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		void Flush();

		void Submit(const vk::CommandBuffer& _Commandbuffer);

		const uint32& GetCurrentFrame() 
		{ return m_CurrentIndex; }

		const uint32& GetBufferCount() { return m_Swapchain->GetSize(); }

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanSynchronization> m_Sync;
		Vector<vk::CommandBuffer> m_Queue;
		uint32 m_CurrentIndex = 0;

	public:
		static Ref<VulkanQueue> Make();

	};

}