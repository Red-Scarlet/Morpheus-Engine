#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanCore/VulkanBuffer.h"

namespace Morpheus { namespace Vulkan {

	enum class VulkanCommandTypes : uint8
	{
		ResetBuffer, BeginBuffer, EndBuffer, BeginQuery, EndQuery, WriteQuery,
		CopyBufferToBuffer, CopyBufferToImage, CopyImageToImage, BindBarrier,
		SetViewport, SetScissor, SetClearColor,
		BeginRenderpass, EndRenderpass, BindPipeline,
		DrawIndexed, BindVertexBuffer, BindIndexBuffer, BindDescriptorSet,
		ExecuteCommandBuffers
	};

	class CommandResetBuffer : public VulkanInstruction
	{
	public:
		CommandResetBuffer() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer;  m_Buffer->ResetBuffer(); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;

	public:
		static Ref<CommandResetBuffer> Create();
	};

	class CommandBeginBuffer : public VulkanInstruction
	{

	public:
		CommandBeginBuffer(const Boolean& _IsSecondary) : m_Secondary(_IsSecondary) {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; if(m_Secondary) m_Buffer->BeginSecondary(m_Renderpass); else m_Buffer->BeginPrimary(); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkRenderPass& _Renderpass)
		{ m_Renderpass = _Renderpass; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkRenderPass m_Renderpass;
		Boolean m_Secondary = false;

	public:
		static Ref<CommandBeginBuffer> Create(const Boolean& _IsSecondary = false);
	};

	class CommandEndBuffer : public VulkanInstruction
	{
	public:
		CommandEndBuffer() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->EndBuffer(); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;

	public:
		static Ref<CommandEndBuffer> Create();
	};

	class CommandCopyBufferToBuffer : public VulkanInstruction
	{
	public:
		CommandCopyBufferToBuffer() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->CopyBuffer(m_SourceBuffer->GetBuffer(), m_DestinationBuffer->GetBuffer(), m_CopyRegion); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const Ref<VulkanBuffer>& _Source, const Ref<VulkanBuffer>& _Destination, const Vector<VkBufferCopy>& _CopyRegion)
		{ m_SourceBuffer = _Source; m_DestinationBuffer = _Destination; m_CopyRegion = _CopyRegion; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		Ref<VulkanBuffer> m_SourceBuffer;
		Ref<VulkanBuffer> m_DestinationBuffer;
		Vector<VkBufferCopy> m_CopyRegion;

	public:
		static Ref<CommandCopyBufferToBuffer> Create();

	};

	class CommandCopyBufferToImage : public VulkanInstruction
	{
	public:
		CommandCopyBufferToImage() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->CopyImage(m_SourceBuffer, m_DestinationBuffer, m_CopyRegion); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkBuffer& _Source, const VkImage& _Destination, const VkBufferImageCopy& _CopyRegion)
		{ m_SourceBuffer = _Source; m_DestinationBuffer = _Destination; m_CopyRegion = _CopyRegion; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkBuffer m_SourceBuffer;
		VkImage m_DestinationBuffer;
		VkBufferImageCopy m_CopyRegion;

	public:
		static Ref<CommandCopyBufferToImage> Create();

	};

	class CommandCopyImageToImage : public VulkanInstruction
	{
	public:
		CommandCopyImageToImage() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->CopyImageToImage(m_Source, m_Destination, m_Region); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkImage& _Source, const VkImage& _Destination, const VkImageCopy& _Region)
		{ m_Source = _Source; m_Destination = _Destination; m_Region = _Region; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkImage m_Source;
		VkImage m_Destination;
		VkImageCopy m_Region;

	public:
		static Ref<CommandCopyImageToImage> Create();

	};

	class CommandBindBarrier : public VulkanInstruction
	{
	public:
		CommandBindBarrier() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->BindBarrier(m_Barrier, m_SourceStage, m_DestinationStage); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage)
		{ m_Barrier = _Barrier; m_SourceStage = _SourceStage; m_DestinationStage = _DestinationStage; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkImageMemoryBarrier m_Barrier;
		VkPipelineStageFlags m_SourceStage;
		VkPipelineStageFlags m_DestinationStage;

	public:
		static Ref<CommandBindBarrier> Create();

	};

	class CommandSetViewport : public VulkanInstruction
	{
	public:
		CommandSetViewport() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->SetViewport(m_Viewport); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkViewport& _Viewport)
		{ m_Viewport = _Viewport; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkViewport m_Viewport;

	public:
		static Ref<CommandSetViewport> Create();

	};

