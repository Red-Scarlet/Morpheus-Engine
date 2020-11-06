#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSynchronization.h"

#include <deque>

namespace Morpheus {

	class VulkanRenderQueue
	{
	public:
		VulkanRenderQueue();
		~VulkanRenderQueue();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		// These will take CommandBuffers.
		void Submit(const vk::CommandBuffer& _Commandbuffer0);

		void SetCapacityMode();	// Will allow for two CommandBUffers or one submittion time

		// And A sync
		void Flush(bool ClearBuffers);
		uint32 GetCurrentFrame() { return m_CurrentCMD; }

	private:
		Ref<VulkanSynchronization> m_Sync;
		//Vector<Vector<vk::CommandBuffer>> m_Queue;
		Vector<vk::CommandBuffer> m_Queue;

		uint32 m_ID = 0;

		uint32 m_CurrentCMD = 0;

	public:
		static Ref<VulkanRenderQueue> VulkanCreate();

	};

}