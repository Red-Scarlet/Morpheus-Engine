#include "Morppch.h"
#include "VulkanSurface.h"
#include "Morpheus/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Morpheus { namespace Vulkan {

	VulkanSurface::VulkanSurface(const VkInstance& _Instance)
		: m_Instance(_Instance)
	{
		CreateSurface();
		VULKAN_CORE_WARN("[VULKAN] Surface Was Created!");
	}

	VulkanSurface::~VulkanSurface()
	{
		VULKAN_CORE_WARN("[VULKAN] Surface Was Destroyed!");
	}

	void VulkanSurface::CreateSurface()
	{
		MORP_PROFILE_FUNCTION();

		Window& GLFW = Application::Get().GetWindow();
		VkResult result = glfwCreateWindowSurface(m_Instance,
			(GLFWwindow*)GLFW.GetNativeWindow(), nullptr, &m_Surface);
		VULKAN_CORE_ASSERT(result, "Failed to create Window Surface!");
	}

	void VulkanSurface::DestroySurface()
	{
		MORP_PROFILE_FUNCTION();

		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
	}

	Ref<VulkanSurface> VulkanSurface::Create(const VkInstance& _Instance)
	{
		return CreateRef<VulkanSurface>(_Instance);
	}

	void VulkanSurface::Destroy(const Ref<VulkanSurface>& _Surface)
	{
		_Surface->DestroySurface();
	}

}}