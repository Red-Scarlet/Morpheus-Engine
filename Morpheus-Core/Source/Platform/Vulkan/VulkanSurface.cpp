// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"
#include "VulkanSurface.h"

#include <GLFW/glfw3.h>
#include "Morpheus/Core/Application.h"

namespace Morpheus {

	VulkanSurface::VulkanSurface(VulkanInstance* Instance)
		: m_VulkanInstance(Instance)
	{
		CreateSurface();
		MORP_CORE_WARN("[VULKAN] Vulkan Surface Was Created!");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_VulkanInstance->GetVulkan(), m_Surface, nullptr);
	}

	void VulkanSurface::CreateSurface()
	{
		Window& GLFW = Application::Get().GetWindow();

		VkResult result = glfwCreateWindowSurface(m_VulkanInstance->GetVulkan(), (GLFWwindow*)GLFW.GetWindowCore(), nullptr, &m_Surface);
		MORP_CORE_ASSERT(result, "Failed to create Window Surface!");
	}

}
