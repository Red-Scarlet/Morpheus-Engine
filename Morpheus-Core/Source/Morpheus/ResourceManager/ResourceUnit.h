#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Base.h"

#include "Morpheus/ResourceManager/ResourceSystem.h"

namespace Morpheus {

	// MT-CLASS
	class ResourceUnit : public Base
	{
	public:
		ResourceUnit(const uint32& _ThreadID);
		virtual ~ResourceUnit();

		virtual void Init() override;
		virtual void Run() override;
		virtual void Stop() override;

	public:
		void AddSystem(const Ref<ResourceSystem>& System);

		uint32 GetSystemCount()
		{ return m_SystemCount; }
		uint32 GetThreadingID()
		{ return m_ThreadID; }

	private:
		Vector<Ref<ResourceSystem>> m_Systems;
		uint32 m_SystemCount;
		uint32 m_ThreadID;

	public:
		static Ref<ResourceUnit> Create(const uint32& _ThreadID);
	};

	class ResourceUnitCache
	{
	public:
		typedef Tuple<uint32, uint32> ResourceIndex;

		static const Ref<ResourceUnit>& Get(const uint32& _Index)
		{
			MORP_PROFILE_FUNCTION();

			auto It = s_Cache.find(_Index);
			if (It != s_Cache.end())
				return It->second;
			s_Cache[_Index] = ResourceUnit::Create(_Index);
			return Get(_Index);
		}

		static const Ref<ResourceUnit>& GetAvailable()
		{
			MORP_PROFILE_FUNCTION();

			//return Get(0);

			Vector<ResourceIndex> Indices;
			for (auto unit : s_Cache)
				Indices.emplace_back(std::make_pair<uint32, uint32>(unit.second->GetThreadingID(), unit.second->GetSystemCount()));
			std::sort(Indices.begin(), Indices.end(), CompareResourceIndex);
			return Get(std::get<0>(Indices.back()));
		}

	private:
		static bool CompareResourceIndex(ResourceIndex First, ResourceIndex Second)
		{ return (std::get<1>(First) > std::get<1>(Second)); }

	private:
		static UnorderedMap<uint32, Ref<ResourceUnit>> s_Cache;
	};
}