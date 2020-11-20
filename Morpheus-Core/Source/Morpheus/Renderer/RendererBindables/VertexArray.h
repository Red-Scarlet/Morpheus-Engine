#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"

namespace Morpheus {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const uint32& GetID() const = 0;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) = 0;
		virtual void SetUniformBuffer(const uint32& _UniformBufferID) = 0;

		virtual const uint32& GetVertexBufferID() const = 0;
		virtual const uint32& GetIndexBufferID() const = 0;
		virtual const uint32& GetUniformBufferID() const = 0;

		virtual const Boolean& CheckCompiled() const = 0;
		virtual void SetCompiled(const Boolean& _Value) = 0;

	public:
		static Ref<VertexArray> Create();

	};

}