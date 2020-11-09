#include "Morppch.h"
#include "VulkanSurface.h"

#include "Morpheus/Core/Application.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

#include <GLFW/glfw3.h>

namespace Morpheus {

	VulkanSurface::VulkanSurface(const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex)
		: VulkanGlobal(VulkanGlobalTypes::VulkanSurface), m_PhysicalDevice(_Physical), m_QueueFamilyIndex(_QueueFamilyIndex)
	{
		m_Instance = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanInstance>(VulkanGlobalTypes::VulkanInstance);

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Surface Was Created!");
		SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanSurface));
	}

	VulkanSurface::~VulkanSurface()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Surface Was Destoryed!");
	}

	void VulkanSurface::SetupColorFormats()
	{
		Vector<vk::SurfaceFormatKHR> SurfaceFormats = m_PhysicalDevice.getSurfaceFormatsKHR(m_Surface);

		if (SurfaceFormats.size() == 1 && SurfaceFormats[0].format == vk::Format::eUndefined)
			m_ColorFormat = vk::Format::eB8G8R8A8Unorm;
		else
			m_ColorFormat = SurfaceFormats[0].format;
		m_ColorSpace = SurfaceFormats[0].colorSpace;
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
				m_DepthFormat = Format;
				break;
			}
		}
	}

	void VulkanSurface::VulkanCreate()
	{
		VkSurfaceKHR Surface = VkSurfaceKHR(m_Surface);
		Window& GLFW = Application::Get().GetWindow();
		VkResult result = glfwCreateWindowSurface(m_Instance->GetInstance(), (GLFWwindow*)GLFW.GetNativeWindow(), nullptr, &Surface);
		MORP_CORE_ASSERT(result, "Failed to create Window Surface!");
		m_Surface = vk::SurfaceKHR(Surface);

		if (!m_PhysicalDevice.getSurfaceSupportKHR(m_QueueFamilyIndex, Surface))
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Surface cannot be Created!");
	}

	void VulkanSurface::VulkanDestory()
	{
		// Do Delete!
	}

	Ref<VulkanSurface> VulkanSurface::Make(const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex)
	{
		Ref<VulkanSurface> s_VulkanSurface = CreateRef<VulkanSurface>(_Physical, _QueueFamilyIndex);
		VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit(s_VulkanSurface);
		return s_VulkanSurface;
	}
	
}