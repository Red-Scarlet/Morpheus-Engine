#include "Morppch.h"
#include "Coordinator.h"

namespace Morpheus {

	Coordinator* Coordinator::s_Instance = nullptr;

	uint32_t Coordinator::s_EntityCount = 0;

	Coordinator* Coordinator::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new Coordinator();
		return s_Instance;
	}

	Coordinator::Coordinator()
	{
		m_ComponentManager = CreateScope<ComponentManager>();
		m_EntityManager = CreateScope<EntityManager>();
		m_SystemManager = CreateScope<SystemManager>();
	}

	Entity Coordinator::CreateEntity()
	{
		s_EntityCount++;
		return m_EntityManager->CreateEntity();
	}

	void Coordinator::DestroyEntity(Entity entity)
	{
		s_EntityCount--;
		m_EntityManager->DestroyEntity(entity);
		m_ComponentManager->EntityDestroyed(entity);
		m_SystemManager->EntityDestroyed(entity);
	}

}