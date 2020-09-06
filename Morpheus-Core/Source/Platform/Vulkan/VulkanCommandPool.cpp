#include "Morppch.h"
#include "VulkanCommandPool.h"

namespace Morpheus {

	VulkanCommandPool::VulkanCommandPool(VulkanInstance* Instance, VulkanDevice* Device)
		: m_VulkanInstance(Instance), m_VulkanDevice(Device)
	{
		CreateCommandPool();
		MORP_CORE_WARN("[VULKAN] Vulkan CommandPool Was Created!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_VulkanDevice->GetLogicalDevice(), m_CommandPool, nullptr);
	}

	void VulkanCommandPool::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = m_VulkanDevice->GetQueueFamilies();

		VkCommandPoolCreateInfo poolInfo {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

		VkResult result = vkCreateCommandPool(m_VulkanDevice->GetLogicalDevice(), &poolInfo, nullptr, &m_CommandPool);
		MORP_CORE_ASSERT(result, "Failed to create CommandPool!");
	}

}

