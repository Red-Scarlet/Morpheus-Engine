#include "Morppch.h"
#include "VulkanCommandAllocator.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanCommandAllocator::VulkanCommandAllocator()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Command Allocator Was Created!");
	}

	VulkanCommandAllocator::~VulkanCommandAllocator()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Command System Was Destoryed!");
	}

	void VulkanCommandAllocator::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

		VkCommandPoolCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		CreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CreateInfo.queueFamilyIndex = QueueFamilyIndex;

		vkCreateCommandPool(Device, &CreateInfo, nullptr, &m_CommandPool);
	}

	void VulkanCommandAllocator::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyCommandPool(m_CommandPool);
	}

	VkCommandBuffer VulkanCommandAllocator::Allocate(Boolean _IsSecondary)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Vector<VkCommandBuffer> Buffers(1);

		VkCommandBufferAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_CommandPool;
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		AllocateInfo.commandBufferCount = 1;

		if (_IsSecondary)
			AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

		vkAllocateCommandBuffers(Device, &AllocateInfo, Buffers.data());
		return Buffers[0];
	}

	void VulkanCommandAllocator::Deallocate(const VkCommandBuffer& _Buffer)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vkFreeCommandBuffers(Device, m_CommandPool, 1, &_Buffer);
	}

	Vector<VkCommandBuffer> VulkanCommandAllocator::BatchAllocate(const uint32& _Size, Boolean _IsSecondary)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Vector<VkCommandBuffer> Buffers(_Size);

		VkCommandBufferAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_CommandPool;
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		AllocateInfo.commandBufferCount = _Size;

		if (_IsSecondary)
			AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

		vkAllocateCommandBuffers(Device, &AllocateInfo, Buffers.data());
		return Buffers;
	}

	void VulkanCommandAllocator::BatchDeallocate(const Vector<VkCommandBuffer>& _Buffers)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vkFreeCommandBuffers(Device, m_CommandPool, _Buffers.size(), _Buffers.data());
	}

	Ref<VulkanCommandAllocator> VulkanCommandAllocator::Make()
	{
		Ref<VulkanCommandAllocator> s_VulkanCommand = CreateRef<VulkanCommandAllocator>();
		VulkanMemoryManager::GetInstance()->SetCommandSystem(s_VulkanCommand);
		return s_VulkanCommand;
	}

}