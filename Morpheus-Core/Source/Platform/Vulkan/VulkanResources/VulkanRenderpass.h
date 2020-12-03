#pragma once

#include "Platform/Vulkan/VulkanCommon.h"
#include "Morpheus/Renderer/RendererResources/Renderpass.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"

namespace Morpheus {

	class VulkanRenderpass : public Renderpass
	{
	public:
		VulkanRenderpass(const RenderpassLayout& _Layout);
		virtual ~VulkanRenderpass();

		virtual const uint32& GetID() const { return m_ID; }
		virtual const RenderpassLayout& GetLayout() { return m_Layout; }

		const VkRenderPass& GetRenderpass() { return m_Renderpass; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;

		VkRenderPass m_Renderpass;
		RenderpassLayout m_Layout;
		uint32 m_ID;

	public:
		static Ref<VulkanRenderpass> Make(const RenderpassLayout& _Layout);

	};

	class VulkanRenderpassCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanRenderpass>& _Renderpass)
		{ m_Cache[_ID] = _Renderpass; m_Count++; }

		const Ref<VulkanRenderpass>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID); // TODO: Might be a optimize with branchless conditions
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find Renderpass #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }	// TODO: Might be a optimize with branchless conditions
		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanRenderpass>> m_Cache;
		uint32 m_Count = 0;
	};

}