#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

#include "Morpheus/Renderer/RendererBindables/FrameBuffer.h"

namespace Morpheus {

	class VulkanFrameBuffer : public FrameBuffer
	{
	public:
		VulkanFrameBuffer(const Ref<Renderpass>& _Renderpass);
		virtual ~VulkanFrameBuffer();
		virtual const uint32& GetID() const override { return m_ID; }

		virtual void Bind(const uint32& _Slot) override;
		virtual void Unbind() override;

		const VkFramebuffer& GetFrameBuffer(const uint32& _Index) { return m_Framebuffers[_Index]; }
		const VkRenderPass& GetRenderpass() { return m_Renderpass->GetRenderpass(); };

	private:
		void CreateFrameBuffers();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanSwapchain> m_Swapchain;

		Ref<VulkanRenderpass> m_Renderpass;
		Vector<VkFramebuffer> m_Framebuffers;
		Vector<VkImageView> m_Views;
		Vector<VkImage> m_Images;

		uint32 m_ID;

	public:
		static Ref<VulkanFrameBuffer> Make(const Ref<Renderpass>& _Renderpass);
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