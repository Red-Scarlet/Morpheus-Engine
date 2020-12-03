#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

namespace Morpheus {

	class VulkanCommand
	{
	public:
		virtual void Execute() = 0;
		virtual const VkCommandBuffer& Retrieve() const = 0;
	};

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(const VkCommandBuffer& _Buffer);
		virtual ~VulkanCommandBuffer() = default;

		const VkCommandBuffer& GetBuffer() const
		{ return m_Buffer; }

	public:
		void ResetBuffer();
		void BeginSecondary(const VkRenderPass& _Renderpass);
		void BeginPrimary();
		void EndBuffer();

		void CopyBuffer(const VkBuffer& _Source, const VkBuffer& _Destination, const Vector<VkBufferCopy>& _CopyRegion);
		void CopyImage(const VkBuffer& _Source, const VkImage& _DestinationImage, const VkBufferImageCopy& _CopyRegion);
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
		Ref<VulkanDevice> m_Device;
		VkCommandBuffer m_Buffer;

		// Non-Command Data
		Vector4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };

	public:
		static Ref<VulkanCommandBuffer> Make(const VkCommandBuffer& _Buffer);

	};


}