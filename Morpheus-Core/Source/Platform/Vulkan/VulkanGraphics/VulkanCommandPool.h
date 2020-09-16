// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(VulkanLogicalDevice* _lDevice);
		~VulkanCommandPool();

		VkCommandPool GetCommandPool() { return m_VulkanObject.CommandPool; }

	private:
		void CreateCommandPool();

	private:
		struct {
			VulkanPhysicalDevice* pDevice;
			VulkanLogicalDevice* lDevice;
		} m_VulkanCore;

		struct {
			VkCommandPool CommandPool;
		} m_VulkanObject;

	};

}