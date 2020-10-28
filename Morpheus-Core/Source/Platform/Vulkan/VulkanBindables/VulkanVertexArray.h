#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererBindables/VertexArray.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"
#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

namespace Morpheus {

	class VulkanVertexArray : public VertexArray
	{
	public:
		VulkanVertexArray();
		virtual ~VulkanVertexArray();

		virtual void Bind() override;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;
		virtual void SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer) override;

		virtual const Ref<VertexBuffer>& GetVertexBuffer() const override;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override;;
		virtual const Ref<UniformBuffer>& GetUniformBuffer() const override;;

	public:
		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<UniformBuffer> m_UniformBuffer;
		uint32 m_ID;

	public:
		static Ref<VulkanVertexArray> VulkanCreate();
	};

}