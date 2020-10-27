#include "Morppch.h"
#include "VulkanInstance.h"

namespace Morpheus {

	VulkanInstance::VulkanInstance()
	{
        CreateInstance();
        MORP_CORE_WARN("[VULKAN] Instance Was Created!");
	}

	VulkanInstance::~VulkanInstance()
	{
        delete m_Instance;
        MORP_CORE_WARN("[VULKAN] Instance Was Destoryed!");
	}

    void VulkanInstance::Destroy()
    {
        m_Instance.destroy();
    }

    Vector<Extention> VulkanInstance::GetExtensions(const Vector<vk::ExtensionProperties>& _Installed, const Vector<Extention>& _Wanted)
    {
        Vector<Extention> Out;
        for (Extention const& w : _Wanted)
            for (vk::ExtensionProperties const& i : _Installed) {
                String s = i.extensionName;
                if (s.compare(w) == 0) {
                    Out.push_back(w);
                    break;
                }
            }
        return Out;
    }

    Vector<Extention> VulkanInstance::GetLayers(const Vector<vk::LayerProperties>& _Installed, const Vector<Extention>& _Wanted)
    {
        Vector<Extention> Out;
        for (Extention const& w : _Wanted)
            for (vk::LayerProperties const& i : _Installed) {
                String s = i.layerName;
                if (s.compare(w) == 0) {
                    Out.push_back(w);
                    break;
                }
            }
              
        return Out;
    }

    void VulkanInstance::CreateInstance()
	{
        Vector<vk::ExtensionProperties> installedExtensions = vk::enumerateInstanceExtensionProperties();

        Vector<Extention> wantedExtensions =
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
        #ifdef VK_USE_PLATFORM_WIN32_KHR
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_MACOS_MVK
            VK_MVK_MACOS_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_XCB_KHR
            VK_KHR_XCB_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_ANDROID_KHR
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_XLIB_KHR
            VK_KHR_XLIB_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_XCB_KHR
            VK_KHR_XCB_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_WAYLAND_KHR
            VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_MIR_KHR || VK_USE_PLATFORM_DISPLAY_KHR
            VK_KHR_DISPLAY_EXTENSION_NAME
        #elif VK_USE_PLATFORM_ANDROID_KHR
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
        #elif VK_USE_PLATFORM_IOS_MVK
            VK_MVK_IOS_SURFACE_EXTENSION_NAME
        #endif
        };

        Vector<Extention>& extensions = GetExtensions(installedExtensions, wantedExtensions);
        Vector<vk::LayerProperties> installedLayers = vk::enumerateInstanceLayerProperties();
        Vector<Extention> wantedLayers = { "VK_LAYER_LUNARG_standard_validation" };
        Vector<Extention>& layers = GetLayers(installedLayers, wantedLayers);

        vk::ApplicationInfo AppInfo("Morpheus", VK_MAKE_VERSION(1, 0, 0), "MorpheusEngine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0);
        vk::InstanceCreateInfo CreateInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &AppInfo, (uint32)layers.size(), 
            layers.data(), (uint32)extensions.size(), extensions.data());

        m_Instance = vk::createInstance(CreateInfo);
	}

}