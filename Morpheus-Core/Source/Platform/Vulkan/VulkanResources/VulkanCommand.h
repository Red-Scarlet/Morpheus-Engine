#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

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

		const bool& GetRecompilationState() { return m_RecompilationRequired; }
		void SetRecompilationState(const bool& _State) { m_RecompilationRequired = _State; }

		bool SetupCommands(const Vector4& ClearColor);

		const vk::CommandBuffer& GetCommandBuffer(const uint32& _Index) 
		{ return m_CommandBuffers.at(_Index); }
		const vk::CommandPool& GetPool() 
		{ return m_CommandPool; }

	private:
		void CreateCommandPool();
		void CreateCommandBuffers();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		vk::CommandPool m_CommandPool;
		Vector<vk::CommandBuffer> m_CommandBuffers;

		uint32 m_ID = 0;
		bool m_RecompilationRequired = true;

	public:
		static Ref<VulkanCommand> Create();

	};

}