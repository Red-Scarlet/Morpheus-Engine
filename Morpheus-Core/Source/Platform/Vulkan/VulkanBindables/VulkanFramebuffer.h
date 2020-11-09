#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

#include "Morpheus/Renderer/RendererBindables/FrameBuffer.h"
#include "Morpheus/Renderer/RendererResources/Renderpass.h"

#include "VulkanBindable.h"

namespace Morpheus {

	struct VulkanSwapchainBuffer
	{
	public:
		vk::Image Image;
		std::array<vk::ImageView, 2> Views;
		vk::Framebuffer Framebuffer;
	};

	class VulkanFramebuffer : public VulkanBindable, public FrameBuffer
	{
	public:
		VulkanFramebuffer(const Ref<Renderpass> _Renderpass, bool Depth);
		virtual ~VulkanFramebuffer();
		virtual void Bind() override;

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const Vector<VkFramebuffer>& GetFramebuffers() { return m_Framebuffers; }
		const const VkFramebuffer& GetFramebuffer(const uint32& _Index) { return m_Framebuffers[_Index]; }
		void CreateFramebuffers();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanRenderpass> m_Renderpass;

		Vector<VkFramebuffer> m_Framebuffers;
		Vector<VkImageView> m_Views;
		Vector<VkImage> m_Images;
		bool m_CreateDepthImages;

	public:
		static Ref<VulkanFramebuffer> Make(const Ref<Renderpass> _Renderpass, bool Depth = true);
	};

}