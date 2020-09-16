// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanCommandPool.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	VulkanCommandPool::VulkanCommandPool(VulkanLogicalDevice* _lDevice)
		: m_VulkanCore({ VulkanInstance::GetInstance()->GetPhysicalDevice(), _lDevice })
	{
		CreateCommandPool();
		MORP_CORE_WARN("[VULKAN] CommandPool Was Created!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.CommandPool, nullptr);
	}

	void VulkanCommandPool::CreateCommandPool()
	{
		QueueFamilyIndices QueueFamilyIndices = m_VulkanCore.pDevice->m_VulkanObject.Indices;

		VkCommandPoolCreateInfo PoolInfo {};
		{
			PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			PoolInfo.queueFamilyIndex = QueueFamilyIndices.GraphicsFamily.value();
		}

		VkResult result = vkCreateCommandPool(m_VulkanCore.lDevice->GetDevice(), &PoolInfo, nullptr, &m_VulkanObject.CommandPool);
		MORP_CORE_ASSERT(result, "Failed to create CommandPool!");
	}

}

