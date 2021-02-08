#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

namespace Morpheus { namespace Vulkan {

	// TODO: Add the ability to reissue commandbuffer, however reset them before.
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(const uint32& _PrimaryBuffer, const uint32& _SecondaryBuffer);
		virtual ~VulkanCommandPool();
		const VkCommandBuffer& GetCommandBuffer(const bool& _SecondaryBuffer);

	private:
		void VulkanDestroy();

		void Expand();
		void AllocatePrimary();
		void AllocateSecondary();
		void DeallocatePrimary(const uint32& _Index);
		void DeallocateSecondary(const uint32& _Index);
		uint32 CheckBufferAvailability(const bool& _SecondaryBuffer);

	private:
		Ref<VulkanDevice> m_Device;
		uint32 m_PrimaryMax, m_SecondaryMax;
		uint32 m_PrimaryCount, m_SecondaryCount;
		Vector<VkCommandPool> m_Pools;
		Vector<Tuple<VkCommandBuffer, bool>> m_PrimaryBuffers;
		Vector<Tuple<VkCommandBuffer, bool>> m_SecondaryBuffers;

	public:
		static Ref<VulkanCommandPool> Create(const uint32& _PrimaryBuffer = 32, const uint32& _SecondaryBuffer = 64);
		static void Destroy(const Ref<VulkanCommandPool>& _Pool);
	};

}}