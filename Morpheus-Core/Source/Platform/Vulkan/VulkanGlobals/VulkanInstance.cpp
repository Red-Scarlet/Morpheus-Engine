#include "Morppch.h"
#include "VulkanInstance.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"



namespace Morpheus {

	VulkanInstance::VulkanInstance()
        : VulkanGlobal(VulkanGlobalTypes::VulkanInstance)
	{
        VulkanCreate();
        MORP_CORE_WARN("[VULKAN] Instance Was Created!");
        SetID(VulkanMemoryManager::GetInstance()->GetGlobalCache()->GetNextGlobalID(VulkanGlobalTypes::VulkanInstance));
	}

	VulkanInstance::~VulkanInstance()
	{
        VulkanDestory();
        MORP_CORE_WARN("[VULKAN] Instance Was Destoryed!");
	}

    void VulkanInstance::VulkanCreate()
    {
        m_Validation = new VulkanValidation();

        Vector<vk::ExtensionProperties> installedExtensions = vk::enumerateInstanceExtensionProperties();

        Vector<Extention> wantedExtensions =
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
        #ifdef VK_USE_PLATFORM_WIN32_KHR
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
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

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        CreateInfo.enabledLayerCount = m_Validation->GetValidationSize();
        CreateInfo.ppEnabledLayerNames = m_Validation->GetValidationData();
        m_Validation->PopulateDebugMessenger(debugCreateInfo);
        CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

        m_Instance = vk::createInstance(CreateInfo);
    }

    void VulkanInstance::VulkanDestory()
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

    Ref<VulkanInstance> VulkanInstance::Make()
    {
        Ref<VulkanInstance> s_VulkanInstance = CreateRef<VulkanInstance>();
        VulkanMemoryManager::GetInstance()->GetGlobalCache()->Submit(s_VulkanInstance);
        return s_VulkanInstance;
    }

}