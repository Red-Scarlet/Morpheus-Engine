#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommandAllocator.h"

#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"

namespace Morpheus {

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32* _Indices, const uint32& _Size);
		virtual ~VulkanIndexBuffer();

		virtual const uint32& GetID() const override { return m_ID; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	public:
		const vk::Buffer& GetBuffer() { return m_VulkanBuffer.Buffer; }
		const uint32& GetCount() { return m_Count; }

	private:
		void Submit(const VulkanBuffer& _Staging);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommandAllocator> m_CommandSystem;

		VulkanBuffer m_VulkanBuffer;
		uint32* m_Data;
		uint32 m_Count;
		uint32 m_Size;

		uint32 m_ID;

	public:
		static Ref<VulkanIndexBuffer> Make(uint32* _Indices, const uint32& _Size);
	};

	class VulkanIndexBufferCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanIndexBuffer>& _IndexBuffer)
		{ m_Cache[_ID] = _IndexBuffer; m_Count++; }

		const Ref<VulkanIndexBuffer>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find IndexBuffer #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanIndexBuffer>> m_Cache;
		uint32 m_Count = 0;
	};

}