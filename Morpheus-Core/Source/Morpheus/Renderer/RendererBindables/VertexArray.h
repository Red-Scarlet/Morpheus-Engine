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

		virtual const uint32& GetVertexBufferID() const = 0;
		virtual const uint32& GetIndexBufferID() const = 0;
		virtual const uint32& GetUniformBufferID() const = 0;

	public:
		static Ref<VertexArray> Create();

	};

}