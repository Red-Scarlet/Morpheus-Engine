#include "Morppch.h"
#include "VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanSwapchain::VulkanSwapchain()
	{
        m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
        m_Surface = VulkanMemoryManager::GetInstance()->GetSurface();

        VulkanCreate();
        MORP_CORE_WARN("[VULKAN] Swapchain Was Created!");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
        VulkanDestory();
        MORP_CORE_WARN("[VULKAN] Swapchain Was Destoryed!");
	}

    void VulkanSwapchain::VulkanCreate()
    {
        vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
        vk::Device Device = m_Device->GetLogicalDevice();
        uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

        vk::SurfaceKHR Surface = m_Surface->GetSurface();
        vk::Format ColorFormat = m_Surface->GetColorFormat();
        vk::ColorSpaceKHR ColorSpace = m_Surface->GetColorSpace();
        vk::Format DepthFormat = m_Surface->GetDepthFormat();

        vk::Extent2D SwapchainSize = vk::Extent2D(m_Width, m_Height);
        vk::SurfaceCapabilitiesKHR surfaceCapabilities = PhysicalDevice.getSurfaceCapabilitiesKHR(Surface);
        if (!(surfaceCapabilities.currentExtent.width == -1 || surfaceCapabilities.currentExtent.height == -1)) {
            SwapchainSize = surfaceCapabilities.currentExtent;
            m_RenderArea = vk::Rect2D(vk::Offset2D(), SwapchainSize);
            m_Viewport = vk::Viewport(0.0f, 0.0f, (float32)SwapchainSize.width, (float32)SwapchainSize.height, 0, 1.0f);
        }

        Vector<vk::PresentModeKHR> surfacePresentModes = PhysicalDevice.getSurfacePresentModesKHR(Surface);
        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eImmediate;

        for (vk::PresentModeKHR& pm : surfacePresentModes) {
            if (pm == vk::PresentModeKHR::eMailbox) {
                presentMode = vk::PresentModeKHR::eMailbox;
                break;
            }
        }

        m_Device->Wait();
        vk::SwapchainKHR oldSwapchain = m_Swapchain;

        uint32 backbufferCount = std::clamp(surfaceCapabilities.maxImageCount, 1U, 2U);

        vk::SwapchainCreateInfoKHR CreateInfo{};
        CreateInfo.flags = vk::SwapchainCreateFlagsKHR();
        CreateInfo.surface = Surface;
        CreateInfo.minImageCount = backbufferCount;
        CreateInfo.imageFormat = ColorFormat;
        CreateInfo.imageColorSpace = ColorSpace;
        CreateInfo.imageExtent = SwapchainSize;

        CreateInfo.imageArrayLayers = 1;
        CreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        CreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
        CreateInfo.queueFamilyIndexCount = 1;
        CreateInfo.pQueueFamilyIndices = &QueueFamilyIndex;
        CreateInfo.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
        CreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        CreateInfo.presentMode = presentMode;
        CreateInfo.clipped = VK_TRUE;
        CreateInfo.oldSwapchain = oldSwapchain;


        m_Swapchain = Device.createSwapchainKHR(CreateInfo);

        m_SurfaceSize = vk::Extent2D(std::clamp(SwapchainSize.width, 1U, 8192U), std::clamp(SwapchainSize.height, 1U, 8192U));
        m_RenderArea = vk::Rect2D(vk::Offset2D(), m_SurfaceSize);
        m_Viewport = vk::Viewport(0.0f, 0.0f, (float32)m_SurfaceSize.width, (float32)m_SurfaceSize.height, 0, 1.0f);

        if (oldSwapchain != vk::SwapchainKHR(nullptr))
            Device.destroySwapchainKHR(oldSwapchain);

        m_BackBufferCount = backbufferCount;
    }

    void VulkanSwapchain::VulkanDestory()
    {
        vk::Device Device = m_Device->GetLogicalDevice();
        Device.destroySwapchainKHR(m_Swapchain);
    }

	Ref<VulkanSwapchain> VulkanSwapchain::Make()
	{
        Ref<VulkanSwapchain> s_VulkanSwapchain = CreateRef<VulkanSwapchain>();
        VulkanMemoryManager::GetInstance()->SetSwapchain(s_VulkanSwapchain);
        return s_VulkanSwapchain;
	}

}