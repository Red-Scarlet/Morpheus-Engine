#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Morpheus/Renderer/RendererBindables/VertexArray.h"
#include "VulkanBindable.h"

namespace Morpheus {

	class VulkanVertexArray : public VulkanBindable, public VertexArray
	{
	public:
		VulkanVertexArray();
		virtual ~VulkanVertexArray();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		virtual void Bind() override;

		virtual void SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer) override;
		virtual void SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer) override;

		virtual const uint32& GetVertexBufferID() const override;
		virtual const uint32& GetIndexBufferID() const override;
		virtual const uint32& GetUniformBufferID() const override;

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<UniformBuffer> m_UniformBuffer;

	public:
		static Ref<VulkanVertexArray> Make();
	};

}