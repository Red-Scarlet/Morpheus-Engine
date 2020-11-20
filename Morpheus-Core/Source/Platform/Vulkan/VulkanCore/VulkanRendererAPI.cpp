#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"
#include "Platform/Vulkan/VulkanBindables/VulkanBindingChain.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"


namespace Morpheus {

	void VulkanRendererAPI::Init()
	{
		m_Queue = VulkanMemoryManager::GetInstance()->GetQueue();
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetCommandSystem();
		m_Commands = m_CommandSystem->BatchAllocate(m_Queue->GetBufferCount());
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

		uint32 ShaderID = Chain.GetShaderID();
		if (ShaderID == uint32_max)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] No Shader is binded!");
		Ref<VulkanShader> Shader = VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID);

		if (Chain.GetVertexArrayID() == _VertexArray->GetID()) {
			m_VertexArrays.emplace_back(_VertexArray->GetID());
			Shader->CompileUniform(_VertexArray->GetUniformBufferID());
		}	
	}

	void VulkanRendererAPI::Flush()
	{
		if (!m_Compilation)
			SetupCommands();

		uint32 _FrameIndex = m_Queue->GetCurrentFrame();
		m_Queue->Submit(m_Commands[_FrameIndex], QueueCommandFlags::DeleteCommand);
	}

	void VulkanRendererAPI::SetupCommands()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		uint32 FrameBufferID = Chain.GetFrameBufferID();
		if (FrameBufferID == uint32_max)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] No FrameBuffer is binded!");
		Ref<VulkanFrameBuffer> FrameBuffer = VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Get(FrameBufferID);

		uint32 ShaderID = Chain.GetShaderID();
		if (ShaderID == uint32_max)
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] No Shader is binded!");
		Ref<VulkanShader> Shader = VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID);

		for (uint32 i = 0; i < m_Queue->GetBufferCount(); i++)
		{
			Ref<VulkanSwapchain> Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();
			vk::Viewport Viewport = Swapchain->GetViewport();
			vk::Rect2D RenderArea = Swapchain->GetRenderArea();

			VulkanCommandBuffer BufferExecutor(m_Commands[i]);
			BufferExecutor.ResetBuffer();
			BufferExecutor.BeginBuffer();
			BufferExecutor.SetClearColor(m_ClearColor);
			BufferExecutor.BindFramebuffer(FrameBuffer, i);
			BufferExecutor.SetViewport(Viewport);
			BufferExecutor.SetScissor(RenderArea);
			BufferExecutor.BindShader(Shader);

			for (uint32 j = 0; j < m_VertexArrays.size(); j++) 
				BufferExecutor.SubmitVertexArray(m_VertexArrays[j]);

			BufferExecutor.DrawIndexed();
			BufferExecutor.EndBuffer();
			BufferExecutor.Compile();
		}

		m_Compilation = true;
	}

}