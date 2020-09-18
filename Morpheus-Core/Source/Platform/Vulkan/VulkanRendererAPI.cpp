#include "Morppch.h"
#include "VulkanRendererAPI.h"

namespace Morpheus {

	void VulkanRendererAPI::Init()
	{
		m_Vulkan.Instance = VulkanInstance::GetInstance();
		m_Vulkan.Presentation = m_Vulkan.Instance->GetPresentation();
		m_Vulkan.Device = m_Vulkan.Instance->GetLogicalDevice();

		m_Vulkan.Presentation->SetupSwapchain();
		m_Vulkan.Presentation->SetupImageview();
		
		m_Command.System = new VulkanCommandSystem();

		m_Command.Synchronization = new VulkanSynchronization(m_Vulkan.Device, m_Vulkan.Presentation);
	}

	void VulkanRendererAPI::Shutdown()
	{		
		delete m_Command.Synchronization;
		delete m_Command.System;

		delete m_Vulkan.Device;
		delete m_Vulkan.Presentation;

		m_Vulkan.Instance->Shutdown();
	}

	void VulkanRendererAPI::Begin()
	{
		m_Command.System->AllocateBuffers();
		m_Command.System->BeginRecord();
	}

	void VulkanRendererAPI::End()
	{
		m_Command.System->EndRecord();
		m_Command.System->CompileBuffers();
	}

	void VulkanRendererAPI::Flush()
	{
		m_Command.Synchronization->Flush(m_Command.System);
		m_Vulkan.Device->Wait();
		m_Command.System->DeallocateBuffers();
	}

	void VulkanRendererAPI::Reset()
	{
		m_Command.System->DeallocateBuffers();
		m_Vulkan.Device->Wait();
	}

	void VulkanRendererAPI::SetViewport()
	{
		m_Command.System->SetViewport();
	}

	void VulkanRendererAPI::SetClearColor(const Vector4& _Color)
	{
		m_Command.System->SetClearcolor(_Color);
	}

	void VulkanRendererAPI::BindPipeline(const Ref<Pipeline>& _Pipeline)
	{
		m_Command.System->BindPipeline(_Pipeline);
	}

	void VulkanRendererAPI::BeginRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		m_Command.System->BeginRenderpass(_Renderpass);
	}

	void VulkanRendererAPI::EndRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		m_Command.System->EndRenderpass(_Renderpass);
	}

	void VulkanRendererAPI::DrawGeomerty(const Ref<VertexBuffer>& _VertexBuffer)
	{
		m_Command.System->DrawGeomerty(_VertexBuffer);
	}


}