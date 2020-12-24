 // MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanValidation.h"
#include "VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		VULKAN_CORE_ERROR("[VULKAN] Validation Layer: " + String(pCallbackData->pMessage));
		return VK_FALSE;
	}

	VulkanValidation::VulkanValidation()
	{
		if (!CheckValidationSupport())
			MORP_CORE_ASSERT(true, "Vulkan Validation Layers not available!");
		VULKAN_CORE_WARN("[VULKAN] Validation Layer was Created!");
	}

	VulkanValidation::~VulkanValidation()
	{
		DestroyDebugMessenger(VulkanInstance::GetInstance()->GetVulkanInstance(), m_DebugMessenger, nullptr);
	}

	bool VulkanValidation::CheckValidationSupport()
	{
		uint32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		Vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const float8* layerName : m_ValidationLayers) {
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

	void VulkanValidation::SetupDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessenger(createInfo);

		VkResult result = CreateDebugMessenger(VulkanInstance::GetInstance()->GetVulkanInstance(), &createInfo, nullptr, &m_DebugMessenger);
		MORP_CORE_ASSERT(result, "Vulkan Instance Error!");
	}

	void VulkanValidation::PopulateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo)
	{
		CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		CreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		CreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		CreateInfo.pfnUserCallback = DebugCallback;
	}

	VkResult VulkanValidation::CreateDebugMessenger(const VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void VulkanValidation::DestroyDebugMessenger(const VkInstance& instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	Ref<VulkanValidation> VulkanValidation::Create()
	{
		return CreateRef<VulkanValidation>();
	}

}}