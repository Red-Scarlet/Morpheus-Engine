#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

namespace Morpheus {

	class VulkanCommandAllocator
	{
	public:
		VulkanCommandAllocator();
		virtual ~VulkanCommandAllocator();
	
		const VkCommandPool& GetCommandPool() { return m_CommandPool; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	public:
		VkCommandBuffer Allocate(Boolean _IsSecondary = false);
		void Deallocate(const VkCommandBuffer& _Buffer);

		Vector<VkCommandBuffer> BatchAllocate(const uint32& _Size, Boolean _IsSecondary = false);
		void BatchDeallocate(const Vector<VkCommandBuffer>& _Buffers);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		VkCommandPool m_CommandPool;

	public:
		static Ref<VulkanCommandAllocator> Make();
	};

}