#include "Morppch.h"
#include "VulkanCommandList.h"

namespace Morpheus {

	namespace VulkanCommandList {

		Ref<ResetBuffer> ResetBuffer::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<ResetBuffer>(_Buffer);
		}

		Ref<BeginBuffer> BeginBuffer::Create(const Ref<VulkanCommandBuffer>& _Buffer, const Boolean& _IsSecondary)
		{
			return CreateRef<BeginBuffer>(_Buffer, _IsSecondary);
		}

		Ref<EndBuffer> EndBuffer::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<EndBuffer>(_Buffer);
		}

		Ref<CopyBuffer> CopyBuffer::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<CopyBuffer>(_Buffer);
		}

		Ref<CopyImage> CopyImage::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<CopyImage>(_Buffer);
		}

		Ref<BindBarrier> BindBarrier::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<BindBarrier>(_Buffer);
		}

		Ref<SetViewport> SetViewport::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<SetViewport>(_Buffer);
		}

		Ref<SetScissor> SetScissor::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<SetScissor>(_Buffer);
		}

		Ref<SetClearColor> SetClearColor::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<SetClearColor>(_Buffer);
		}

		Ref<BeginRenderpass> BeginRenderpass::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<BeginRenderpass>(_Buffer);
		}

		Ref<EndRenderpass> EndRenderpass::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<EndRenderpass>(_Buffer);
		}

		Ref<BindPipeline> BindPipeline::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<BindPipeline>(_Buffer);
		}

		Ref<DrawIndexed> DrawIndexed::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<DrawIndexed>(_Buffer);
		}

		Ref<BindVertexBuffer> BindVertexBuffer::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<BindVertexBuffer>(_Buffer);
		}

		Ref<BindIndexBuffer> BindIndexBuffer::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<BindIndexBuffer>(_Buffer);
		}

		Ref<BindDescriptorSet> BindDescriptorSet::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<BindDescriptorSet>(_Buffer);
		}

		Ref<ExecuteCommandBuffers> ExecuteCommandBuffers::Create(const Ref<VulkanCommandBuffer>& _Buffer)
		{
			return CreateRef<ExecuteCommandBuffers>(_Buffer);
		}

	}
}