#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"
#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

namespace Morpheus {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() = 0;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) = 0;
		virtual void SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer) = 0;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const Ref<UniformBuffer>& GetUniformBuffer() const = 0;

	public:
		static Ref<VertexArray> Create();

	};

}