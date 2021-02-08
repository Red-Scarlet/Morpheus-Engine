#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandBuffer.h"

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

	class CommandResetBuffer : public IVulkanCommand
	{
	public:
		CommandResetBuffer() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

	private:
		Ref<VulkanCommandBuffer> m_Buffer;

	public:
		static Ref<CommandResetBuffer> Create();
	};

	class CommandBeginBuffer : public IVulkanCommand
	{

	public:
		CommandBeginBuffer(const bool& _IsSecondary) : m_Secondary(_IsSecondary) {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkRenderPass& _Renderpass)
		{ m_Renderpass = _Renderpass; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkRenderPass m_Renderpass;
		bool m_Secondary = false;

	public:
		static Ref<CommandBeginBuffer> Create(const bool& _IsSecondary = false);
	};

	class CommandEndBuffer : public IVulkanCommand
	{
	public:
		CommandEndBuffer() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

	private:
		Ref<VulkanCommandBuffer> m_Buffer;

	public:
		static Ref<CommandEndBuffer> Create();
	};

	//class CommandCopyBufferToBuffer : public IVulkanCommand
	//{
	//public:
	//	CommandCopyBufferToBuffer() {}
	//	virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override
	//	{ m_Buffer = _Buffer; m_Buffer->CopyBuffer(m_SourceBuffer->GetData().Buffer, m_DestinationBuffer->GetData().Buffer, m_CopyRegion); }
	//	virtual const Ref<VulkanCommandBuffer>& Retrieve() const override
	//	
	//
	//	// TODO: Improve with a DataMessage Protocal
	//	void PopulateData(const Ref<VulkanBuffer>& _Source, const Ref<VulkanBuffer>& _Destination, const Vector<VkBufferCopy>& _CopyRegion)
	//	{ m_SourceBuffer = _Source; m_DestinationBuffer = _Destination; m_CopyRegion = _CopyRegion; }
	//
	//private:
	//	Ref<VulkanCommandBuffer> m_Buffer;
	//	Ref<VulkanBuffer> m_SourceBuffer;
	//	Ref<VulkanBuffer> m_DestinationBuffer;
	//	Vector<VkBufferCopy> m_CopyRegion;
	//
	//public:
	//	static Ref<CommandCopyBufferToBuffer> Create();
	//
	//};

	class CommandCopyBufferToImage : public IVulkanCommand
	{
	public:
		CommandCopyBufferToImage() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

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

	class CommandCopyImageToImage : public IVulkanCommand
	{
	public:
		CommandCopyImageToImage() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

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

	class CommandBindBarrier : public IVulkanCommand
	{
	public:
		CommandBindBarrier() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

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

	class CommandSetViewport : public IVulkanCommand
	{
	public:
		CommandSetViewport() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkViewport& _Viewport)
		{ m_Viewport = _Viewport; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkViewport m_Viewport;

	public:
		static Ref<CommandSetViewport> Create();
	};

	class CommandSetScissor : public IVulkanCommand
	{
	public:
		CommandSetScissor() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkRect2D& _Scissor)
		{ m_Scissor = _Scissor; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkRect2D m_Scissor;

	public:
		static Ref<CommandSetScissor> Create();
	};

	class CommandBeginRenderpass : public IVulkanCommand
	{
	public:
		CommandBeginRenderpass() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

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

	class CommandEndRenderpass : public IVulkanCommand
	{
	public:
		CommandEndRenderpass() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

	private:
		Ref<VulkanCommandBuffer> m_Buffer;

	public:
		static Ref<CommandEndRenderpass> Create();

	};

	class CommandBindPipeline : public IVulkanCommand
	{
	public:
		CommandBindPipeline() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkPipeline& _Pipeline)
		{ m_Pipeline = _Pipeline; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkPipeline m_Pipeline;

	public:
		static Ref<CommandBindPipeline> Create();
	};

	class CommandDrawIndexed : public IVulkanCommand
	{
	public:
		CommandDrawIndexed() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const uint32& _IndexCount)
		{ m_IndexCount  = _IndexCount; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		uint32 m_IndexCount = 0;

	public:
		static Ref<CommandDrawIndexed> Create();
	};

	class CommandBindVertexBuffer : public IVulkanCommand
	{
	public:
		CommandBindVertexBuffer() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkBuffer& _VertexBuffer)
		{ m_VertexBuffer  = _VertexBuffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkBuffer m_VertexBuffer;

	public:
		static Ref<CommandBindVertexBuffer> Create();
	};

	class CommandBindIndexBuffer : public IVulkanCommand
	{
	public:
		CommandBindIndexBuffer() = default;
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

		// TODO: Improve with a DataMessage Protocal
		void PopulateData(const VkBuffer& _IndexBuffer)
		{ m_IndexBuffer  = _IndexBuffer; }

	private:
		Ref<VulkanCommandBuffer> m_Buffer;
		VkBuffer m_IndexBuffer;

	public:
		static Ref<CommandBindIndexBuffer> Create();
	};

	class CommandBindDescriptorSet : public IVulkanCommand
	{
	public:
		CommandBindDescriptorSet() {}
		virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;

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

	//class CommandExecuteCommandBuffers : public IVulkanCommand
	//{
	//public:
	//	CommandExecuteCommandBuffers() {}
	//	virtual bool Execute(const Ref<VulkanCommandBuffer>& _Buffer) override;
	//
	//	// TODO: Improve with a DataMessage Protocal
	//	void PopulateData(const Vector<VkCommandBuffer>& _Commands)
	//	{ m_CommandBuffers = _Commands; }
	//
	//	// TODO: Improve with a DataMessage Protocal
	//	void PopulateData(const VkCommandBuffer& _Command)
	//	{ m_CommandBuffers.push_back(_Command); }
	//
	//private:
	//	Ref<VulkanCommandBuffer> m_Buffer;
	//	Vector<VkCommandBuffer> m_CommandBuffers;
	//
	//public:
	//	static Ref<CommandExecuteCommandBuffers> Create();
	//};

}}