#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanSwapchain.h"
#include "Morpheus/Renderer/RendererResources/Renderpass.h"

namespace Morpheus {

	#define VULKAN_RENDERPASS_CACHE_TYPE VulkanCacheType(4)
	class VulkanRenderpass : public Renderpass
	{
	public:
		typedef VulkanCacheMember<VulkanRenderpass> RenderpassCache;

	public:
		VulkanRenderpass(const RenderpassLayout& _Layout, const Ref<RenderpassCache>& _Cache);
		virtual ~VulkanRenderpass();
		virtual const uint32& GetID() const { return m_ID; }

		virtual const RenderpassLayout& GetLayout() { return m_Layout; }
		const VkRenderPass& GetRenderpass() { return m_Renderpass; }

	private:
		void VulkanCreate();
		void VulkanDestory();

		VkAttachmentDescription CreateAttachment(const RenderpassElement& _Element);
		VkAttachmentReference CreateReference(const RenderpassElement& _Element);
		VkSubpassDescription CreateDescription(const RenderpassElement& _Element, const VkAttachmentReference& _Reference);
		VkSubpassDependency CreateDependency(const RenderpassElement& _Element);

	private:
		Ref<VulkanLogicalDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<RenderpassCache> m_Cache;

		VkRenderPass m_Renderpass;
		RenderpassLayout m_Layout;
		uint32 m_ID;

	public:
		static Ref<VulkanRenderpass> Make(const RenderpassLayout& _Layout);

	};

}