// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanGraphicsPipeline.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VulkanInstance* Instance, VulkanDevice* Device, VulkanFramebuffer* Framebuffer, 
			VulkanCommandPool* CommandPool, VulkanSwapchain* VulkanSwapchain, VulkanRenderpass* VulkanRenderpass, VulkanGraphicsPipeline* GraphicsPipeline);
		~VulkanCommandBuffer();

		Vector<VkCommandBuffer> GetCommandBuffer() { return m_CommandBuffers; }

	private:
		void CreateCommandBuffer();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;
		VulkanFramebuffer* m_VulkanFramebuffer;
		VulkanCommandPool* m_VulkanCommandPool;
		VulkanSwapchain* m_VulkanSwapchain;
		VulkanRenderpass* m_VulkanRenderpass;
		VulkanGraphicsPipeline* m_VulkanGraphicsPipeline;

		Vector<VkCommandBuffer> m_CommandBuffers;
	};

}