#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include "VulkanResource.h"

namespace Morpheus {

	void VulkanRendererAPI::Init()
	{
		m_Command = VulkanCommand::Create();
		m_Sync = VulkanSynchronization::Create();
		//m_DescriptorPool = VulkanDescriptorPool::Create();
	}

	void VulkanRendererAPI::Shutdown()
	{
		m_Command->FreeBuffers();
		m_Command->Destory();
		m_Sync->Destory();
	}

	void VulkanRendererAPI::Flush()
	{
		// Check for compilation
		// Check for Ready

		if(m_CompileCommands) {
			m_Command->SetupCommands();	
			m_CompileCommands = false;
			MORP_CORE_WARN("[VULKAN] Command System Compiled!");
		}

		if (m_ReadyRender)
			if (m_Sync->Render())
				Resize();
	}

	void VulkanRendererAPI::SetCompile(const bool& _Value)
	{
		m_CompileCommands = _Value;
	}

	void VulkanRendererAPI::SetReady(const bool& _Value)
	{
		m_ReadyRender = _Value;
	}

	void VulkanRendererAPI::Resize()
	{
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