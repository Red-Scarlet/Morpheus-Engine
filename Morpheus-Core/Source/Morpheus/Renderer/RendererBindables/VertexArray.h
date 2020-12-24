#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"
#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"
#include "Morpheus/Renderer/RendererResources/TextureBuffer.h"

namespace Morpheus {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;
		virtual const uint32& GetID() const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) = 0;
		virtual void SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer) = 0;
		virtual void SetTextureBuffer(const Ref<TextureBuffer>& _TextureBuffer) = 0;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() = 0;
		virtual const Ref<UniformBuffer>& GetUniformBuffer() = 0;
		virtual const Ref<TextureBuffer>& GetTextureBuffer() = 0;

		virtual const Boolean& CheckCompiled() const = 0;
		virtual void SetCompiled(const Boolean& _Value) = 0;

	public:
		static Ref<VertexArray> Create();

	};

}