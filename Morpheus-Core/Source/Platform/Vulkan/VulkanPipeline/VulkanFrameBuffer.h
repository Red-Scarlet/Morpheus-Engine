#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanSurface.h"
#include "Platform/Vulkan/VulkanCore/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

#include "Morpheus/Renderer/RendererBindables/FrameBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

#include "Platform/Vulkan/VulkanCommandSystem/VulkanExecutionStack.h"

namespace Morpheus {

	#define VULKAN_FRAMEBUFFER_CACHE_TYPE VulkanCacheType(7)
	class VulkanFrameBuffer : public FrameBuffer
	{
	public:
		typedef VulkanCacheMember<VulkanFrameBuffer> FrameBufferCache;

	public:
		VulkanFrameBuffer(const Ref<Renderpass>& _Renderpass, const Ref<FrameBufferCache> _Cache);
		virtual ~VulkanFrameBuffer();
		virtual const uint32& GetID() const override { return m_ID; }

		virtual void* GetImage() override;

		virtual void Bind() override;
		virtual void Unbind() override;

		const VkRenderPass& GetRenderpass() { return m_Renderpass->GetRenderpass(); };
		const VkFramebuffer& GetFrameBuffer(const uint32& _Index) { return m_Framebuffers[_Index]; }

	private:
		void CreateFrameBuffers();
		void CopyImage(const uint32& _Index);
		void Transition(const Ref<VulkanExecutionStack>& _ExecutionStack, const VkImage& _Image,
			const VkAccessFlags& srcAccessMask,
			const VkAccessFlags& dstAccessMask,
			const VkImageLayout& oldImageLayout,
			const VkImageLayout& newImageLayout,
			const VkPipelineStageFlags& srcStageMask,
			const VkPipelineStageFlags& dstStageMask,
			const VkImageSubresourceRange& subresourceRange);

		void Copy(const Ref<VulkanExecutionStack>& _ExecutionStack, const VkImage& _SourceImage, const VkImage& _DesinationImage, const uint32& _Width, const uint32& _Height);

	private:
		Ref<VulkanLogicalDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<FrameBufferCache> m_Cache;

		Ref<VulkanRenderpass> m_Renderpass;
		Vector<VkFramebuffer> m_Framebuffers;
		Vector<VkImageView> m_Views;
		Vector<VkImage> m_Images;
		void* SwapchainImageOutput;

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