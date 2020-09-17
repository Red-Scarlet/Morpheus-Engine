#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	//TODO LIST: 
	/*
		Ref<Shader>
		Cleanup and maybe adopt Ref<T> instead of *
	*/

	void VulkanRendererAPI::Init()
	{
		m_VulkanInstance = VulkanInstance::GetInstance();
		m_VulkanPresentation = m_VulkanInstance->GetPresentation();
		m_VulkanDevice = m_VulkanInstance->GetLogicalDevice();

		m_VulkanPresentation->SetupSwapchain();
		m_VulkanPresentation->SetupImageview();
		
		m_VulkanCommandSystem = new VulkanCommandSystem();
		m_CommandBuffer = new VulkanCommandBuffer(m_VulkanDevice, m_VulkanPresentation);
		m_VulkanSynchronization = new VulkanSynchronization(m_VulkanDevice, m_VulkanPresentation);
	}

	void VulkanRendererAPI::Shutdown()
	{		
		delete m_VulkanSynchronization;
		delete m_VulkanCommandSystem;
		delete m_VulkanDevice;
		delete m_VulkanPresentation;
		m_VulkanInstance->Shutdown();
	}

	void VulkanRendererAPI::Begin()
	{
		m_VulkanCommandSystem->AllocateBuffers(m_CommandBuffer);

		m_CommandBuffer->BeginRecord();
	}

	void VulkanRendererAPI::End()
	{
		m_CommandBuffer->EndRecord();
		m_VulkanCommandSystem->ComputeBuffers(m_CommandBuffer);
	}

	void VulkanRendererAPI::Flush()
	{
		m_VulkanSynchronization->Flush(m_VulkanCommandSystem);
		m_VulkanDevice->Wait();

		m_VulkanCommandSystem->ResetBuffers(m_CommandBuffer);
	}

	void VulkanRendererAPI::Reset()
	{
		m_VulkanDevice->Wait();
	}

	void VulkanRendererAPI::SetViewport()
	{
		m_CommandBuffer->cbSetViewport();
	}

	void VulkanRendererAPI::SetClearColor(const Vector4& _Color)
	{
		m_CommandBuffer->cbSetClearcolor(_Color);
	}

	void VulkanRendererAPI::SetRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		m_CommandBuffer->cbSetRenderpass(_Renderpass);
	}

	void VulkanRendererAPI::SetPipeline(const Ref<Pipeline>& _Pipeline)
	{
		m_CommandBuffer->cbSetPipeline(_Pipeline);
	}

	void VulkanRendererAPI::SetFramebuffer(const Ref<Framebuffer>& _Framebuffer)
	{
		m_CommandBuffer->cbSetFramebuffer(_Framebuffer);
	}

	void VulkanRendererAPI::DrawGeomerty()
	{
		m_CommandBuffer->cbDraw();
	}


}