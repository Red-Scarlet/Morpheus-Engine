// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"
#include "VulkanRenderpass.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation, VulkanRenderpass* _Renderpass);
		~VulkanFramebuffer();

		const VkFramebuffer& GetFramebuffers(const uint32& _Index) { return m_VulkanObject.Framebuffers[_Index]; }
		const uint32& GetSize() { return m_VulkanObject.Framebuffers.size(); }

	private:
		void CreateFramebuffer();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
		} m_VulkanCore;

		struct {
			Vector<VkFramebuffer> Framebuffers;
			VulkanRenderpass* Renderpass;

		} m_VulkanObject;

	};

}