#pragma once

#include "Morpheus/Core/Common.h"
#include "ResourceTypes.h"

namespace Morpheus {

	class IResourceComponentArray
	{
	public:
		virtual ~IResourceComponentArray() = default;
		virtual void DestroyResource(const Resource& _Resource) = 0;
	};

	template<typename T>
	class ResourceComponentArray : public IResourceComponentArray
	{
	public:
		void InsertComponent(const Resource& _Resource, T _Component)
		{
			MORP_CORE_ASSERT(m_ResourceToIndexMap.find(_Resource) == m_ResourceToIndexMap.end(), "[RESOURCE] Component added to same entity more than once.");

			uint32 newIndex = m_Size;
			m_ResourceToIndexMap[_Resource] = newIndex;
			m_IndexToResourceMap[newIndex] = _Resource;
			m_ComponentArray[newIndex] = _Component;
			++m_Size;
		}

		void RemoveComponent(const Resource& _Resource)
		{
			MORP_CORE_ASSERT(m_ResourceToIndexMap.find(_Resource) != m_ResourceToIndexMap.end(), "[RESOURCE] Removing non-existent component.");

			size_t indexOfRemovedResource = m_ResourceToIndexMap[_Resource];
			size_t indexOfLastElement = m_Size - 1;
			m_ComponentArray[indexOfRemovedResource] = m_ComponentArray[indexOfLastElement];

			Resource resourceOfLastElement = m_IndexToResourceMap[indexOfLastElement];
			m_ResourceToIndexMap[resourceOfLastElement] = indexOfRemovedResource;
			m_IndexToResourceMap[indexOfRemovedResource] = resourceOfLastElement;

			m_ResourceToIndexMap.erase(_Resource);
			m_IndexToResourceMap.erase(indexOfLastElement);

			--m_Size;
		}

		T& GetComponent(const Resource& _Resource)
		{
			const float8* typeName = typeid(T).name();

			MORP_CORE_ASSERT(m_ResourceToIndexMap.find(_Resource) != m_ResourceToIndexMap.end(), "[RESOURCE] Retrieving non-existent component.");
			return m_ComponentArray[m_ResourceToIndexMap[_Resource]];
		}

		bool ContainsComponent(const Resource& _Resource)
		{
			return m_ResourceToIndexMap.find(_Resource) != m_ResourceToIndexMap.end();
		}

		void DestroyResource(const Resource& _Resource) override
		{
			if (m_ResourceToIndexMap.find(_Resource) != m_ResourceToIndexMap.end())
				RemoveComponent(_Resource);
		}

	private:
		Array<T, MaxResources> m_ComponentArray;
		UnorderedMap<Resource, uint32> m_ResourceToIndexMap;
		UnorderedMap<uint32, Resource> m_IndexToResourceMap;
		uint32 m_Size;
	};

}