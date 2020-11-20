#pragma once

#include "Morpheus/Core/Common.h"

#include "EntityCSTypes.h"
#include "ComponentArray.h"

namespace Morpheus {

	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_ComponentTypes.find(typeName) == m_ComponentTypes.end(), "Registering component type more than once.");

			m_ComponentTypes.insert({ typeName, m_NextComponentType });
			m_ComponentArrays.insert({ typeName, CreateRef<ComponentArray<T>>() });
			++m_NextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use");
			return m_ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& pair : m_ComponentArrays) {
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

	private:
		UnorderedMap<const float8*, ComponentType> m_ComponentTypes{};
		UnorderedMap<const float8*, Ref<IComponentArray>> m_ComponentArrays{};
		ComponentType m_NextComponentType{};

		template<typename T>
		Ref<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();
			CC_CORE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component not registered before use.");
			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}
			
	};

}