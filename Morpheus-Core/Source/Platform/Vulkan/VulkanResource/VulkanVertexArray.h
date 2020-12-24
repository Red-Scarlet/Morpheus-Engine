#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Morpheus/Renderer/RendererBindables/VertexArray.h"

namespace Morpheus {

	#define VULKAN_VERTEX_ARRAY_CACHE_TYPE VulkanCacheType(5)
	class VulkanVertexArray : public VertexArray
	{
	public:
		typedef VulkanCacheMember<VulkanVertexArray> VertexArrayCache;

	public:
		VulkanVertexArray(const Ref<VertexArrayCache>& _Cache);
		virtual ~VulkanVertexArray();
		virtual const uint32& GetID() const override { return m_ID; }

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;
		virtual void SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer) override;
		virtual void SetTextureBuffer(const Ref<TextureBuffer>& _TextureBuffer) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() override { return m_VertexBuffer; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
		virtual const Ref<UniformBuffer>& GetUniformBuffer() override { return m_UniformBuffer; }
		virtual const Ref<TextureBuffer>& GetTextureBuffer() override { return m_TextureBuffer; }

		virtual const Boolean& CheckCompiled() const override { return m_Compiled; }
		virtual void SetCompiled(const Boolean& _Value) override { m_Compiled = _Value; }

	private:
		Ref<VertexArrayCache> m_Cache;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<UniformBuffer> m_UniformBuffer;
		Ref<TextureBuffer> m_TextureBuffer;

		Boolean m_Compiled = true;
		Boolean m_ShaderCompiled = false;
		uint32 m_ID;

	public:
		static Ref<VulkanVertexArray> Make();
	};

}