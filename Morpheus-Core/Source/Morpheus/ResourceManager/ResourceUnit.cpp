#include "Morppch.h"
#include "ResourceUnit.h"

namespace Morpheus {

	UnorderedMap<uint32, Ref<ResourceUnit>> ResourceUnitCache::s_Cache = UnorderedMap<uint32, Ref<ResourceUnit>>();

	ResourceUnit::ResourceUnit(const uint32& _ThreadID)
		: m_SystemCount(0), m_ThreadID(_ThreadID)
	{
		MORP_CORE_WARN("[RESOURCE] ResourceUnit #" + ToString(m_ThreadID) + " has been Created!");
	}

	ResourceUnit::~ResourceUnit()
	{
	}

	void ResourceUnit::Init()
	{
		MORP_PROFILE_FUNCTION();

		ResourceUnitCache::Get(m_ThreadID);
	}

	void ResourceUnit::Run()
	{
		MORP_PROFILE_FUNCTION();
		for (Ref<ResourceSystem>& system : m_Systems)
			system->Update();
	}

	void ResourceUnit::Stop()
	{
		MORP_PROFILE_FUNCTION();
	}

	void ResourceUnit::AddSystem(const Ref<ResourceSystem>& System)
	{
		MORP_PROFILE_FUNCTION();

		m_Systems.push_back(System);
		m_SystemCount++;
	}

	Ref<ResourceUnit> ResourceUnit::Create(const uint32& _ThreadID)
	{
		return CreateRef<ResourceUnit>(_ThreadID);
	}
}

