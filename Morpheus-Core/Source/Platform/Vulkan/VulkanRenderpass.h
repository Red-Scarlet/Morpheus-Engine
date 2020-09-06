// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanRenderpass
	{
	public:
		VulkanRenderpass(VulkanInstance* Instance, VulkanDevice* Device, VulkanSwapchain* Swapchain);
		~VulkanRenderpass();

		VkRenderPass GetRenderpass() { return m_RenderPass; }
		VkRenderPassCreateInfo& GetRenderpassInfo() { return m_RenderPassInfo; }

	private:
		void CreateRenderpass();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;
		VulkanSwapchain* m_VulkanSwapchain;

		VkRenderPass m_RenderPass;
		VkRenderPassCreateInfo m_RenderPassInfo;

	};


}