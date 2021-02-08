#pragma once

#include "Morpheus/Core/Common.h"

#include "ResourceTypes.h"
#include "ResourceManager.h"
#include "ResourceComponentManager.h"
#include "ResourceSystemManager.h"

namespace Morpheus {

	class ResourceCoordinator
	{
	public:
		ResourceCoordinator();
		~ResourceCoordinator();

	public:
		Resource CreateResource();
		void DestroyResource(const Resource& _Resource);

		template<typename T>
		void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(const Resource& _Resource, T _Component)
		{
			if (ContainsComponent<T>(_Resource) == false) {
				m_ComponentManager->AddComponent<T>(_Resource, _Component);
				auto signature = m_ResourceManager->GetSignature(_Resource);
				signature.set(m_ComponentManager->GetComponentType<T>(), true);
				m_ResourceManager->SetSignature(_Resource, signature);
				m_SystemManager->ResourceSignatureChanged(_Resource, signature);
			}
		}

		template<typename T>
		void AddComponentAll(T _Component)
		{
			for (const ResourceObject& resource : m_ResourceIndex)
				AddComponent<T>(resource, _Component);
		}

		template<typename T>
		void RemoveComponent(const Resource& _Resource)
		{
			m_ComponentManager->RemoveComponent<T>(_Resource);

			auto signature = m_ResourceManager->GetSignature(_Resource);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_ResourceManager->SetSignature(_Resource, signature);

			m_SystemManager->ResourceSignatureChanged(_Resource, signature);
		}

		template<typename T>
		T& GetComponent(const Resource& _Resource)
		{
			return m_ComponentManager->GetComponent<T>(_Resource);
		}

		template<typename T>
		bool ContainsComponent(const Resource& _Resource)
		{
			return m_ComponentManager->ContainsComponent<T>(_Resource);
		}

		template<typename T>
		ResourceComponent GetComponentType()
		{
			return m_ComponentManager->GetComponentType<T>();
		}

		template<typename T>
		Ref<T> RegisterSystem()
		{
			return m_SystemManager->RegisterSystem<T>();
		}

		template<typename T>
		Ref<T> RegisterSystem(const Ref<T>& _System)
		{
			return m_SystemManager->RegisterSystem<T>(_System);
		}

		template<typename T>
		void SetSystemSignature(const ResourceSignature& _Signature)
		{
			m_SystemManager->SetSignature<T>(_Signature);
		}

		uint32 GetResourceCount()
		{
			return m_ResourceCount;
		}

	private:
		Scope<ResourceManager> m_ResourceManager;
		Scope<ResourceComponentManager> m_ComponentManager;
		Scope<ResourceSystemManager> m_SystemManager;
		uint32 m_ResourceCount;
	};

}