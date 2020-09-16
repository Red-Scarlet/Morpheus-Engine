#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

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
		
		// WRAP INTO COMMAND SYSTEM
		m_VulkanCommandPool = new VulkanCommandPool(m_VulkanDevice);
		m_VulkanCommandBuffer = new VulkanCommandBuffer(m_VulkanDevice, m_VulkanPresentation, m_VulkanFramebuffer, m_VulkanCommandPool,
			m_VulkanRenderpass, m_VulkanGraphicsPipeline);

		m_VulkanSynchronization = new VulkanSynchronization(m_VulkanDevice, m_VulkanPresentation);
	}

	void VulkanRendererAPI::Shutdown()
	{		
		delete m_VulkanSynchronization;
		delete m_VulkanCommandBuffer;
		delete m_VulkanCommandPool;
		delete m_VulkanFramebuffer;
		delete m_VulkanGraphicsPipeline;
		delete m_VulkanRenderpass;
		delete m_VulkanDevice;
		delete m_VulkanPresentation;
		m_VulkanInstance->Shutdown();
	}

	void VulkanRendererAPI::Begin()
	{
		m_VulkanSynchronization->Begin(m_VulkanCommandBuffer);
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