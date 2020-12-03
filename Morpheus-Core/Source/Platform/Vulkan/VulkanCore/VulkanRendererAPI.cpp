#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"
#include "Platform/Vulkan/VulkanBindables/VulkanBindingChain.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandList.h"

namespace Morpheus {

	void VulkanRendererAPI::Init()
	{
		m_Queue = VulkanMemoryManager::GetInstance()->GetQueue();
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetCommandSystem();

		m_CommandExecutor = VulkanExecutionStack::Make();
		m_CommandBuffers[0] = VulkanCommandBuffer::Make(m_CommandSystem->Allocate());
		m_CommandBuffers[1] = VulkanCommandBuffer::Make(m_CommandSystem->Allocate());
		m_CompileRequired = true;
	}

	void VulkanRendererAPI::Shutdown()
	{
	}

	void VulkanRendererAPI::SetClearColor(const Vector4& _ClearColor)
	{
		m_ClearColor = _ClearColor;
	}

	void VulkanRendererAPI::Clear()
	{
	}

	void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& _VertexArray)
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		uint32 VertexArrayID = Chain.GetVertexArrayID();
		
		if (VertexArrayID == _VertexArray->GetID()) {
			bool IsCommandNew = false;
			if(_VertexArray->CheckCompiled())
				return;
				
			Ref<VulkanCommandBuffer> Command;
			auto& It = m_SecondaryCommandBuffers.find(VertexArrayID);
			if (It != m_SecondaryCommandBuffers.end())				
				Command = It->second;
			else {
				Command = VulkanCommandBuffer::Make(m_CommandSystem->Allocate(true));
				IsCommandNew = true;
			}

			uint32 FrameBufferID = Chain.GetFrameBufferID();
			if (FrameBufferID == uint32_max)
				MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] No FrameBuffer is binded!");
			Ref<VulkanFrameBuffer> FrameBuffer = VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Get(FrameBufferID);
		
			uint32 ShaderID = Chain.GetShaderID();
			if (ShaderID == uint32_max)
				MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] No Shader is binded!");
			Ref<VulkanShader> Shader = VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID);
		
			Ref<VulkanSwapchain> Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();
			VkViewport Viewport = Swapchain->GetViewport();
			VkRect2D Scissor = Swapchain->GetRenderArea();

			// Get Vulkan Data
			VkDescriptorSet DescriptorSet = VulkanMemoryManager::GetInstance()->GetUniformBufferCache().Get(_VertexArray->GetUniformBufferID())->GetDescriptorSet();
			VkBuffer VertexBuffer = VulkanMemoryManager::GetInstance()->GetVertexBufferCache().Get(_VertexArray->GetVertexBufferID())->GetBuffer();
			VkBuffer IndexBuffer = VulkanMemoryManager::GetInstance()->GetIndexBufferCache().Get(_VertexArray->GetIndexBufferID())->GetBuffer();
			uint32 IndexCount = VulkanMemoryManager::GetInstance()->GetIndexBufferCache().Get(_VertexArray->GetIndexBufferID())->GetCount();
		
			Ref<CommandBeginBuffer> BeginBuffer = CommandBeginBuffer::Create(Command, true);
			BeginBuffer->PopulateData(FrameBuffer->GetRenderpass());
			m_CommandExecutor->AppendCommand(BeginBuffer);
			
			Ref<CommandSetViewport> SetViewport = CommandSetViewport::Create(Command);
			SetViewport->PopulateData(Viewport);
			m_CommandExecutor->AppendCommand(SetViewport);
			
			Ref<CommandSetScissor> SetScissor = CommandSetScissor::Create(Command);
			SetScissor->PopulateData(Scissor);
			m_CommandExecutor->AppendCommand(SetScissor);

			Ref<CommandBindPipeline> BindPipeline = CommandBindPipeline::Create(Command);
			BindPipeline->PopulateData(Shader->GetPipeline());
			m_CommandExecutor->AppendCommand(BindPipeline);

			Ref<CommandBindVertexBuffer> BindVertexBuffer = CommandBindVertexBuffer::Create(Command);
			BindVertexBuffer->PopulateData(VertexBuffer);
			m_CommandExecutor->AppendCommand(BindVertexBuffer);

			Ref<CommandBindIndexBuffer> BindIndexBuffer = CommandBindIndexBuffer::Create(Command);
			BindIndexBuffer->PopulateData(IndexBuffer);
			m_CommandExecutor->AppendCommand(BindIndexBuffer);

			Ref<CommandBindDescriptorSet> BindDescriptorSet = CommandBindDescriptorSet::Create(Command);
			BindDescriptorSet->PopulateData(Shader->GetPipelineLayout(), DescriptorSet);
			m_CommandExecutor->AppendCommand(BindDescriptorSet);

			Ref<CommandDrawIndexed> DrawIndexed = CommandDrawIndexed::Create(Command);
			DrawIndexed->PopulateData(IndexCount);
			m_CommandExecutor->AppendCommand(DrawIndexed);

			m_CommandExecutor->AppendCommand(CommandEndBuffer::Create(Command));

			m_CommandExecutor->Compile();
			m_CommandExecutor->Reset();

			if (IsCommandNew)
				m_SecondaryCommandBuffers[VertexArrayID] = Command;

			_VertexArray->SetCompiled(true);
			m_CompileRequired = true;
		}
	}

	void VulkanRendererAPI::Flush()
	{
		if (m_CompileRequired)
			BuildPrimary();

		uint32 _FrameIndex = m_Queue->GetCurrentFrame();
		m_Queue->Submit(m_CommandBuffers[_FrameIndex]->GetBuffer(), QueueCommandFlags::DeleteCommand);
	}

	void VulkanRendererAPI::BuildPrimary()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		uint32 FrameBufferID = Chain.GetFrameBufferID();
		if (FrameBufferID == uint32_max)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] No FrameBuffer is binded!");
		Ref<VulkanFrameBuffer> FrameBuffer = VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Get(FrameBufferID);

		for (uint32 i = 0; i < m_Queue->GetBufferCount(); i++) {
			Ref<VulkanSwapchain> Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();
			VkViewport Viewport = Swapchain->GetViewport();
			VkRect2D Scissor = Swapchain->GetRenderArea();

			m_CommandExecutor->AppendCommand(CommandResetBuffer::Create(m_CommandBuffers[i]));
			m_CommandExecutor->AppendCommand(CommandBeginBuffer::Create(m_CommandBuffers[i]));

			Ref<CommandSetClearColor> SetClearColor = CommandSetClearColor::Create(m_CommandBuffers[i]);
			SetClearColor->PopulateData(m_ClearColor);
			m_CommandExecutor->AppendCommand(SetClearColor);

			Ref<CommandBeginRenderpass> BeginRenderpass = CommandBeginRenderpass::Create(m_CommandBuffers[i]);
			BeginRenderpass->PopulateData(FrameBuffer->GetRenderpass(), FrameBuffer->GetFrameBuffer(i), Scissor);
			m_CommandExecutor->AppendCommand(BeginRenderpass);
			
			//Ref<CommandSetViewport> SetViewport = CommandSetViewport::Create(m_CommandBuffers[i]);
			//SetViewport->PopulateData(Viewport);
			//m_CommandExecutor->AppendCommand(SetViewport);
			//
			//Ref<CommandSetScissor> SetScissor = CommandSetScissor::Create(m_CommandBuffers[i]);
			//SetScissor->PopulateData(Scissor);
			//m_CommandExecutor->AppendCommand(SetScissor);

			Ref<CommandExecuteCommandBuffers> ExecuteCommandBuffers = CommandExecuteCommandBuffers::Create(m_CommandBuffers[i]);
			for (auto it = m_SecondaryCommandBuffers.begin(); it != m_SecondaryCommandBuffers.end(); ++it)
				ExecuteCommandBuffers->PopulateData(it->second);
			m_CommandExecutor->AppendCommand(ExecuteCommandBuffers);

			m_CommandExecutor->AppendCommand(CommandEndRenderpass::Create(m_CommandBuffers[i]));
			m_CommandExecutor->AppendCommand(CommandEndBuffer::Create(m_CommandBuffers[i]));
			m_CommandExecutor->Compile();

			m_CommandExecutor->Reset();
		}

		m_CompileRequired = false;
		MORP_CORE_TRACE("[VULKAN] RendererAPI Primary CommandBuffer Compiled!");
	}

	void VulkanRendererAPI::BuildSecondary()
	{
	}

}