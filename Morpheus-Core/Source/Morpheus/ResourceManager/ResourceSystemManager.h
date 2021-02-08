#pragma once

#include "Morpheus/Core/Common.h"
#include "ResourceSystem.h"
#include "ResourceTypes.h"

namespace Morpheus {

	class ResourceSystemManager
	{
	public: 
		template<typename T>
		Ref<T> RegisterSystem()
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_System.find(typeName) == m_System.end(), "[RESOURCE] Registering system more than once.");

			auto system = CreateRef<T>();
			m_System.insert({ typeName, system });
			return system;
		}

		template<typename T>
		Ref<T> RegisterSystem(const Ref<T>& _System)
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_System.find(typeName) == m_System.end(), "[RESOURCE] Registering system more than once.");

			m_System.insert({ typeName, _System });
			return _System;
		}


		template<typename T>
		void SetSignature(const ResourceSignature& _Signature)
		{
			const float8* typeName = typeid(T).name();
			MORP_CORE_ASSERT(m_System.find(typeName) != m_System.end(), "[RESOURCE] System used before registered.");
			m_Signature.insert({ typeName, _Signature });
		}

		void DestroyResource(const Resource& _Resource)
		{
			for (auto const& pair : m_System) {
				auto const& system = pair.second;
				system->m_Resources.erase(_Resource);
			}
		}

		void ResourceSignatureChanged(const Resource& _Resource, const ResourceSignature& _ResourceSignature)
		{
			for (auto const& pair : m_System) {
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signature[type];

				if ((_ResourceSignature & systemSignature) == systemSignature)
					system->m_Resources.insert(_Resource);
				else system->m_Resources.erase(_Resource);
			}
		}

	private:
		UnorderedMap<const float8*, ResourceSignature> m_Signature = {};
		UnorderedMap<const float8*, Ref<ResourceSystem>> m_System = {};

	};

}