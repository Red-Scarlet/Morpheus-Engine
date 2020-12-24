#include "Morppch.h"
#include "VulkanCommandPool.h"

namespace Morpheus { namespace Vulkan {

	VulkanCommandPool::VulkanCommandPool(const Ref<VulkanDevice>& _Device, const uint32& _Max, const bool& _IsSecondary)
		: m_Device(_Device), m_AllocationMax(_Max), m_IsSecondary(_IsSecondary)
	{
		m_AllocationCount = 0;

		Expand();
		VULKAN_CORE_WARN("[VULKAN] VulkanCommandPool Was Created!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		for (uint32 i = 0; i < m_CommandBuffers.size(); i++)
			Deallocate(i);

		for (VkCommandPool& pool : m_Pools)
			vkDestroyCommandPool(m_Device->GetLogical(), pool, nullptr);
		VULKAN_CORE_WARN("[VULKAN] VulkanCommandPool Was Destroyed!");
	}

	const VkCommandBuffer& VulkanCommandPool::GetCommandBuffer()
	{
		MORP_PROFILE_FUNCTION();

		uint32 Index = CheckBufferAvailability();
		Tuple<VkCommandBuffer, uint32, bool>& Buffer = m_CommandBuffers.at(Index);
		std::get<bool>(Buffer) = true;
		return std::get<VkCommandBuffer>(Buffer);
	}

	void VulkanCommandPool::Expand()
	{
		MORP_PROFILE_FUNCTION();

		VkCommandPool Pool = {};
		VkCommandPoolCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		CreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CreateInfo.queueFamilyIndex = m_Device->GetQueueIndices(QueueType::VULKAN_QUEUE_GRAPHICS);
		vkCreateCommandPool(m_Device->GetLogical(), &CreateInfo, nullptr, &Pool);
		m_Pools.push_back(Pool);

		VULKAN_CORE_INFO("[VULKAN] Command Pool #" + ToString(m_Pools.size() - 1) + " with maximum size of " + ToString(m_AllocationMax) + " was Created!");
	}

	void VulkanCommandPool::Allocate()
	{
		MORP_PROFILE_FUNCTION();

		m_CommandBuffers.resize(m_CommandBuffers.size() + 1);

		if (m_AllocationCount >= m_AllocationMax)
		{ Expand(); m_AllocationCount = 0; }

		VkCommandBufferAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_Pools[m_Pools.size() - 1];
		AllocateInfo.commandBufferCount = 1;
		if (m_IsSecondary) AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		else AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		Tuple<VkCommandBuffer, uint32, bool>& Buffer = m_CommandBuffers.at(m_CommandBuffers.size() - 1);
		VkResult Result = vkAllocateCommandBuffers(m_Device->GetLogical(), &AllocateInfo, &std::get<VkCommandBuffer>(Buffer));
		MORP_CORE_ASSERT(Result, "[VULKAN] CommandBuffer cannot be Allocated!");
		std::get<uint32>(Buffer) = m_Pools.size() - 1;
		std::get<bool>(Buffer) = false;

		VULKAN_CORE_DEBUG_PTR(std::get<VkCommandBuffer>(Buffer));
		VULKAN_CORE_INFO("[VULKAN] Command Buffer #" + ToString(m_CommandBuffers.size() - 1) + " was Allocated from Pool #" + ToString(m_Pools.size() - 1));
		m_AllocationCount++;
	}

	void VulkanCommandPool::Deallocate(const uint32& _Index)
	{
		// Get Size
		// Max Size = 2 
		// 

		// ID == 16
		// Pools 8
		// ID_M Pools * MaxSize = 16

		// ID_M - _Index = 0;

		// ID == 15
		// Pools 8
		// ID_M Pools * MaxSize = 16

		// ID_M - _Index = 1;

		// ID == 13
		// Pools 8
		// ID_M Pools * MaxSize = 16

		// ID_M - _Index = 3;

		Tuple<VkCommandBuffer, uint32, bool>& Buffer = m_CommandBuffers.at(_Index);
		vkFreeCommandBuffers(m_Device->GetLogical(), m_Pools[std::get<uint32>(Buffer)], 1, &std::get<VkCommandBuffer>(Buffer));
		m_CommandBuffers.resize(m_CommandBuffers.size() - 1);
	}

	uint32 VulkanCommandPool::CheckBufferAvailability()
	{
		MORP_PROFILE_FUNCTION();

		uint32 Index = 0;
		for (const auto& Buffer : m_CommandBuffers)
			if (std::get<bool>(Buffer) == false)
				return Index;
			else Index++;
		Allocate();
		return m_CommandBuffers.size() - 1;
	}

	Ref<VulkanCommandPool> VulkanCommandPool::Create(const Ref<VulkanDevice>& _Device, const uint32& _Max, const bool& _IsSecondary)
	{
		return CreateRef<VulkanCommandPool>(_Device, _Max, _IsSecondary);
	}

}}