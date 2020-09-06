// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanImageview.h"
#include "VulkanRenderpass.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(VulkanInstance* Instance, VulkanDevice* Device);
		~VulkanCommandPool();

		VkCommandPool GetCommandPool() { return m_CommandPool; }

	private:
		void CreateCommandPool();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;

		VkCommandPool m_CommandPool;
	};

}