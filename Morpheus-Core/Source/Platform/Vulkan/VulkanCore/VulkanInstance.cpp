#include "Morppch.h"
#include "VulkanInstance.h"
#include "Morpheus/Core/Application.h"

#include "Platform/Vulkan/VulkanCore/VulkanCache.h"

// TODO: REMOVE
#include <GLFW/glfw3.h>

namespace Morpheus { namespace Vulkan {
	
	Ref<VulkanInstance> VulkanInstance::s_Instance = nullptr;

	VulkanInstance::VulkanInstance()
	{
		Init();
		VULKAN_CORE_WARN("[VULKAN] Instance Was Created!");
	}

	VulkanInstance::~VulkanInstance()
	{
		Shutdown();
		VULKAN_CORE_WARN("[VULKAN] Instance Was Destroyed!");
	}

	Ref<VulkanSurface> VulkanInstance::GetSurface()
	{
		MORP_PROFILE_FUNCTION();

		if (m_Surface == nullptr)
			m_Surface = VulkanSurface::Create(m_VulkanInstance);
		return m_Surface;
	}

	Ref<VulkanDevice> VulkanInstance::GetDevice(const uint32& _DeviceID)
	{
		MORP_PROFILE_FUNCTION();

		Ref<VulkanDevice> _Device;
		Ref<VulkanDevice::DeviceCache> d_Cache = VulkanCache<VulkanDevice>::Get(VULKAN_CACHE_DEVICE_TYPE);
		if (d_Cache->Exists(_DeviceID))
			_Device = d_Cache->Get(_DeviceID);
		else _Device = VulkanDevice::Create(m_VulkanInstance, m_Surface->GetSurface());
		return _Device;
	}

	#ifdef MORP_DEBUG
	const Ref<VulkanValidation>& VulkanInstance::GetValidationLayer()
	{ return m_ValidationLayer; }
	#endif

	Ref<VulkanInstance> VulkanInstance::GetInstance()
	{	
		MORP_PROFILE_FUNCTION();

		if (s_Instance == nullptr)
			s_Instance = VulkanInstance::Create();
		return s_Instance;
	}

	void VulkanInstance::Init()
	{
		MORP_PROFILE_FUNCTION();

		Window& GLFW = Application::Get().GetWindow();

		#ifdef MORP_DEBUG
		m_ValidationLayer = VulkanValidation::Create();
		#endif

		VkApplicationInfo AppInfo = {};
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = "MorpheusEngine";
		AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		AppInfo.pEngineName = "MorpheusEngine";
		AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		AppInfo.apiVersion = VK_API_VERSION_1_0;

		auto Extensions = GetRequiredExtensions();

		#ifdef VULKAN_CORE_LOG_EXT
		for (auto ext : Extensions)
			VULKAN_CORE_INFO("Using Instance Extension: " + String(ext));
		#endif

		VkInstanceCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		CreateInfo.pApplicationInfo = &AppInfo;
		CreateInfo.enabledExtensionCount = Extensions.size();
		CreateInfo.ppEnabledExtensionNames = Extensions.data();
		CreateInfo.enabledLayerCount = 0;
		CreateInfo.pNext = nullptr;

		#ifdef MORP_DEBUG
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		CreateInfo.enabledLayerCount = m_ValidationLayer->GetValidationSize();
		CreateInfo.ppEnabledLayerNames = m_ValidationLayer->GetValidationData();
		m_ValidationLayer->PopulateDebugMessenger(debugCreateInfo);
		CreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		#endif
		
		VkResult result = vkCreateInstance(&CreateInfo, nullptr, &m_VulkanInstance);
		MORP_CORE_ASSERT(result, "Vulkan Instance Error!");
	}

	void VulkanInstance::Shutdown()
	{
		MORP_PROFILE_FUNCTION();

		Ref<VulkanDevice::DeviceCache> d_Cache = VulkanCache<VulkanDevice>::Get(VULKAN_CACHE_DEVICE_TYPE);
		d_Cache->Clear();

		m_Surface.reset();
		vkDestroyInstance(m_VulkanInstance, nullptr);
	}

	Vector<const float8*> VulkanInstance::GetRequiredExtensions()
	{
		uint32 glfwExtensionCount = 0;
		const float8** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		Vector<const float8*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		#ifdef MORP_DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		#endif

		return extensions;
	}

	Ref<VulkanInstance> VulkanInstance::Create()
	{
		return CreateScope<VulkanInstance>();
	}

}}