	class CommandSetScissor : public VulkanInstruction
	{
	public:
		CommandSetScissor() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->SetScissor(m_Scissor); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkRect2D& _Scissor)
		{ m_Scissor = _Scissor; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkRect2D m_Scissor;

	public:
		static Ref<CommandSetScissor> Create();

	};

	class CommandSetClearColor : public VulkanInstruction
	{
	public:
		CommandSetClearColor() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->SetClearColor(m_ClearColor); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const Vector4& _ClearColor)
		{ m_ClearColor = _ClearColor; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		Vector4 m_ClearColor;

	public:
		static Ref<CommandSetClearColor> Create();

	};

	class CommandBeginRenderpass : public VulkanInstruction
	{
	public:
		CommandBeginRenderpass() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->BeginRenderpass(m_Renderpass, m_FrameBuffer, m_Scissor); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkRenderPass& _Renderpass, const VkFramebuffer& _FrameBuffer, const VkRect2D& _Scissor)
		{ m_Renderpass = _Renderpass; m_FrameBuffer = _FrameBuffer, m_Scissor = _Scissor; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkRenderPass m_Renderpass;
		VkFramebuffer m_FrameBuffer;
		VkRect2D m_Scissor;

	public:
		static Ref<CommandBeginRenderpass> Create();

	};

	class CommandEndRenderpass : public VulkanInstruction
	{
	public:
		CommandEndRenderpass() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->EndRenderpass(); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;

	public:
		static Ref<CommandEndRenderpass> Create();

	};

	class CommandBindPipeline : public VulkanInstruction
	{
	public:
		CommandBindPipeline() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->BindPipeline(m_Pipeline); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkPipeline& _Pipeline)
		{ m_Pipeline = _Pipeline; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkPipeline m_Pipeline;

	public:
		static Ref<CommandBindPipeline> Create();
	};

	class CommandDrawIndexed : public VulkanInstruction
	{
	public:
		CommandDrawIndexed() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->DrawIndexed(m_IndexCount); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const uint32& _IndexCount)
		{ m_IndexCount  = _IndexCount; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		uint32 m_IndexCount = 0;

	public:
		static Ref<CommandDrawIndexed> Create();
	};

	class CommandBindVertexBuffer : public VulkanInstruction
	{
	public:
		CommandBindVertexBuffer() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->BindVertexBuffer(m_VertexBuffer); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkBuffer& _VertexBuffer)
		{ m_VertexBuffer  = _VertexBuffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkBuffer m_VertexBuffer;

	public:
		static Ref<CommandBindVertexBuffer> Create();
	};

	class CommandBindIndexBuffer : public VulkanInstruction
	{
	public:
		CommandBindIndexBuffer() = default;
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->BindIndexBuffer(m_IndexBuffer); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkBuffer& _IndexBuffer)
		{ m_IndexBuffer  = _IndexBuffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkBuffer m_IndexBuffer;

	public:
		static Ref<CommandBindIndexBuffer> Create();
	};

	class CommandBindDescriptorSet : public VulkanInstruction
	{
	public:
		CommandBindDescriptorSet() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->BindDescriptorSet(m_Layout, m_DescriptorSet); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkPipelineLayout& _Layout, const VkDescriptorSet& _DescriptorSet)
		{ m_Layout = _Layout; m_DescriptorSet = _DescriptorSet; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkPipelineLayout m_Layout;
		VkDescriptorSet m_DescriptorSet;

	public:
		static Ref<CommandBindDescriptorSet> Create();
	};

	class CommandExecuteCommandBuffers : public VulkanInstruction
	{
	public:
		CommandExecuteCommandBuffers() {}
		virtual void Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
		{ m_Buffer = _Buffer; m_Buffer->ExecuteCommandBuffers(m_CommandBuffers); }
		virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
		{ return m_Buffer; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const Vector<VkCommandBuffer>& _Commands)
		{ m_CommandBuffers = _Commands; }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkCommandBuffer& _Command)
		{ m_CommandBuffers.push_back(_Command); }

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const Ref<VulkanCommandBuffer>& _Command)
		{ m_CommandBuffers.push_back(_Command->m_Buffer); }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		Vector<VkCommandBuffer> m_CommandBuffers;

	public:
		static Ref<CommandExecuteCommandBuffers> Create();
	};

}}