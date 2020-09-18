// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanRenderpass : public Renderpass
	{
	public:
		VulkanRenderpass();
		virtual ~VulkanRenderpass();

		const VkRenderPass& GetRenderpass() { return m_VulkanObject.Renderpass; }
		const VkFramebuffer& GetFramebuffer(const uint32& _Index);
		const VkRenderPassBeginInfo& GetBeginInfo(const VkClearValue& _Color, const VkExtent2D& _Extent, const uint32& _Index);

	private:
		void CreateRenderpass();
		void CreateFramebuffer();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
		} m_VulkanCore;

		struct {
			VkRenderPass Renderpass;
			Vector<VkFramebuffer> Framebuffers;
		} m_VulkanObject;


	};


}