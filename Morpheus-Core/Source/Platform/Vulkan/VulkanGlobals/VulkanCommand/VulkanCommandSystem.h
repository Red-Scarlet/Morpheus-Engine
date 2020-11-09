#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanGlobal.h"

namespace Morpheus {

	class VulkanCommandSystem : public VulkanGlobal
	{
	public:
		VulkanCommandSystem();
		virtual ~VulkanCommandSystem();
	
		const vk::CommandPool GetCommandPool() { return m_CommandPool; }

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		vk::CommandBuffer Allocate(const bool& _LevelFlag = false);
		void Deallocate(const vk::CommandBuffer& _Buffer);

		Vector<vk::CommandBuffer> BatchAllocate(const uint32& Size);
		void BatchDeallocate(const Vector<vk::CommandBuffer>& _Buffers);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		vk::CommandPool m_CommandPool;

	public:
		static Ref<VulkanCommandSystem> Make();
	};

}