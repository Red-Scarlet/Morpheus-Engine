#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	void VulkanRendererAPI::Init()
	{
		m_Command = VulkanCommand::Create();
		m_RenderQueue = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanRenderQueue>(VulkanGlobalTypes::VulkanRenderQueue);
		//m_RenderQueue = VulkanRenderQueue::VulkanCreate();

	}

	void VulkanRendererAPI::Shutdown()
	{
		m_Command->FreeBuffers();
		m_Command->Destory();
		//m_Sync->Destory();
	}

	void VulkanRendererAPI::Flush()
	{
		// Check for Ready
		if(m_Command->SetupCommands(m_ClearColor)) {
			//m_RenderQueue->Submit(m_Command->GetCommandBuffer(1));
		}	// Clear upon submission
		
		m_RenderQueue->Submit(m_Command->GetCommandBuffer(m_RenderQueue->GetCurrentFrame()));
		//MORP_CORE_INFO("[QUEUE] Submission made from RENDERERAPI #" + std::to_string(m_RenderQueue->GetCurrentFrame()) + "!");

		//if (m_ReadyRender) 
		//	m_RenderQueue->Flush(false);
	}

	void VulkanRendererAPI::SetCompile(const bool& _Value)
	{
		m_Command->SetRecompilationState(_Value);
	}

	void VulkanRendererAPI::SetReady(const bool& _Value)
	{
		m_ReadyRender = _Value;
	}

	void VulkanRendererAPI::SetClearColor(const Vector4& _ClearColor)
	{
		m_ClearColor = _ClearColor;
		m_Command->SetRecompilationState(true);
	}

	void VulkanRendererAPI::Resize()
	{
		MORP_CORE_ERROR("WHY DID THIS NOT RENDER?");

		//VulkanResourceManager::GetInstance()->DeviceFunction(
		//	[](Ref<VulkanDevice> Device) {
		//	Device->Wait();
		//});
		//
		//// Swapchain Function
		//
		//// FrameBuffer Reset
		//
		//m_Command->Destroy();
		//m_Command = VulkanCommand::Create(VulkanMessageType::CommandCreate);
		//m_CompileCommands = true;
	}

}