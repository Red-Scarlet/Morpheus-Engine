#include "Morppch.h"
#include "VulkanMemoryManager.h"

namespace Morpheus {

	VulkanMemoryManager* VulkanMemoryManager::s_Instance = nullptr;

	VulkanMemoryManager::VulkanMemoryManager()
	{
		Init();
	}

	VulkanMemoryManager::~VulkanMemoryManager()
	{
		Shutdown();
	}

	VulkanMemoryManager* VulkanMemoryManager::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new VulkanMemoryManager();
		return s_Instance;
	}

	void VulkanMemoryManager::Shutdown()
	{
		MORP_CORE_WARN("[VULKAN] VulkanMemoryManager Was Destoryed!");
	}

	void VulkanMemoryManager::Init()
	{
		m_Globals = CreateRef<VulkanGlobalCache>();
		m_Resources = CreateRef<VulkanResourceCache>();
		m_Bindables = CreateRef<VulkanBindableCache>();

		MORP_CORE_WARN("[VULKAN] VulkanMemoryManager Was Created!");
	}

}