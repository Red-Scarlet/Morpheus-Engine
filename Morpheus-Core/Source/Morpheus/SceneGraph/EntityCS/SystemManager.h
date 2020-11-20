#pragma once

#include "Morpheus/Core/Common.h"
#include "System.h"

#include "EntityCSTypes.h"

namespace Morpheus {

	class SystemManager
	{
	public: 
		template<typename T>
		Ref<T> RegisterSystem()
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_System.find(typeName) == m_System.end(), "Registering system more than once.");

			auto system = CreateRef<T>();
			m_System.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_System.find(typeName) != m_System.end(), "System used before registered.");
			m_Signature.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& pair : m_System) {
				auto const& system = pair.second;
				system->m_Entities.erase(entity);
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			for (auto const& pair : m_System) {
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signature[type];

				if ((entitySignature & systemSignature) == systemSignature)
					system->m_Entities.insert(entity);
				else system->m_Entities.erase(entity);
			}
		}

	private:
		UnorderedMap<const float8*, Signature> m_Signature{};
		UnorderedMap<const float8*, Ref<System>> m_System{};

	};

}