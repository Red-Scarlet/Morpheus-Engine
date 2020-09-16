// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"

#include "VulkanInstance.h"
#include "Morpheus/Core/Application.h"

namespace Morpheus {
	
	VulkanInstance* VulkanInstance::s_Instance = nullptr;

	VulkanInstance::VulkanInstance()
	{
		Init();
		MORP_CORE_WARN("[VULKAN] Instance Was Created!");
	}

	VulkanInstance::~VulkanInstance()
	{
		Shutdown();
	}

	VulkanPresentation* VulkanInstance::GetPresentation()
	{
		if (m_VulkanObject.Presentation == nullptr)
			m_VulkanObject.Presentation = new VulkanPresentation();
		m_VulkanObject.Presentation->SetupSurface();
		return m_VulkanObject.Presentation;
	}

	VulkanPhysicalDevice* VulkanInstance::GetPhysicalDevice()
	{
		if (m_VulkanObject.PhysicalDevice == nullptr)
			m_VulkanObject.PhysicalDevice = new VulkanPhysicalDevice();
		return m_VulkanObject.PhysicalDevice;
	}

	VulkanLogicalDevice* VulkanInstance::GetLogicalDevice()
	{
		if (m_VulkanObject.LogicalDevice == nullptr)
			m_VulkanObject.LogicalDevice = new VulkanLogicalDevice(GetPhysicalDevice());
		return m_VulkanObject.LogicalDevice;
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

		if (m_VulkanDebug.ValidationState)
			m_VulkanDebug.ValidationLayer = new VulkanValidation();

		VkApplicationInfo appInfo {};
		{
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "MorpheusEngine";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "MorpheusEngine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;
		}

		auto extensions = GetRequiredExtensions(GLFW.GetExtensionsCount(), GLFW.GetExtensionsData());
		VkInstanceCreateInfo createInfo {};
		{
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();

			if (m_VulkanDebug.ValidationState) {
				VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
				createInfo.enabledLayerCount = m_VulkanDebug.ValidationLayer->GetValidationSize();
				createInfo.ppEnabledLayerNames = m_VulkanDebug.ValidationLayer->GetValidationData();
				m_VulkanDebug.ValidationLayer->PopulateDebugMessenger(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			} else {
				createInfo.enabledLayerCount = 0;
				createInfo.pNext = nullptr;
			}
		}
		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VulkanObject.Vulkan);
		MORP_CORE_ASSERT(result, "Vulkan Instance Error!");
	}

	void VulkanInstance::Shutdown()
	{
		vkDestroyInstance(m_VulkanObject.Vulkan, nullptr);
	}

	Vector<const float8*> VulkanInstance::GetRequiredExtensions(uint32 Count, const float8** Data)
	{
		Vector<const float8*> extensions(Data, Data + Count);
		if (m_VulkanDebug.ValidationState)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		return extensions;
	}
	
}