// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include "VulkanFramebuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanGraphicsPipeline.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation, VulkanFramebuffer* _Framebuffer, VulkanCommandPool* _CommandPool, 
			VulkanRenderpass* _VulkanRenderpass, VulkanGraphicsPipeline* _GraphicsPipeline);
		~VulkanCommandBuffer();

		//Vector<VkCommandBuffer> GetCommandBuffer() { return m_CommandBuffers; }
		const VkCommandBuffer& GetCommandBuffer(const uint32& _Index) { return m_CommandBuffers[_Index]; }

	private:
		void CreateCommandBuffer();

	private:
		VulkanLogicalDevice* m_VulkanDevice;
		VulkanPresentation* m_VulkanPresentation;
		VulkanCommandPool* m_VulkanCommandPool;

		VulkanFramebuffer* m_VulkanFramebuffer;
		VulkanRenderpass* m_VulkanRenderpass;
		VulkanGraphicsPipeline* m_VulkanGraphicsPipeline;

		Vector<VkCommandBuffer> m_CommandBuffers;
	};

}