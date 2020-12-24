#include "Morppch.h"
#include "VulkanInstructions.h"

namespace Morpheus { namespace Vulkan {

	Ref<CommandResetBuffer> CommandResetBuffer::Create()
	{
		return CreateRef<CommandResetBuffer>();
	}

	Ref<CommandBeginBuffer> CommandBeginBuffer::Create(const Boolean& _IsSecondary)
	{
		return CreateRef<CommandBeginBuffer>(_IsSecondary);
	}

	Ref<CommandEndBuffer> CommandEndBuffer::Create()
	{
		return CreateRef<CommandEndBuffer>();
	}

	Ref<CommandCopyBufferToBuffer> CommandCopyBufferToBuffer::Create()
	{
		return CreateRef<CommandCopyBufferToBuffer>();
	}

	Ref<CommandCopyBufferToImage> CommandCopyBufferToImage::Create()
	{
		return CreateRef<CommandCopyBufferToImage>();
	}

	Ref<CommandCopyImageToImage> CommandCopyImageToImage::Create()
	{
		return CreateRef<CommandCopyImageToImage>();
	}

	Ref<CommandBindBarrier> CommandBindBarrier::Create()
	{
		return CreateRef<CommandBindBarrier>();
	}

	Ref<CommandSetViewport> CommandSetViewport::Create()
	{
		return CreateRef<CommandSetViewport>();
	}

	Ref<CommandSetScissor> CommandSetScissor::Create()
	{
		return CreateRef<CommandSetScissor>();
	}

	Ref<CommandSetClearColor> CommandSetClearColor::Create()
	{
		return CreateRef<CommandSetClearColor>();
	}

	Ref<CommandBeginRenderpass> CommandBeginRenderpass::Create()
	{
		return CreateRef<CommandBeginRenderpass>();
	}

	Ref<CommandEndRenderpass> CommandEndRenderpass::Create()
	{
		return CreateRef<CommandEndRenderpass>();
	}

	Ref<CommandBindPipeline> CommandBindPipeline::Create()
	{
		return CreateRef<CommandBindPipeline>();
	}

	Ref<CommandDrawIndexed> CommandDrawIndexed::Create()
	{
		return CreateRef<CommandDrawIndexed>();
	}

	Ref<CommandBindVertexBuffer> CommandBindVertexBuffer::Create()
	{
		return CreateRef<CommandBindVertexBuffer>();
	}

	Ref<CommandBindIndexBuffer> CommandBindIndexBuffer::Create()
	{
		return CreateRef<CommandBindIndexBuffer>();
	}

	Ref<CommandBindDescriptorSet> CommandBindDescriptorSet::Create()
	{
		return CreateRef<CommandBindDescriptorSet>();
	}

	Ref<CommandExecuteCommandBuffers> CommandExecuteCommandBuffers::Create()
	{
		return CreateRef<CommandExecuteCommandBuffers>();
	}

}}