// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderpass.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanGraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(VulkanInstance* Instance, VulkanDevice* Device, VulkanSwapchain* Swapchain, VulkanRenderpass* Renderpass);
		~VulkanGraphicsPipeline();

		VkPipeline GetPipeline() { return m_GraphicsPipeline; }

	private:
		void CreateGraphicsPipeline();
		VkShaderModule CreateShaderModule(const Vector<FLOAT8>& Code);

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;
		VulkanSwapchain* m_VulkanSwapchain;
		VulkanRenderpass* m_Renderpass;

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};


}