#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	//TODO LIST: 
	/*
		Ref<Shader>
		Ref<Renderpass>
		Ref<GraphicsPipeline>
		Ref<Framebuffer>

		RendererAPI --> CMD

		Cleanup and maybe adopt Ref<T> instead of *
	*/

	void VulkanRendererAPI::Init()
	{
		m_VulkanInstance = VulkanInstance::GetInstance();
		m_VulkanPresentation = m_VulkanInstance->GetPresentation();
		m_VulkanDevice = m_VulkanInstance->GetLogicalDevice();

		m_VulkanPresentation->SetupSwapchain();
		m_VulkanPresentation->SetupImageview();
		
		m_VulkanRenderpass = new VulkanRenderpass(m_VulkanDevice, m_VulkanPresentation);
		m_VulkanGraphicsPipeline = new VulkanGraphicsPipeline(m_VulkanDevice, m_VulkanPresentation, m_VulkanRenderpass);
		m_VulkanFramebuffer = new VulkanFramebuffer(m_VulkanDevice, m_VulkanPresentation, m_VulkanRenderpass);
		
		m_VulkanCommandSystem = new VulkanCommandSystem();

		m_CommandBuffer = new VulkanCommandBuffer();

		m_VulkanSynchronization = new VulkanSynchronization(m_VulkanDevice, m_VulkanPresentation);

		m_CommandBuffer->cbSetRenderpass(m_VulkanRenderpass);
		m_CommandBuffer->cbSetPipeline(m_VulkanGraphicsPipeline);
		m_CommandBuffer->cbSetFramebuffer(m_VulkanFramebuffer);
		m_CommandBuffer->cbSetClearcolor({0.4, 0.4, 0.6, 1.0f});

		m_VulkanCommandSystem->AllocateBuffer(m_CommandBuffer);

		m_CommandBuffer->BeginRecord();
		m_CommandBuffer->cbDraw();
		m_CommandBuffer->EndRecord();

		m_VulkanCommandSystem->ComputeBuffer(m_CommandBuffer);
	}

	void VulkanRendererAPI::Shutdown()
	{		
		delete m_VulkanSynchronization;
		delete m_VulkanCommandSystem;
		delete m_VulkanFramebuffer;
		delete m_VulkanGraphicsPipeline;
		delete m_VulkanRenderpass;
		delete m_VulkanDevice;
		delete m_VulkanPresentation;
		m_VulkanInstance->Shutdown();
	}

	void VulkanRendererAPI::Begin()
	{
		m_VulkanSynchronization->Begin(m_VulkanCommandSystem);
	}

	void VulkanRendererAPI::End()
	{	
		m_VulkanSynchronization->End(m_VulkanRenderpass);
	}

	void VulkanRendererAPI::Flush()
	{
		m_VulkanSynchronization->Flush();
		vkDeviceWaitIdle(m_VulkanDevice->GetDevice());
	}

	void VulkanRendererAPI::Reset()
	{
		m_VulkanDevice->Wait();
	}


}