#include "Morppch.h"
#include "VulkanCommandPool.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	VulkanCommandPool::VulkanCommandPool(const uint32& _PrimaryBuffer, const uint32& _SecondaryBuffer)
		: m_PrimaryMax(_PrimaryBuffer), m_PrimaryCount(0), m_SecondaryMax(_SecondaryBuffer), m_SecondaryCount(0)
	{
		m_Device = VulkanInstance::GetInstance()->GetDevice(0);	
		Expand();

		VULKAN_CORE_WARN("[VULKAN] CommandPool Was Created!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		VULKAN_CORE_WARN("[VULKAN] VulkanCommandPool Was Destroyed!");
	}

	const VkCommandBuffer& VulkanCommandPool::GetCommandBuffer(const bool& _SecondaryBuffer)
	{
		MORP_PROFILE_FUNCTION();

		uint32 Index = CheckBufferAvailability(_SecondaryBuffer);
		if (_SecondaryBuffer) 
			return std::get<VkCommandBuffer>(m_SecondaryBuffers.at(Index));
		return std::get<VkCommandBuffer>(m_PrimaryBuffers.at(Index));
	}

	void VulkanCommandPool::VulkanDestroy()
	{
		for (uint32 i = 0; i < m_PrimaryBuffers.size(); i++)
			DeallocatePrimary(i);
		for (uint32 i = 0; i < m_SecondaryBuffers.size(); i++)
			DeallocateSecondary(i);
		for (VkCommandPool pool : m_Pools)
			vkDestroyCommandPool(m_Device->GetLogical(), pool, nullptr);

		m_Pools.clear();
		m_PrimaryBuffers.clear();
		m_SecondaryBuffers.clear();
		m_PrimaryCount = 0;
		m_SecondaryCount = 0;
	}

	void VulkanCommandPool::Expand()
	{
		MORP_PROFILE_FUNCTION();

		m_Pools.resize(m_Pools.size() + 1);
		VkCommandPoolCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		CreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		CreateInfo.queueFamilyIndex = m_Device->GetQueueIndices(QueueType::VULKAN_QUEUE_GRAPHICS);
		VkResult result = vkCreateCommandPool(m_Device->GetLogical(), &CreateInfo, nullptr, &m_Pools.at(m_Pools.size() - 1));
		VULKAN_CORE_ASSERT(result, "[VULKAN] Failed to Create a Command Pool!");

		VULKAN_CORE_INFO("[VULKAN] CommandPool #" + ToString(m_Pools.size() - 1) + " with maximum size of " + 
			ToString(m_PrimaryMax) + ", " + ToString(m_SecondaryMax) + " was Created!");
	}

	void VulkanCommandPool::AllocatePrimary()
	{
		MORP_PROFILE_FUNCTION();

		m_PrimaryBuffers.resize(m_PrimaryBuffers.size() + 1);
		if (m_PrimaryCount >= m_PrimaryMax)
		{ Expand(); m_PrimaryMax += m_PrimaryMax; }
		uint32 PoolIndex = m_PrimaryCount / m_PrimaryMax;

		VkCommandBufferAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_Pools.at(PoolIndex);
		AllocateInfo.commandBufferCount = 1;
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VkCommandBuffer& buffer = std::get<VkCommandBuffer>(m_PrimaryBuffers.at(m_PrimaryCount));
		VkResult Result = vkAllocateCommandBuffers(m_Device->GetLogical(), &AllocateInfo, &buffer);
		VULKAN_CORE_ASSERT(Result, "[VULKAN] CommandBuffer cannot be Allocated!");

		std::get<bool>(m_PrimaryBuffers.at(m_PrimaryCount)) = false;
		VULKAN_CORE_INFO("[VULKAN] Primary CommandBuffer #" + ToString(m_PrimaryCount) +
			" was Allocated from Pool #" + ToString(m_Pools.size() - 1));
		m_PrimaryCount++;
	}

	void VulkanCommandPool::AllocateSecondary()
	{
		MORP_PROFILE_FUNCTION();

		m_SecondaryBuffers.resize(m_SecondaryBuffers.size() + 1);
		if (m_SecondaryCount >= m_SecondaryMax)
		{ Expand(); m_SecondaryMax += m_SecondaryMax; }
		uint32 PoolIndex = m_SecondaryCount / m_SecondaryMax;

		VkCommandBufferAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocateInfo.commandPool = m_Pools.at(PoolIndex);
		AllocateInfo.commandBufferCount = 1;
		AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

		VkCommandBuffer& buffer = std::get<VkCommandBuffer>(m_SecondaryBuffers.at(m_SecondaryCount));
		VkResult Result = vkAllocateCommandBuffers(m_Device->GetLogical(), &AllocateInfo, &buffer);
		VULKAN_CORE_ASSERT(Result, "[VULKAN] CommandBuffer cannot be Allocated!");

		std::get<bool>(m_SecondaryBuffers.at(m_SecondaryCount)) = false;
		VULKAN_CORE_INFO("[VULKAN] Secondary CommandBuffer #" + ToString(m_SecondaryCount) +
			" was Allocated from Pool #" + ToString(m_Pools.size() - 1));
		m_SecondaryCount++;
	}

	void VulkanCommandPool::DeallocatePrimary(const uint32& _Index)
	{
		uint32 PoolIndex = _Index / m_PrimaryMax;
		Tuple<VkCommandBuffer, bool>& Buffer = m_PrimaryBuffers.at(_Index);
		if(std::get<bool>(Buffer) == true) {
			vkFreeCommandBuffers(m_Device->GetLogical(), m_Pools[PoolIndex], 1, &std::get<VkCommandBuffer>(Buffer));
			std::remove(std::begin(m_PrimaryBuffers), std::end(m_PrimaryBuffers), Buffer);
			m_PrimaryCount--;
		}
	}

	void VulkanCommandPool::DeallocateSecondary(const uint32& _Index)
	{
		uint32 PoolIndex = _Index / m_SecondaryMax;
		Tuple<VkCommandBuffer, bool>& Buffer = m_SecondaryBuffers.at(_Index);
		if (std::get<bool>(Buffer) == true) {
			vkFreeCommandBuffers(m_Device->GetLogical(), m_Pools[PoolIndex], 1, &std::get<VkCommandBuffer>(Buffer));
			std::remove(std::begin(m_SecondaryBuffers), std::end(m_SecondaryBuffers), Buffer);
			m_SecondaryCount--;
		}
	}

	uint32 VulkanCommandPool::CheckBufferAvailability(const bool& _SecondaryBuffer)
	{
		MORP_PROFILE_FUNCTION();

		uint32 Index = 0;
		if (_SecondaryBuffer) {
			for (auto& Buffer : m_SecondaryBuffers)
				if (std::get<bool>(Buffer) == false) {
					std::get<bool>(Buffer) = true;
					return Index;
				} else Index++;
			AllocateSecondary();
			return m_SecondaryBuffers.size() - 1;
		} else {
			for (auto& Buffer : m_PrimaryBuffers)
				if (std::get<bool>(Buffer) == false) {
					std::get<bool>(Buffer) = true;
					return Index;
				} else Index++;
			AllocatePrimary();
			return m_PrimaryBuffers.size() - 1;
		}
	}

	Ref<VulkanCommandPool> VulkanCommandPool::Create(const uint32& _PrimaryBuffer, const uint32& _SecondaryBuffer)
	{
		return CreateRef<VulkanCommandPool>(_PrimaryBuffer, _SecondaryBuffer);
	}

	void VulkanCommandPool::Destroy(const Ref<VulkanCommandPool>& _Pool)
	{
		_Pool->VulkanDestroy();
	}

}}