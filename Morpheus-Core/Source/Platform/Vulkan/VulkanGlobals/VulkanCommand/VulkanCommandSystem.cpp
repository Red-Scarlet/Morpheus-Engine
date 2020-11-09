#include "Morppch.h"
#include "VulkanCommandSystem.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanCommandSystem::VulkanCommandSystem()
		: VulkanGlobal(VulkanGlobalTypes::VulkanCommand)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Command System Was Created!");
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanCommand));
	}

	VulkanCommandSystem::~VulkanCommandSystem()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Command System Was Destoryed!");
	}

	void VulkanCommandSystem::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

		vk::CommandPoolCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::CommandPoolCreateFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
			CreateInfo.queueFamilyIndex = QueueFamilyIndex;
		}

		m_CommandPool = Device.createCommandPool(CreateInfo);
	}

	void VulkanCommandSystem::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyCommandPool(m_CommandPool);
	}

	vk::CommandBuffer VulkanCommandSystem::Allocate(const bool& _LevelFlag)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Vector<vk::CommandBuffer> Buffers;

		Buffers = Device.allocateCommandBuffers(
			vk::CommandBufferAllocateInfo(
				m_CommandPool,
				vk::CommandBufferLevel::ePrimary,
				1
			)
		);	

		return Buffers[0];
	}

	void VulkanCommandSystem::Deallocate(const vk::CommandBuffer& _Buffer)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeCommandBuffers(m_CommandPool, _Buffer);
	}

	Vector<vk::CommandBuffer> VulkanCommandSystem::BatchAllocate(const uint32& _Size)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Vector<vk::CommandBuffer> Buffers(_Size);

		vk::CommandBufferAllocateInfo AllocateInfo = {};
		{
			AllocateInfo.commandPool = m_CommandPool;
			AllocateInfo.level = vk::CommandBufferLevel::ePrimary;
			AllocateInfo.commandBufferCount = _Size;
		}

		Buffers = Device.allocateCommandBuffers(AllocateInfo);
		return Buffers;
	}

	void VulkanCommandSystem::BatchDeallocate(const Vector<vk::CommandBuffer>& _Buffers)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeCommandBuffers(m_CommandPool, _Buffers);
	}

	Ref<VulkanCommandSystem> VulkanCommandSystem::Make()
	{
		Ref<VulkanCommandSystem> s_VulkanCommand = CreateRef<VulkanCommandSystem>();
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit(s_VulkanCommand);
		return s_VulkanCommand;
	}

}