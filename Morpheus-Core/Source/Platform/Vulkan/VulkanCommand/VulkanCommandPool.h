#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

namespace Morpheus { namespace Vulkan {

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(const Ref<VulkanDevice>& _Device, const uint32& _Max, const bool& _IsSecondary);
		~VulkanCommandPool();

		const VkCommandBuffer& GetCommandBuffer();

	private:
		void Expand();
		void Allocate();
		void Deallocate(const uint32& _Index);
		uint32 CheckBufferAvailability();

	private:
		Ref<VulkanDevice> m_Device;
		uint32 m_AllocationMax;
		uint32 m_AllocationCount;
		bool m_IsSecondary;

		Vector<VkCommandPool> m_Pools;
		Vector<Tuple<VkCommandBuffer, uint32, bool>> m_CommandBuffers;

	public:
		static Ref<VulkanCommandPool> Create(const Ref<VulkanDevice>& _Device,
			const uint32& _Max = 2, const bool& _IsSecondaryBuffers = false);
	};

}}