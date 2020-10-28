#include "Morppch.h"
#include "VulkanResource.h"

namespace Morpheus {

	VulkanResourceCache* VulkanResourceCache::s_Instance = nullptr;

	VulkanResourceCache::VulkanResourceCache()
	{
		Init();
	}

	VulkanResourceCache::~VulkanResourceCache()
	{
		Shutdown();
	}

	VulkanResourceCache* VulkanResourceCache::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new VulkanResourceCache();
		return s_Instance;
	}

	void VulkanResourceCache::Shutdown()
	{
		MORP_CORE_WARN("[VULKAN] VulkanResourceCache Was Destoryed!");
	}

	void VulkanResourceCache::Init()
	{
		MORP_CORE_WARN("[VULKAN] VulkanResourceCache Was Created!");
	}

}