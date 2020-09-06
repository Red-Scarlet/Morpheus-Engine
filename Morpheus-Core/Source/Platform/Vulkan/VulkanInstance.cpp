// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#include "Morppch.h"

#include "VulkanInstance.h"
#include "Morpheus/Core/Application.h"

namespace Morpheus {

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		MORP_CORE_ERROR("[VULKAN] Validation Layer: " + String(pCallbackData->pMessage));
		return VK_FALSE;
	}	
	
	VulkanInstance* VulkanInstance::s_Instance = nullptr;

	VulkanInstance::VulkanInstance()
	{
		Init();
		MORP_CORE_WARN("[VULKAN] Vulkan Instance Was Created!");
	}

	VulkanInstance::~VulkanInstance()
	{
		Shutdown();
	}

	VulkanInstance* VulkanInstance::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new VulkanInstance();
		return s_Instance;
	}

	void VulkanInstance::Init()
	{
		Window& GLFW = Application::Get().GetWindow();

		if (m_EnableValidation && !CheckValidationSupport())
			MORP_CORE_ASSERT(true, "Vulkan Validation Layers not available!");

		// Application Information to be submitted to Vulkan Instance!
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "MorpheusEngineApplication";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "MorpheusEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// InstanceCreation Information to be submitted to Vulkan Instance!
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions(GLFW.GetExtensionsCount(), GLFW.GetExtensionsData());
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (m_EnableValidation) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
			PopulateDebugMessenger(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_IVulkan);
		MORP_CORE_ASSERT(result, "Vulkan Instance Error!");
	}

	void VulkanInstance::Shutdown()
	{
		if (m_EnableValidation)
			DestroyDebugMessenger(m_IVulkan, m_DebugMessenger, nullptr);
		vkDestroyInstance(m_IVulkan, nullptr);
	}

	bool VulkanInstance::CheckValidationSupport()
	{
		UINT32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		Vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const FLOAT8* layerName : m_ValidationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			if (!layerFound)
				return false;
		}

		return true;
	}

	FVector8 VulkanInstance::GetRequiredExtensions(UINT32 Count, const FLOAT8** Data)
	{
		Vector<const FLOAT8*> extensions(Data, Data + Count);
		if (m_EnableValidation)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		return extensions;
	}

	void VulkanInstance::SetupDebugMessenger()
	{
		if (!m_EnableValidation) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessenger(createInfo);

		VkResult result = CreateDebugMessenger(m_IVulkan, &createInfo, nullptr, &m_DebugMessenger);
		MORP_CORE_ASSERT(result, "Vulkan Instance Error!");
	}

	void VulkanInstance::PopulateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo)
	{
		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		CreateInfo.pfnUserCallback = DebugCallback;
	}

	VkResult VulkanInstance::CreateDebugMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) 
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) 
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void VulkanInstance::DestroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
}