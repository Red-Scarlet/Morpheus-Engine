// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanImageview.h"
#include "VulkanRenderpass.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(VulkanInstance* Instance, VulkanDevice* Device, VulkanImageview* Imageview, VulkanRenderpass* Renderpass);
		~VulkanFramebuffer();

		Vector<VkFramebuffer> GetFramebuffers() { return m_SwapChainFramebuffers; }

	private:
		void CreateFramebuffer();

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanDevice* m_VulkanDevice;
		VulkanImageview* m_VulkanImageview;
		VulkanRenderpass* m_VulkanRenderpass;

		Vector<VkFramebuffer> m_SwapChainFramebuffers;
	};

}