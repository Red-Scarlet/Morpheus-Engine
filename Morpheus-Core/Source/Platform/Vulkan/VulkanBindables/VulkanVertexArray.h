#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Morpheus/Renderer/RendererBindables/VertexArray.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"
#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

#include "Platform/Vulkan/VulkanBindable.h"

namespace Morpheus {

	struct VulkanVertexAttributes
	{
	public:
		vk::PipelineVertexInputStateCreateInfo InputState;
		vk::VertexInputBindingDescription InputBinding;
		Vector<vk::VertexInputAttributeDescription> InputAttributes;
	};

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
		const uint32& GetID() { return m_Identifier.ID; }
		void SetID(const uint32& _ID) { m_Identifier.ID = _ID; }

		const bool& CheckBound()
		{ return m_Identifier.Bounded; }

		const vk::PipelineVertexInputStateCreateInfo& GetInputState() { return m_Attributes.InputState; }

	private:
		VulkanVertexAttributes m_Attributes;

	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<UniformBuffer> m_UniformBuffer;

		// TODO: MAKE CLEANER
		VulkanBindableIdentifier m_Identifier;

		// TODO: MAKE CLEANER

	public:
		static Ref<VulkanVertexArray> VulkanCreate();
	};

}