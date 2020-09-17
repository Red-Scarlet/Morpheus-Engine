// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"
#include "Morpheus/Renderer/RendererCore/Framebuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(const Ref<Renderpass>& _Renderpass);
		~VulkanFramebuffer();

		const VkFramebuffer& GetFramebuffers(const uint32& _Index) { return m_VulkanObject.Framebuffers[_Index]; }
		const uint32& GetSize() { return m_VulkanObject.Framebuffers.size(); }

	private:
		void CreateFramebuffer();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
			Ref<Renderpass> Renderpass;
		} m_VulkanCore;

		struct {
			Vector<VkFramebuffer> Framebuffers;
		} m_VulkanObject;

	};

}