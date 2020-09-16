// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"
#include "VulkanRenderpass.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanGraphicsPipeline	//TODO SPLIT SHADER UP INTO OWNING CLASS
	{
	public:
		VulkanGraphicsPipeline(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation, VulkanRenderpass* _Renderpass);
		~VulkanGraphicsPipeline();

		VkPipeline GetPipeline() { return m_VulkanObject.Pipeline; }

	private:
		void CreateGraphicsPipeline();
		VkShaderModule CreateShaderModule(const Vector<float8>& Code);

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
		} m_VulkanCore;

		struct {
			VkPipelineLayout PipelineLayout;
			VkPipeline Pipeline;			
			VulkanRenderpass* Renderpass;

		} m_VulkanObject;

	};


}