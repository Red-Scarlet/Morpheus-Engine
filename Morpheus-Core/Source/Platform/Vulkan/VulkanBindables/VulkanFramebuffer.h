#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "Morpheus/Renderer/RendererBindables/FrameBuffer.h"

namespace Morpheus {

	struct VulkanSwapchainBuffer
	{
	public:
		vk::Image Image;
		std::array<vk::ImageView, 2> Views;
		vk::Framebuffer Framebuffer;
	};

	class VulkanFrameBuffer : public FrameBuffer
	{
	public:
		VulkanFrameBuffer(const RenderpassLayout& _Layout);
		virtual ~VulkanFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const uint32& GetID() const override { return m_ID; }

		const VkFramebuffer& GetFrameBuffer(const uint32& _Index) { return m_Framebuffers[_Index]; }
		const vk::RenderPass& GetRenderpass() { return m_Renderpass; };

	private:
		void InvalidateAttachments();
		void CreateRenderpass();
		void CreateFrameBuffers();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanSwapchain> m_Swapchain;

		RenderpassLayout m_Layout;
		vk::RenderPass m_Renderpass;

		Vector<vk::AttachmentDescription> m_Attachments;
		Vector<vk::SubpassDependency> m_Dependencies;

		Vector<vk::AttachmentReference> m_ColorReference;
		Vector<vk::AttachmentReference> m_DepthReference;
		Vector<vk::SubpassDescription> m_SubpassDesc;

		Vector<VkFramebuffer> m_Framebuffers;
		Vector<VkImageView> m_Views;
		Vector<VkImage> m_Images;

		uint32 m_ID;

	public:
		static Ref<VulkanFrameBuffer> Make(const RenderpassLayout& _Layout);
	};

	class VulkanFrameBufferCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanFrameBuffer>& _FrameBuffer)
		{ m_Cache[_ID] = _FrameBuffer; m_Count++; }

		const Ref<VulkanFrameBuffer>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find FrameBuffer #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanFrameBuffer>> m_Cache;
		uint32 m_Count = 0;
	};

}