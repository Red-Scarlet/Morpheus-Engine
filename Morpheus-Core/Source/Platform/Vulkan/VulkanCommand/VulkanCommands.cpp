#include "Morppch.h"
#include "VulkanCommands.h"

namespace Morpheus { namespace Vulkan {

	bool CommandResetBuffer::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandResetBuffer> CommandResetBuffer::Create()
	{
		return CreateRef<CommandResetBuffer>();
	}

	bool CommandBeginBuffer::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBeginBuffer> CommandBeginBuffer::Create(const bool& _IsSecondary)
	{
		return CreateRef<CommandBeginBuffer>(_IsSecondary);
	}

	bool CommandEndBuffer::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandEndBuffer> CommandEndBuffer::Create()
	{
		return CreateRef<CommandEndBuffer>();
	}

	//Ref<CommandCopyBufferToBuffer> CommandCopyBufferToBuffer::Create()
	//{
	//	return CreateRef<CommandCopyBufferToBuffer>();
	//}

	bool CommandCopyBufferToImage::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandCopyBufferToImage> CommandCopyBufferToImage::Create()
	{
		return CreateRef<CommandCopyBufferToImage>();
	}

	bool CommandCopyImageToImage::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandCopyImageToImage> CommandCopyImageToImage::Create()
	{
		return CreateRef<CommandCopyImageToImage>();
	}

	bool CommandBindBarrier::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBindBarrier> CommandBindBarrier::Create()
	{
		return CreateRef<CommandBindBarrier>();
	}

	bool CommandSetViewport::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandSetViewport> CommandSetViewport::Create()
	{
		return CreateRef<CommandSetViewport>();
	}

	bool CommandSetScissor::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandSetScissor> CommandSetScissor::Create()
	{
		return CreateRef<CommandSetScissor>();
	}

	bool CommandBeginRenderpass::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBeginRenderpass> CommandBeginRenderpass::Create()
	{
		return CreateRef<CommandBeginRenderpass>();
	}

	bool CommandEndRenderpass::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandEndRenderpass> CommandEndRenderpass::Create()
	{
		return CreateRef<CommandEndRenderpass>();
	}

	bool CommandBindPipeline::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBindPipeline> CommandBindPipeline::Create()
	{
		return CreateRef<CommandBindPipeline>();
	}

	bool CommandDrawIndexed::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandDrawIndexed> CommandDrawIndexed::Create()
	{
		return CreateRef<CommandDrawIndexed>();
	}

	bool CommandBindVertexBuffer::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBindVertexBuffer> CommandBindVertexBuffer::Create()
	{
		return CreateRef<CommandBindVertexBuffer>();
	}

	bool CommandBindIndexBuffer::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBindIndexBuffer> CommandBindIndexBuffer::Create()
	{
		return CreateRef<CommandBindIndexBuffer>();
	}

	bool CommandBindDescriptorSet::Execute(const Ref<VulkanCommandBuffer>& _Buffer)
	{
		return false;
	}

	Ref<CommandBindDescriptorSet> CommandBindDescriptorSet::Create()
	{
		return CreateRef<CommandBindDescriptorSet>();
	}

	//Ref<CommandExecuteCommandBuffers> CommandExecuteCommandBuffers::Create()
	//{
	//	return CreateRef<CommandExecuteCommandBuffers>();
	//}

}}