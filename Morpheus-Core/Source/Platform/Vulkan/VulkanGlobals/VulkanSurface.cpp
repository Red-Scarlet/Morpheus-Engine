#include "Morppch.h"
#include "VulkanSurface.h"

#include "Morpheus/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Morpheus {

	VulkanSurface::VulkanSurface(const vk::Instance& _Instance, const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex)
	{
		m_Instance = _Instance;
		m_PhysicalDevice = _Physical;
		m_QueueFamilyIndex = _QueueFamilyIndex;

		if (!CreateSurface())
			MORP_CORE_ERROR("[VULKAN] Surface cannot be Created!");
		MORP_CORE_WARN("[VULKAN] Surface Was Created!");
	}

	VulkanSurface::~VulkanSurface()
	{
		MORP_CORE_WARN("[VULKAN] Surface Was Destoryed!");
	}

	void VulkanSurface::SetupColorFormats()
	{
		Vector<vk::SurfaceFormatKHR> SurfaceFormats = m_PhysicalDevice.getSurfaceFormatsKHR(m_Struct.Surface);

		if (SurfaceFormats.size() == 1 && SurfaceFormats[0].format == vk::Format::eUndefined)
			m_Struct.ColorFormat = vk::Format::eB8G8R8A8Unorm;
		else
			m_Struct.ColorFormat = SurfaceFormats[0].format;
		m_Struct.ColorSpace = SurfaceFormats[0].colorSpace;
		Vector<vk::Format> DepthFormats = {
			vk::Format::eD32SfloatS8Uint,
			vk::Format::eD32Sfloat,
			vk::Format::eD24UnormS8Uint,
			vk::Format::eD16UnormS8Uint,
			vk::Format::eD16Unorm
		};

		for (vk::Format& Format : DepthFormats)
		{
			vk::FormatProperties depthFormatProperties = m_PhysicalDevice.getFormatProperties(Format);
			if (depthFormatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment){
				m_Struct.DepthFormat = Format;
				break;
			}
		}
	}

	bool VulkanSurface::CreateSurface()
	{
		VkSurfaceKHR Surface = VkSurfaceKHR(m_Struct.Surface);
		Window& GLFW = Application::Get().GetWindow();
		VkResult result = glfwCreateWindowSurface(m_Instance, (GLFWwindow*)GLFW.GetWindowCore(), nullptr, &Surface);
		MORP_CORE_ASSERT(result, "Failed to create Window Surface!");
		m_Struct.Surface = vk::SurfaceKHR(Surface);
		
		if (!m_PhysicalDevice.getSurfaceSupportKHR(m_QueueFamilyIndex, Surface))
			return false;
		return true;
	}
	
}