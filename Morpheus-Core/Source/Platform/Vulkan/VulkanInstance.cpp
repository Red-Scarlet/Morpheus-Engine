#include "Morppch.h"
#include "VulkanInstance.h"

#include "Morpheus/Core/Application.h"

namespace Morpheus {

	VulkanInstance::VulkanInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		Application& app = Application::Get();
		createInfo.enabledExtensionCount = app.GetWindow().GetExtensionsCount();
		createInfo.ppEnabledExtensionNames = app.GetWindow().GetExtensionsData();

		createInfo.enabledLayerCount = 0;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
			MORP_CORE_ERROR("Failed to create Vulkan Instance!");

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		Vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		for (const auto& extension : extensions)
			MORP_CORE_INFO(extension.extensionName);


	}

	VulkanInstance::~VulkanInstance()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

}