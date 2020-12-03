#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanQueue.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommandAllocator.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommand.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanExecutionStack.h"

namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void SetClearColor(const Vector4& _ClearColor) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& _VertexArray) override;	
		virtual void Flush() override;

	private:
		void BuildPrimary();
		void BuildSecondary();

	private:
		Ref<VulkanQueue> m_Queue;
		Ref<VulkanCommandAllocator> m_CommandSystem;

		Ref<VulkanExecutionStack> m_CommandExecutor;
		Ref<VulkanCommandBuffer> m_CommandBuffers[2];
		UnorderedMap<uint32, Ref<VulkanCommandBuffer>> m_SecondaryCommandBuffers;

		Vector4 m_ClearColor = { 0.25f, 0.25f, 0.25f, 1.00f };
		Boolean m_CompileRequired = false;
	};


}