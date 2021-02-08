#include "Morppch.h"
#include "VulkanContext.h"

#include "Morpheus/Core/Application.h"

namespace Morpheus { namespace Vulkan {

	bool VulkanContext::s_Running = true;

	void VulkanContext::Init()
	{
		MORP_PROFILE_FUNCTION();

		m_Instance = VulkanInstance::GetInstance();
		m_Instance->GetSurface();
		m_Device = m_Instance->GetDevice(0);
		m_Device->CreateSwapchain();
	}

	void VulkanContext::Shutdown()
	{
		MORP_PROFILE_FUNCTION();
		m_Instance->Shutdown();
	}

	void VulkanContext::Flush()
	{
		MORP_PROFILE_FUNCTION();

		// Maybe Construct the values.!
	}

}}