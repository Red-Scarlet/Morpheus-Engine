#include "Morppch.h"
#include "VulkanCommand.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanCommandBuffer::VulkanCommandBuffer(const VkCommandBuffer& _Buffer)
		: m_Buffer(_Buffer)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		VULKAN_CORE_WARN("[VULKAN] CommandBuffer Was Created!");
	}

	void VulkanCommandBuffer::ResetBuffer()
	{
		VkCommandBufferResetFlags ResetFlags = {};
		vkResetCommandBuffer(m_Buffer, ResetFlags);
	}

	void VulkanCommandBuffer::BeginSecondary(const VkRenderPass& _Renderpass)
	{
		VkCommandBufferBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

		VkCommandBufferInheritanceInfo InheritanceInfo = {};
		InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		InheritanceInfo.renderPass = _Renderpass;
		InheritanceInfo.framebuffer = VK_NULL_HANDLE;
		InheritanceInfo.subpass = 0;

		BeginInfo.pInheritanceInfo = &InheritanceInfo;
		vkBeginCommandBuffer(m_Buffer, &BeginInfo);
	}

	void VulkanCommandBuffer::BeginPrimary()
	{
		VkCommandBufferBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(m_Buffer, &BeginInfo);
	}

	void VulkanCommandBuffer::EndBuffer()
	{
		vkEndCommandBuffer(m_Buffer);
	}

	void VulkanCommandBuffer::CopyBuffer(const VkBuffer& _Source, const VkBuffer& _Destination, const Vector<VkBufferCopy>& _CopyRegion)
	{
		vkCmdCopyBuffer(m_Buffer, _Source, _Destination, _CopyRegion.size(), _CopyRegion.data());
	}

	void VulkanCommandBuffer::CopyImage(const VkBuffer& _Source, const VkImage& _DestinationImage, const VkBufferImageCopy& _CopyRegion)
	{
		vkCmdCopyBufferToImage(m_Buffer, _Source, _DestinationImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &_CopyRegion);
	}

	void VulkanCommandBuffer::BindBarrier(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage)
	{
		vkCmdPipelineBarrier(m_Buffer, _SourceStage, _DestinationStage, 0, 0, nullptr, 0, nullptr, 1, &_Barrier);
	}

	void VulkanCommandBuffer::SetViewport(const VkViewport& _Viewport)
	{
		vkCmdSetViewport(m_Buffer, 0, 1, &_Viewport);
	}

	void VulkanCommandBuffer::SetScissor(const VkRect2D& _Scissor)
	{
		vkCmdSetScissor(m_Buffer, 0, 1, &_Scissor);
	}

	void VulkanCommandBuffer::SetClearColor(const Vector4& _ClearColor)
	{
		m_ClearColor = _ClearColor;
	}

	void VulkanCommandBuffer::BeginRenderpass(const VkRenderPass& _Renderpass, const VkFramebuffer& _FrameBuffer, const VkRect2D& _Scissor)
	{
		VkClearValue ColorValue = VkClearValue({ m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w });

		VkRenderPassBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		BeginInfo.renderPass = _Renderpass;
		BeginInfo.framebuffer = _FrameBuffer;
		BeginInfo.renderArea = _Scissor;
		BeginInfo.clearValueCount = 1;
		BeginInfo.pClearValues = &ColorValue;

		vkCmdBeginRenderPass(m_Buffer, &BeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	}

	void VulkanCommandBuffer::EndRenderpass()
	{
		vkCmdEndRenderPass(m_Buffer);
	}

	void VulkanCommandBuffer::BindPipeline(const VkPipeline& _Pipeline)
	{
		vkCmdBindPipeline(m_Buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _Pipeline);
	}

	void VulkanCommandBuffer::PushConstant(const VkPipelineLayout& _Layout, const uint32& Size, const void* _Data)
	{
		// PipelineLayout, Size(DataMessage), Data(DataMessage)
		vkCmdPushConstants(m_Buffer, _Layout, VK_SHADER_STAGE_VERTEX_BIT, 0, Size, _Data);
	}

	void VulkanCommandBuffer::DrawIndexed(const uint32& _IndexCount)
	{
		vkCmdDrawIndexed(m_Buffer, _IndexCount, 1, 0, 0, 1);
	}

	void VulkanCommandBuffer::BindVertexBuffer(const VkBuffer& _VertexBuffer)
	{
		VkDeviceSize Offsets = 0;
		vkCmdBindVertexBuffers(m_Buffer, 0, 1, &_VertexBuffer, &Offsets);
	}

	void VulkanCommandBuffer::BindIndexBuffer(const VkBuffer& _IndexBuffer)
	{
		VkDeviceSize Offsets = 0;
		vkCmdBindIndexBuffer(m_Buffer, _IndexBuffer, Offsets, VkIndexType::VK_INDEX_TYPE_UINT32);
	}

	void VulkanCommandBuffer::BindDescriptorSet(const VkPipelineLayout& _Layout, const VkDescriptorSet& _DescriptorSet)
	{
		vkCmdBindDescriptorSets(m_Buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, _Layout, 0, 1, &_DescriptorSet, 0, nullptr);
	}

	void VulkanCommandBuffer::ExecuteCommandBuffers(const Vector<VkCommandBuffer>& _Commands)
	{
		if(_Commands.size() != 0)
			vkCmdExecuteCommands(m_Buffer, _Commands.size(), _Commands.data());
	}

	Ref<VulkanCommandBuffer> VulkanCommandBuffer::Make(const VkCommandBuffer& _Buffer)
	{
		return CreateRef<VulkanCommandBuffer>(_Buffer);
	}

}