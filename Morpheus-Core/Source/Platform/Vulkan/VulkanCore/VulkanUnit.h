#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanCache.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanResource.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandPool.h"

namespace Morpheus { namespace Vulkan {

	#define VULKAN_CACHE_UNIT_TYPE VulkanCacheType(1)
	class VulkanUnit
	{
	public:
		typedef VulkanCacheMember<VulkanUnit> UnitCache;

	public:
		VulkanUnit(const Ref<VulkanDevice>& _Device);
		~VulkanUnit();

		void Process();
		void MakeResource(const Ref<VulkanResource>& _Resource);
		void MakeResources(const Vector<Ref<VulkanResource>>& _Resources); // TODO: MAKE TUPLE, A FOR FASTER!

		void SetThreadID(const ThreadID& _ID);
		const ThreadID& GetThreadID();

		const uint32& GetWorkID() { return m_WorkID; }
		void SetWorkID(const uint32& _WorkID) { m_WorkID = _WorkID; }

	private:
		void CreateUnit();
		void DestroyUnit();

	private:
		Ref<VulkanDevice> m_Device;
		ThreadID m_ThreadID;
		uint32 m_WorkID = 0;

		Ref<VulkanCommandPool> m_CommandPool;
		Ref<ResourceDispatcher> m_ResourceDispatcher;	//	TODO: Make Shared
		Vector<Ref<VulkanResource>> m_Resources;

	public:
		static Ref<VulkanUnit> Create(const Ref<VulkanDevice>& _Device);
	};

	static Ref<VulkanUnit> FindSmallestWorkLoad(const Ref<VulkanUnit::UnitCache>& Cache, const uint32& _Count)
	{
		// First Find return whatever value
		// Second A = 1, B = 0. return B
		

		//	a0 = 0, b0 = 0 select a0
		//	a0 = 1, b0 = 0 select b0
		//	a0 = 1, b0 = 1 break
		//	a0 = 1, b1 = 0 select b1
		//	a0 = 1, b1 = 1 break

		// a == b, break(b)
		// a > b, return(b)
		Ref<VulkanUnit> unit_a = nullptr;
		for (uint32 a = 0; a < _Count; a++) {
			unit_a = Cache->Get(a);
			for (uint32 b = 0; b < _Count; b++) {
				Ref<VulkanUnit> unit_b = Cache->Get(b);
				if (unit_a->GetWorkID() == unit_b->GetWorkID())
					goto stop;
				if (unit_a->GetWorkID() > unit_b->GetWorkID())
					return unit_b;
			stop: {}
			}
		}
		return unit_a;
	}

	static Ref<VulkanUnit> FindAvailableUnit()
	{
		Ref<VulkanUnit::UnitCache> unit_Cache = VulkanCache<VulkanUnit>::Get(VULKAN_CACHE_UNIT_TYPE);
		Ref<VulkanUnit> unit = FindSmallestWorkLoad(unit_Cache, unit_Cache->GetCount() - 1);
		ThreadID TID = unit->GetThreadID();
		return unit;
	}

}}