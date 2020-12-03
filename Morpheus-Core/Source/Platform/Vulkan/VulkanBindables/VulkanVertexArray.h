#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanTextureBuffer.h"

#include "Morpheus/Renderer/RendererBindables/VertexArray.h"

namespace Morpheus {

	class VulkanVertexArray : public VertexArray
	{
	public:
		VulkanVertexArray();
		virtual ~VulkanVertexArray();

		virtual void Bind(const uint32& _Slot) override;
		virtual void Unbind() override;
		virtual const uint32& GetID() const override { return m_ID; }

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;
		virtual void SetTextureBuffer(const Ref<TextureBuffer>& _TextureBuffer) override;

		virtual const uint32& GetVertexBufferID() const override { return m_VertexBuffer->GetID(); }
		virtual const uint32& GetIndexBufferID() const override { return m_IndexBuffer->GetID(); }
		virtual const uint32& GetUniformBufferID() const override { return m_UniformBuffer->GetID(); }
		virtual const uint32& GetTextureBufferID() const override { return m_TextureBuffer->GetID(); }

		virtual const Boolean& CheckCompiled() const override { return m_Compiled; }
		virtual void SetCompiled(const Boolean& _Value) override { m_Compiled = true; }

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<TextureBuffer> m_TextureBuffer;
		Ref<VulkanUniformBuffer> m_UniformBuffer;
		Boolean m_Compiled = true;
		uint32 m_ID;

		Boolean m_ShaderCompiled = false;

	public:
		static Ref<VulkanVertexArray> Make();
	};

	class VulkanVertexArrayCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanVertexArray>& _VertexArray)
		{ m_Cache[_ID] = _VertexArray; m_Count++; }

		const Ref<VulkanVertexArray>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find VertexArray #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	private:
		UnorderedMap<uint32, Ref<VulkanVertexArray>> m_Cache;
		uint32 m_Count = 0;
	};

}