#include "Morppch.h"

#include "Platform/Vulkan/VulkanCommand/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	VulkanCommandBuffer::VulkanCommandBuffer(const VkCommandBuffer& _Buffer)
		: m_Buffer(_Buffer)
	{
		VULKAN_CORE_WARN("[VULKAN] CommandBuffer Was Created!");
	}

	void VulkanCommandBuffer::ResetBuffer()
	{
		MORP_PROFILE_FUNCTION();

		VkCommandBufferResetFlags ResetFlags = {};
		ResetFlags = VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
		vkResetCommandBuffer(m_Buffer, ResetFlags);
		VULKAN_CORE_DEBUG_CMD("Reset", "");
	}

	void VulkanCommandBuffer::BeginSecondary(const VkRenderPass& _Renderpass)
	{
		MORP_PROFILE_FUNCTION();

		VkCommandBufferBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		VkCommandBufferInheritanceInfo InheritanceInfo = {};
		InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		InheritanceInfo.renderPass = _Renderpass;
		InheritanceInfo.framebuffer = VK_NULL_HANDLE;
		InheritanceInfo.subpass = 0;

		BeginInfo.pInheritanceInfo = &InheritanceInfo;
		vkBeginCommandBuffer(m_Buffer, &BeginInfo);
		VULKAN_CORE_DEBUG_CMD("BeginSecondary", "");
	}

	void VulkanCommandBuffer::BeginPrimary()
	{
		MORP_PROFILE_FUNCTION();

		VkCommandBufferBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		vkBeginCommandBuffer(m_Buffer, &BeginInfo);
		VULKAN_CORE_DEBUG_CMD("BeginPrimary", "");
	}

	void VulkanCommandBuffer::EndBuffer()
	{
		MORP_PROFILE_FUNCTION();

		vkEndCommandBuffer(m_Buffer);
		VULKAN_CORE_DEBUG_CMD("End", "");
	}

	void VulkanCommandBuffer::BeginQueue(const VkQueryPool& _Queue)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdBeginQuery(m_Buffer, _Queue, 0, 0);
	}

	void VulkanCommandBuffer::EndQueue(const VkQueryPool& _Queue)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdEndQuery(m_Buffer, _Queue, 0);
	}

	void VulkanCommandBuffer::WriteQueue(const VkQueryPool& _Queue)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdWriteTimestamp(m_Buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, _Queue, 0);
	}

	void VulkanCommandBuffer::CopyBuffer(const VkBuffer& _Source, const VkBuffer& _Destination, const Vector<VkBufferCopy>& _CopyRegion)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdCopyBuffer(m_Buffer, _Source, _Destination, _CopyRegion.size(), _CopyRegion.data());
		VULKAN_CORE_DEBUG_CMD("CopyBuffer", ToString(_CopyRegion.size()));
	}

	void VulkanCommandBuffer::CopyImage(const VkBuffer& _Source, const VkImage& _DestinationImage, const VkBufferImageCopy& _CopyRegion)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdCopyBufferToImage(m_Buffer, _Source, _DestinationImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &_CopyRegion);
		VULKAN_CORE_DEBUG_CMD("CopyImage", ToString(_CopyRegion.imageExtent.width / _CopyRegion.imageExtent.height));
	}

	void VulkanCommandBuffer::CopyImageToImage(const VkImage& _SourceImage, const VkImage& _DesinationImage, const VkImageCopy& _CopyRegion)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdCopyImage(m_Buffer, _SourceImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _DesinationImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &_CopyRegion);
		VULKAN_CORE_DEBUG_CMD("CopyImageToImage", ToString(_CopyRegion.imageExtent.width / _CopyRegion.imageExtent.height));
	}

	void VulkanCommandBuffer::BindBarrier(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdPipelineBarrier(m_Buffer, _SourceStage, _DestinationStage, 0, 0, nullptr, 0, nullptr, 1, &_Barrier);
		VULKAN_CORE_DEBUG_CMD("BindBarrier", "");
	}

	void VulkanCommandBuffer::SetViewport(const VkViewport& _Viewport)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdSetViewport(m_Buffer, 0, 1, &_Viewport);
		VULKAN_CORE_DEBUG_CMD("SetViewport", ToString(_Viewport.width / _Viewport.height));
	}

	void VulkanCommandBuffer::SetScissor(const VkRect2D& _Scissor)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdSetScissor(m_Buffer, 0, 1, &_Scissor);
		VULKAN_CORE_DEBUG_CMD("SetScissor", "");
	}

	void VulkanCommandBuffer::SetClearColor(const Vector4& _ClearColor)
	{
		MORP_PROFILE_FUNCTION();

		m_ClearColor = _ClearColor;
		VULKAN_CORE_DEBUG_CMD("SetClearColor", "");
	}

	void VulkanCommandBuffer::BeginRenderpass(const VkRenderPass& _Renderpass, const VkFramebuffer& _FrameBuffer, const VkRect2D& _Scissor)
	{
		MORP_PROFILE_FUNCTION();

		VkClearValue ColorValue = VkClearValue({ m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w });

		VkRenderPassBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		BeginInfo.renderPass = _Renderpass;
		BeginInfo.framebuffer = _FrameBuffer;
		BeginInfo.renderArea = _Scissor;
		BeginInfo.clearValueCount = 1;
		BeginInfo.pClearValues = &ColorValue;

		vkCmdBeginRenderPass(m_Buffer, &BeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		VULKAN_CORE_DEBUG_CMD("BeginRenderpass", "");
	}

	void VulkanCommandBuffer::EndRenderpass()
	{
		MORP_PROFILE_FUNCTION();

		vkCmdEndRenderPass(m_Buffer);
		VULKAN_CORE_DEBUG_CMD("EndRenderpass", "");
	}

	void VulkanCommandBuffer::BindPipeline(const VkPipeline& _Pipeline)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdBindPipeline(m_Buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _Pipeline);
		VULKAN_CORE_DEBUG_CMD("BindPipeline", "");
	}

	void VulkanCommandBuffer::PushConstant(const VkPipelineLayout& _Layout, const uint32& Size, const void* _Data)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdPushConstants(m_Buffer, _Layout, VK_SHADER_STAGE_VERTEX_BIT, 0, Size, _Data);
		VULKAN_CORE_DEBUG_CMD("PushConstant", "");
	}

	void VulkanCommandBuffer::DrawIndexed(const uint32& _IndexCount)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdDrawIndexed(m_Buffer, _IndexCount, 1, 0, 0, 0);
		VULKAN_CORE_DEBUG_CMD("DrawIndexed", ToString(_IndexCount));
	}

	void VulkanCommandBuffer::BindVertexBuffer(const VkBuffer& _VertexBuffer)
	{
		MORP_PROFILE_FUNCTION();

		VkDeviceSize Offsets = 0;
		vkCmdBindVertexBuffers(m_Buffer, 0, 1, &_VertexBuffer, &Offsets);
		VULKAN_CORE_DEBUG_CMD("BindVertexBuffer", "");
	}

	void VulkanCommandBuffer::BindIndexBuffer(const VkBuffer& _IndexBuffer)
	{
		MORP_PROFILE_FUNCTION();

		VkDeviceSize Offsets = 0;
		vkCmdBindIndexBuffer(m_Buffer, _IndexBuffer, Offsets, VkIndexType::VK_INDEX_TYPE_UINT32);
		VULKAN_CORE_DEBUG_CMD("BindIndexBuffer", "");
	}

	void VulkanCommandBuffer::BindDescriptorSet(const VkPipelineLayout& _Layout, const VkDescriptorSet& _DescriptorSet)
	{
		MORP_PROFILE_FUNCTION();

		vkCmdBindDescriptorSets(m_Buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, _Layout, 0, 1, &_DescriptorSet, 0, nullptr);
		VULKAN_CORE_DEBUG_CMD("BindDescriptorSet", "");
	}

	void VulkanCommandBuffer::ExecuteCommandBuffers(const Vector<VkCommandBuffer>& _Commands)
	{
		MORP_PROFILE_FUNCTION();

		if(_Commands.size() != 0)
			vkCmdExecuteCommands(m_Buffer, _Commands.size(), _Commands.data());
		VULKAN_CORE_DEBUG_CMD("ExecuteCommandBuffers", ToString(_Commands.size()));
	}
}}