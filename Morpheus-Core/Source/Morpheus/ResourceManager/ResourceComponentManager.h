#pragma once

#include "Morpheus/Core/Common.h"
#include "ResourceComponentArray.h"
#include "ResourceTypes.h"

namespace Morpheus {

	class ResourceComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const float8* typeName = typeid(T).name();
			if (m_ComponentTypes.find(typeName) == m_ComponentTypes.end()) {
				m_ComponentTypes.insert({ typeName, m_NextComponentType });
				m_ComponentArrays.insert({ typeName, CreateRef<ResourceComponentArray<T>>() });
				++m_NextComponentType;
			}
			//MORP_CORE_ASSERT(m_ComponentTypes.find(typeName) == m_ComponentTypes.end(), "[RESOURCE] Registering component type more than once.");
		}

		template<typename T>
		ResourceComponent GetComponentType()
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "[RESOURCE] Component not registered before use");
			return m_ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(const Resource& _Resource, T _Component)
		{
			GetComponentArray<T>()->InsertComponent(_Resource, _Component);
		}

		template<typename T>
		void RemoveComponent(const Resource& _Resource)
		{
			GetComponentArray<T>()->RemoveComponent(_Resource);
		}

		template<typename T>
		T& GetComponent(const Resource& _Resource)
		{
			return GetComponentArray<T>()->GetComponent(_Resource);
		}

		template<typename T>
		bool ContainsComponent(const Resource& _Resource)
		{
			return GetComponentArray<T>()->ContainsComponent(_Resource);
		}

		void DestroyResource(const Resource& _Resource)
		{
			for (auto const& pair : m_ComponentArrays) {
				auto const& component = pair.second;
				component->DestroyResource(_Resource);
			}
		}

	private:
		UnorderedMap<const float8*, ResourceComponent> m_ComponentTypes = {};
		UnorderedMap<const float8*, Ref<IResourceComponentArray>> m_ComponentArrays = {};
		ResourceComponent m_NextComponentType = {};
					
		template<typename T>
		Ref<ResourceComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "[RESOURCE] Component not registered before use.");
			return std::static_pointer_cast<ResourceComponentArray<T>>(m_ComponentArrays[typeName]);
		}
			
	};

}