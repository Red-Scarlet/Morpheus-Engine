// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanRenderpass
	{
	public:
		VulkanRenderpass(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation);
		~VulkanRenderpass();

		VkRenderPass GetRenderpass() { return m_VulkanObject.Renderpass; }
		VkRenderPassCreateInfo& GetInfo() { return m_VulkanObject.Info; }

	private:
		void CreateRenderpass();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
		} m_VulkanCore;

		struct {
			VkRenderPass Renderpass;
			VkRenderPassCreateInfo Info;
		} m_VulkanObject;


	};


}