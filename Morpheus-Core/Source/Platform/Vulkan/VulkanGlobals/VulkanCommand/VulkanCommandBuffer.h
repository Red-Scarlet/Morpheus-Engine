#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"

namespace Morpheus {

	// TODO: Add Messaging System and optimizations
	// TODO: Allow For multi shader and multiframebuffer bindings and split vaos

	typedef Vector<vk::CommandBuffer> VulkanCommands;

	struct VulkanCommandData
	{
	public:
		Vector4 ClearColor;

		vk::RenderPass Renderpass;
		vk::Framebuffer Framebuffer;

		vk::Viewport Viewport;
		vk::Rect2D Scissor;

		vk::Pipeline Pipeline;
		vk::PipelineLayout PipelineLayout;

		Vector<vk::DescriptorSet> DescriptorSets;
		Vector<uint32> VertexArrays;

		vk::Buffer SourceBuffer;
		vk::Buffer DestinationBuffer;
		Vector<vk::BufferCopy> CopyRegion;

		VkImage DestinationImage;
		VkBufferImageCopy ImageRegion;

		VkImageMemoryBarrier Barrier;
		VkPipelineStageFlags SourceStage;
		VkPipelineStageFlags DestinationStage;

		bool RenderpassCheck = false;
		bool FramebufferCheck = false;	// For multiframebuffer usage
		bool ShaderCheck = false;		// For multishader usage
	};

	typedef Function<void(vk::CommandBuffer, VulkanCommandData)> ExecuteCommand;

	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(const vk::CommandBuffer& _Buffer);
		virtual ~VulkanCommandBuffer() = default;

		void ResetBuffer();
		void BeginBuffer();
		void EndBuffer();

		void CopyBuffer(const vk::Buffer& _SourceBuffer, const vk::Buffer& _DestinationBuffer, const Vector<vk::BufferCopy>& _CopyRegion);
		void CopyImage(const VkBuffer& _SourceBuffer, const VkImage& _DestinationImage, const VkBufferImageCopy& _CopyRegion);
		void BindBarrier(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage);

		// Change to be built into the framebuffer
		void SetClearColor(const Vector4& _Clearcolor);
		void SetViewport(const vk::Viewport& _Viewport);
		void SetScissor(const vk::Rect2D& _Scissor);

		void BindFramebuffer(const Ref<VulkanFrameBuffer>& _Framebuffer, const uint32& _Index);
		void BindShader(const Ref<VulkanShader>& _Shader);

		void SubmitVertexArray(const uint32& _VertexArray);
		void DrawIndexed();

		void Compile(bool ClearFlag = true);

	private:
		Vector<ExecuteCommand> m_ExecutionStack;
		VulkanCommandData m_DefaultData;
		vk::CommandBuffer m_Buffer;

		bool m_RecompileFlag = false;
	};

}