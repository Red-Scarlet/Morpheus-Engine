#pragma once

#include "Morpheus/Core/Common.h"
#include "ResourceTypes.h"

namespace Morpheus {

	class ResourceManager
	{
	public:
		ResourceManager()
		{
			for (Resource resource = 0; resource < MaxResources; ++resource)
				m_AvailableResources.push(resource);
		}

		Resource CreateResource()
		{
			MORP_CORE_ASSERT(m_LivingResourceCount < MaxResources, "[RESOURCE] Too many Resources in existence.");
			Resource id = m_AvailableResources.front();
			m_AvailableResources.pop();
			++m_LivingResourceCount;
			return id;
		}

		void DestroyResource(const Resource& _Resource)
		{
			MORP_CORE_ASSERT(_Resource < MaxResources, "[RESOURCE] Resource out of range.");
			m_Signatures[_Resource].reset();

			m_AvailableResources.push(_Resource);
			--m_LivingResourceCount;
		}

		void SetSignature(const Resource& _Resource, const ResourceSignature& _Signature)
		{
			MORP_CORE_ASSERT(_Resource < MaxResources, "[RESOURCE] Resource out of range.");
			m_Signatures[_Resource] = _Signature;
		}

		ResourceSignature GetSignature(const Resource& _Resource)
		{
			MORP_CORE_ASSERT(_Resource < MaxResources, "[RESOURCE] Resource out of range.");
			return m_Signatures[_Resource];
		}

	private:
		Queue<Resource> m_AvailableResources = {};
		Array<ResourceSignature, MaxResources> m_Signatures = {};
		uint32 m_LivingResourceCount = {};
		uint32 m_ThreadID;
	};

}