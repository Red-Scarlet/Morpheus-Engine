#pragma once

#include "Morpheus/Core/Common.h"

#include "EntityCSTypes.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace Morpheus {

	class Coordinator
	{
	public:
		static Coordinator* GetInstance();
		Coordinator();

	public:
		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		template<typename T>
		void RegisterComponent();

		template<typename T>
		void AddComponent(Entity entity, T component);

		template<typename T>
		void RemoveComponent(Entity entity);

		template<typename T>
		T& GetComponent(Entity entity);

		template<typename T>
		ComponentType GetComponentType();

		template<typename T>
		Ref<T> RegisterSystem();

		template<typename T>
		void SetSystemSignature(Signature signature);

	public:
		static uint32 s_EntityCount;

	private:
		static Coordinator* s_Instance;

		Scope<ComponentManager> m_ComponentManager;
		Scope<EntityManager> m_EntityManager;
		Scope<SystemManager> m_SystemManager;
	};

	template<typename T>
	void Coordinator::RegisterComponent()
	{
		m_ComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void Coordinator::AddComponent(Entity entity, T component)
	{
		m_ComponentManager->AddComponent<T>(entity, component);

		auto signature = m_EntityManager->GetSignature(entity);
		signature.set(m_ComponentManager->GetComponentType<T>(), true);
		m_EntityManager->SetSignature(entity, signature);

		m_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void Coordinator::RemoveComponent(Entity entity)
	{
		m_ComponentManager->RemoveComponent<T>(entity);

		auto signature = m_EntityManager->GetSignature(entity);
		signature.set(m_ComponentManager->GetComponentType<T>(), false);
		m_EntityManager->SetSignature(entity, signature);

		m_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& Coordinator::GetComponent(Entity entity)
	{
		return m_ComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType Coordinator::GetComponentType()
	{
		return m_ComponentManager->GetComponentType<T>();
	}

	template<typename T>
	Ref<T> Coordinator::RegisterSystem()
	{
		return m_SystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void Coordinator::SetSystemSignature(Signature signature)
	{
		m_SystemManager->SetSignature<T>(signature);
	}

}