#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanResources/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"

namespace Morpheus {

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

		uint32 FBOINDEX = 0;

		Vector<uint32> VertexArrays;

		vk::Buffer SourceBuffer;
		vk::Buffer DestinationBuffer;
		Vector<vk::BufferCopy> CopyRegion;
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

		void Copy(const vk::Buffer& _SourceBuffer, const vk::Buffer& _DestinationBuffer, const Vector<vk::BufferCopy>& _CopyRegion);
		void Function(ExecuteCommand _Command);

		void SetViewport(const vk::Viewport& _Viewport);
		void SetScissor(const vk::Rect2D& _Scissor);
		void SetClearColor(const Vector4& _Clearcolor);

		void BindFramebuffer(const Ref<VulkanFramebuffer>& _Framebuffer, const uint32& _Index);
		void BeginRenderpass(const Ref<VulkanRenderpass>& _Renderpass);
		void EndRenderpass();

		void BindPipeline(const Ref<VulkanPipeline>& _Pipeline);

		void SubmitVertexArray(const Ref<VulkanVertexArray>& _VertexArray);
		void DrawIndexed();

		void Compile(bool ClearFlag = true);

		const VulkanCommandData& GetCommandData();

	private:
		Vector<ExecuteCommand> m_ExecutionStack;
		VulkanCommandData m_DefaultData;
		vk::CommandBuffer m_Buffer;

		bool m_RecompileFlag = false;
	};

}