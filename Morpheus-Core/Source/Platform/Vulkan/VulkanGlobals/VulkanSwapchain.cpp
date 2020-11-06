#include "Morppch.h"
#include "VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanSwapchain::VulkanSwapchain()
	{
        m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
        SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanSwapchain));
        CreateSwapchain();
        MORP_CORE_WARN("[VULKAN] Swapchain Was Created!");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
        MORP_CORE_WARN("[VULKAN] Swapchain Was Destoryed!");
	}

    void VulkanSwapchain::Destory()
    {
        vk::Device Device = m_Device->GetLogicalDevice();
        Device.destroySwapchainKHR(m_Swapchain);
    }

    void VulkanSwapchain::CreateSwapchain()
    {
        vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
        vk::Device Device = m_Device->GetLogicalDevice();
        uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

        SurfaceStruct Struct = m_Device->GetSurface()->GetStruct();

        vk::Extent2D SwapchainSize = vk::Extent2D(m_Width, m_Height);
        vk::SurfaceCapabilitiesKHR surfaceCapabilities = PhysicalDevice.getSurfaceCapabilitiesKHR(Struct.Surface);
        if (!(surfaceCapabilities.currentExtent.width == -1 || surfaceCapabilities.currentExtent.height == -1)) {
            SwapchainSize = surfaceCapabilities.currentExtent;
            m_RenderArea = vk::Rect2D(vk::Offset2D(), SwapchainSize);
            m_Viewport = vk::Viewport(0.0f, 0.0f, (float32)SwapchainSize.width, (float32)SwapchainSize.height, 0, 1.0f);
        }

        Vector<vk::PresentModeKHR> surfacePresentModes = PhysicalDevice.getSurfacePresentModesKHR(Struct.Surface);
        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eImmediate;

        for (vk::PresentModeKHR& pm : surfacePresentModes) {
            if (pm == vk::PresentModeKHR::eMailbox) {
                presentMode = vk::PresentModeKHR::eMailbox;
                break;
            }
        }

        m_Device->Wait();
        vk::SwapchainKHR oldSwapchain = m_Swapchain;

        uint32_t backbufferCount = std::clamp(surfaceCapabilities.maxImageCount, 1U, 2U);

        vk::SwapchainCreateInfoKHR CreateInfo {};
        CreateInfo.flags = vk::SwapchainCreateFlagsKHR();
        CreateInfo.surface = Struct.Surface;
        CreateInfo.minImageCount = backbufferCount;
        CreateInfo.imageFormat = Struct.ColorFormat;
        CreateInfo.imageColorSpace = Struct.ColorSpace;
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

        // Destroy previous swapchain
        if (oldSwapchain != vk::SwapchainKHR(nullptr))
            Device.destroySwapchainKHR(oldSwapchain);
        
        // Resize swapchain buffers for use later
        m_BackBufferCount = backbufferCount;
        // m_SwapchainBuffers.resize(backbufferCount);
	}

	Ref<VulkanSwapchain> VulkanSwapchain::Create()
	{
        Ref<VulkanSwapchain> s_VulkanSwapchain = CreateRef<VulkanSwapchain>();
        VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain, s_VulkanSwapchain);
        return s_VulkanSwapchain;
	}

}