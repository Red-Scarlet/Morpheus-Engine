#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"

namespace Morpheus {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(QuadVertex* _Data, const uint32& _Size);
		VulkanVertexBuffer(const uint32& _Size);
		virtual ~VulkanVertexBuffer();
		
		virtual const uint32& GetID() const override { return m_ID; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	public:
		const vk::Buffer& GetBuffer() { return m_VulkanBuffer.Buffer; }
		virtual void SetData(const Memory32& _Data, const uint32& _Size) override;
		const bool& GetCompleted() { return m_Completed; }

	private:
		void Submit(const VulkanBuffer& _Staging);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommandSystem> m_CommandSystem;

		VulkanBuffer m_VulkanBuffer;
		QuadVertex* m_Data;
		uint32 m_Size;

		bool m_Completed = false;

		uint32 m_ID;

	public:
		static Ref<VulkanVertexBuffer> Make(QuadVertex* _Data, const uint32& _Size);
		static Ref<VulkanVertexBuffer> Make(const uint32& _Size);

	};

	class VulkanVertexBufferCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanVertexBuffer>& _VertexBuffer)
		{ m_Cache[_ID] = _VertexBuffer; m_Count++; }

		const Ref<VulkanVertexBuffer>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find VertexBuffer #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanVertexBuffer>> m_Cache;
		uint32 m_Count = 0;
	};

}