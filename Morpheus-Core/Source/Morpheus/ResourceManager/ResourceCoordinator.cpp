#include "Morppch.h"
#include "ResourceCoordinator.h"

namespace Morpheus {

	ResourceCoordinator::ResourceCoordinator()
	{
		m_ResourceManager = CreateScope<ResourceManager>();
		m_ComponentManager = CreateScope<ResourceComponentManager>();
		m_SystemManager = CreateScope<ResourceSystemManager>();
		m_ResourceCount = 0;
	}

	ResourceCoordinator::~ResourceCoordinator()
	{
		m_ResourceManager.reset();
		m_ComponentManager.reset();
		m_SystemManager.reset();
		m_ResourceCount = 0;
	}

	Resource ResourceCoordinator::CreateResource()
	{
		MORP_PROFILE_FUNCTION();
		m_ResourceCount++;
		return m_ResourceManager->CreateResource();
	}

	void ResourceCoordinator::DestroyResource(const Resource& _Resource)
	{
		MORP_PROFILE_FUNCTION();

		m_ResourceCount--;
		m_ResourceManager->DestroyResource(_Resource);
		m_ComponentManager->DestroyResource(_Resource);
		m_SystemManager->DestroyResource(_Resource);
	}

}