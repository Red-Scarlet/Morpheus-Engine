// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/GraphicsContext.h"

#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanImageview.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanRenderpass.h"
#include "VulkanFramebuffer.h"
#include "VulkanCommandPool.h"

#include "VulkanCommandBuffer.h"

namespace Morpheus {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();
		~VulkanContext();

		virtual void Draw() override;
		virtual void Stop() override;

	private:
		void SetupSemaphores();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanSurface* m_VulkanSurface;
		VulkanDevice* m_VulkanDevice;
		VulkanSwapchain* m_VulkanSwapchain;
		VulkanImageview* m_VulkanImageview;
		VulkanRenderpass* m_VulkanRenderpass;
		VulkanGraphicsPipeline* m_VulkanGraphicsPipeline;
		VulkanFramebuffer* m_VulkanFramebuffer;
		VulkanCommandPool* m_VulkanCommandPool;
		VulkanCommandBuffer* m_VulkanCommandBuffer;

		// TODO
		const UINT32 MAX_FRAMES_IN_FLIGHT = 2;
		Vector<VkSemaphore> m_ImageAvailableSemaphores;
		Vector<VkSemaphore> m_RenderFinishedSemaphores;

		//VkSemaphore m_ImageAvailableSemaphore;
		//VkSemaphore m_RenderFinishedSemaphore;
	};

}