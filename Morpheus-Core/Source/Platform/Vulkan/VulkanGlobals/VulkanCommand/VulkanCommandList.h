#pragma once

#include "Morpheus/Core/Common.h"
#include "VulkanCommand.h"

namespace Morpheus {

	namespace VulkanCommandList {

		class ResetBuffer : public VulkanCommand
		{
		public:
			ResetBuffer(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->ResetBuffer(); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

		public:
			static Ref<ResetBuffer> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BeginBuffer : public VulkanCommand
		{
		public:
			BeginBuffer(const Ref<VulkanCommandBuffer>& _Buffer, const Boolean& _IsSecondary) : m_Buffer(_Buffer), m_Secondary(_IsSecondary) {}
			virtual void Execute() override
			{ if (m_Secondary) m_Buffer->BeginSecondary(m_Renderpass); else m_Buffer->BeginPrimary(); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkRenderPass& _Renderpass)
			{ m_Renderpass = _Renderpass; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			
			VkRenderPass m_Renderpass;
			Boolean m_Secondary;


		public:
			static Ref<BeginBuffer> Create(const Ref<VulkanCommandBuffer>& _Buffer, const Boolean& _IsSecondary = false);
		};

		class EndBuffer : public VulkanCommand
		{
		public:
			EndBuffer(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->EndBuffer(); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

		public:
			static Ref<EndBuffer> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class CopyBuffer : public VulkanCommand
		{
		public:
			CopyBuffer(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->CopyBuffer(m_SourceBuffer, m_DestinationBuffer, m_CopyRegion); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkBuffer& _Source, const VkBuffer& _Destination, const Vector<VkBufferCopy>& _CopyRegion)
			{ m_SourceBuffer = _Source; m_DestinationBuffer = _Destination; m_CopyRegion = _CopyRegion; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

			VkBuffer m_SourceBuffer;
			VkBuffer m_DestinationBuffer;
			Vector<VkBufferCopy> m_CopyRegion;

		public:
			static Ref<CopyBuffer> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class CopyImage : public VulkanCommand
		{
		public:
			CopyImage(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->CopyImage(m_SourceBuffer, m_DestinationBuffer, m_CopyRegion); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkBuffer& _Source, const VkImage& _Destination, const VkBufferImageCopy& _CopyRegion)
			{ m_SourceBuffer = _Source; m_DestinationBuffer = _Destination; m_CopyRegion = _CopyRegion; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

			VkBuffer m_SourceBuffer;
			VkImage m_DestinationBuffer;
			VkBufferImageCopy m_CopyRegion;

		public:
			static Ref<CopyImage> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BindBarrier : public VulkanCommand
		{
		public:
			BindBarrier(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->BindBarrier(m_Barrier, m_SourceStage, m_DestinationStage); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage)
			{ m_Barrier = _Barrier; m_SourceStage = _SourceStage; m_DestinationStage = _DestinationStage; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

			VkImageMemoryBarrier m_Barrier;
			VkPipelineStageFlags m_SourceStage;
			VkPipelineStageFlags m_DestinationStage;

		public:
			static Ref<BindBarrier> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class SetViewport : public VulkanCommand
		{
		public:
			SetViewport(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->SetViewport(m_Viewport); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkViewport& _Viewport)
			{ m_Viewport = _Viewport; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			VkViewport m_Viewport;

		public:
			static Ref<SetViewport> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class SetScissor : public VulkanCommand
		{
		public:
			SetScissor(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->SetScissor(m_Scissor); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkRect2D& _Scissor)
			{ m_Scissor = _Scissor; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			VkRect2D m_Scissor;

		public:
			static Ref<SetScissor> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class SetClearColor : public VulkanCommand
		{
		public:
			SetClearColor(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->SetClearColor(m_ClearColor); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const Vector4& _ClearColor)
			{ m_ClearColor = _ClearColor; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			Vector4 m_ClearColor;

		public:
			static Ref<SetClearColor> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BeginRenderpass : public VulkanCommand
		{
		public:
			BeginRenderpass(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->BeginRenderpass(m_Renderpass, m_FrameBuffer, m_Scissor); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkRenderPass& _Renderpass, const VkFramebuffer& _FrameBuffer, const VkRect2D& _Scissor)
			{ m_Renderpass = _Renderpass; m_FrameBuffer = _FrameBuffer, m_Scissor = _Scissor; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			
			VkRenderPass m_Renderpass;
			VkFramebuffer m_FrameBuffer;
			VkRect2D m_Scissor;

		public:
			static Ref<BeginRenderpass> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class EndRenderpass : public VulkanCommand
		{
		public:
			EndRenderpass(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->EndRenderpass(); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

		public:
			static Ref<EndRenderpass> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BindPipeline : public VulkanCommand
		{
		public:
			BindPipeline(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->BindPipeline(m_Pipeline); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkPipeline& _Pipeline)
			{ m_Pipeline = _Pipeline; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			VkPipeline m_Pipeline;

		public:
			static Ref<BindPipeline> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class DrawIndexed : public VulkanCommand
		{
		public:
			DrawIndexed(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->DrawIndexed(m_IndexCount); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const uint32& _IndexCount)
			{ m_IndexCount  = _IndexCount; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			uint32 m_IndexCount = 0;

		public:
			static Ref<DrawIndexed> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BindVertexBuffer : public VulkanCommand
		{
		public:
			BindVertexBuffer(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->BindVertexBuffer(m_VertexBuffer); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkBuffer& _VertexBuffer)
			{ m_VertexBuffer  = _VertexBuffer; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			VkBuffer m_VertexBuffer;

		public:
			static Ref<BindVertexBuffer> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BindIndexBuffer : public VulkanCommand
		{
		public:
			BindIndexBuffer(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->BindIndexBuffer(m_IndexBuffer); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkBuffer& _IndexBuffer)
			{ m_IndexBuffer  = _IndexBuffer; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			VkBuffer m_IndexBuffer;

		public:
			static Ref<BindIndexBuffer> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class BindDescriptorSet : public VulkanCommand
		{
		public:
			BindDescriptorSet(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->BindDescriptorSet(m_Layout, m_DescriptorSet); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkPipelineLayout& _Layout, const VkDescriptorSet& _DescriptorSet)
			{ m_Layout = _Layout; m_DescriptorSet = _DescriptorSet; }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;

			VkPipelineLayout m_Layout;
			VkDescriptorSet m_DescriptorSet;

		public:
			static Ref<BindDescriptorSet> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

		class ExecuteCommandBuffers : public VulkanCommand
		{
		public:
			ExecuteCommandBuffers(const Ref<VulkanCommandBuffer>& _Buffer) : m_Buffer(_Buffer) {}
			virtual void Execute() override
			{ m_Buffer->ExecuteCommandBuffers(m_CommandBuffers); }
			virtual const VkCommandBuffer& Retrieve() const override
			{ return m_Buffer->GetBuffer(); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const Vector<VkCommandBuffer>& _Commands)
			{ m_CommandBuffers = _Commands; }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const VkCommandBuffer& _Command)
			{ m_CommandBuffers.push_back(_Command); }

			// TODO: Improve with a DataMessage Protocal
			void PopulateData(const Ref<VulkanCommandBuffer>& _Command)
			{ m_CommandBuffers.push_back(_Command->GetBuffer()); }

		private:
			Ref<VulkanCommandBuffer> m_Buffer;
			Vector<VkCommandBuffer> m_CommandBuffers;

		public:
			static Ref<ExecuteCommandBuffers> Create(const Ref<VulkanCommandBuffer>& _Buffer);
		};

	}

	typedef VulkanCommandList::ResetBuffer CommandResetBuffer;
	typedef VulkanCommandList::BeginBuffer CommandBeginBuffer;
	typedef VulkanCommandList::EndBuffer CommandEndBuffer;
	typedef VulkanCommandList::CopyBuffer CommandCopyBuffer;
	typedef VulkanCommandList::CopyImage CommandCopyImage;
	typedef VulkanCommandList::BindBarrier CommandBindBarrier;
	typedef VulkanCommandList::SetViewport CommandSetViewport;
	typedef VulkanCommandList::SetScissor CommandSetScissor;
	typedef VulkanCommandList::SetClearColor CommandSetClearColor;
	typedef VulkanCommandList::BeginRenderpass CommandBeginRenderpass;
	typedef VulkanCommandList::EndRenderpass CommandEndRenderpass;
	typedef VulkanCommandList::BindPipeline CommandBindPipeline;
	typedef VulkanCommandList::DrawIndexed CommandDrawIndexed;
	typedef VulkanCommandList::BindVertexBuffer CommandBindVertexBuffer;
	typedef VulkanCommandList::BindIndexBuffer CommandBindIndexBuffer;
	typedef VulkanCommandList::BindDescriptorSet CommandBindDescriptorSet;
	typedef VulkanCommandList::ExecuteCommandBuffers CommandExecuteCommandBuffers;

}