#pragma once

#include "Morpheus/Core/Common.h"

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

namespace Morpheus {

	class VulkanCommand
	{
	public:
		VulkanCommand();
		~VulkanCommand();
		void Destory();
		void FreeBuffers();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		void SetupCommands();

		const vk::CommandBuffer& GetCommandBuffer(const uint32& _Index) 
		{ return m_CommandBuffers.at(_Index); }
		const vk::CommandPool& GetPool() 
		{ return m_CommandPool; }

	private:
		void CreateCommandPool();
		void CreateCommandBuffers();

	private:
		vk::CommandPool m_CommandPool;
		Vector<vk::CommandBuffer> m_CommandBuffers;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanCommand> Create();

	};

}