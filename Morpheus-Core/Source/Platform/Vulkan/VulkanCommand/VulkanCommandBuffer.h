#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

namespace Morpheus { namespace Vulkan {

	class VulkanCommandBuffer
	{
	public:
		friend class VulkanExecutionStack;
		friend class CommandExecuteCommandBuffers;

	public:
		VulkanCommandBuffer(const VkCommandBuffer& _Buffer);
		virtual ~VulkanCommandBuffer() = default;

	public:
		void ResetBuffer();
		void BeginSecondary(const VkRenderPass& _Renderpass);
		void BeginPrimary();
		void EndBuffer();

		void BeginQueue(const VkQueryPool& _Queue);
		void EndQueue(const VkQueryPool& _Queue);
		void WriteQueue(const VkQueryPool& _Queue);

		void CopyBuffer(const VkBuffer& _Source, const VkBuffer& _Destination, const Vector<VkBufferCopy>& _CopyRegion);
		void CopyImage(const VkBuffer& _Source, const VkImage& _DestinationImage, const VkBufferImageCopy& _CopyRegion);
		void CopyImageToImage(const VkImage& _SourceImage, const VkImage& _DesinationImage, const VkImageCopy& _CopyRegion);
		void BindBarrier(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage);

		void SetViewport(const VkViewport& _Viewport);
		void SetScissor(const VkRect2D& _Scissor);
		void SetClearColor(const Vector4& _ClearColor);

		void BeginRenderpass(const VkRenderPass& _Renderpass, const VkFramebuffer& _FrameBuffer, const VkRect2D& _Scissor);
		void EndRenderpass();

		void BindPipeline(const VkPipeline& _Pipeline);
		void PushConstant(const VkPipelineLayout& _Layout, const uint32& Size, const void* _Data);

		void DrawIndexed(const uint32& _IndexCount); // VBO, IBO, DESCRIPTORSET, PIPELINELAYOUT;
		void BindVertexBuffer(const VkBuffer& _VertexBuffer);
		void BindIndexBuffer(const VkBuffer& _IndexBuffer);
		void BindDescriptorSet(const VkPipelineLayout& _Layout, const VkDescriptorSet& _DescriptorSet);

		void ExecuteCommandBuffers(const Vector<VkCommandBuffer>& _Commands);

	private:
		VkCommandBuffer m_Buffer;

		// Non-Command Data
		Vector4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	class VulkanInstruction
	{
	public:
		VulkanInstruction() = default;
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) = 0;
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const = 0;
	};

}}