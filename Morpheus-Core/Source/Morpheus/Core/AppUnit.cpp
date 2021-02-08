#include "Morppch.h"
#include "AppUnit.h"

#include "Morpheus/Core/Application.h"

namespace Morpheus {

	AppUnit::AppUnit(const uint32& _ThreadID)
		: m_Running(true), m_ThreadID(_ThreadID)
	{
	}

	AppUnit::~AppUnit()
	{
	}

	void AppUnit::Init()
	{
		MORP_PROFILE_FUNCTION();

		m_ResourceUnit = ResourceUnitCache::Get(m_ThreadID);
		m_ResourceUnit->Init();
	}

	void AppUnit::Run()
	{
		MORP_PROFILE_FUNCTION();

		std::stringstream ss;
		ss << std::this_thread::get_id();
		MORP_CORE_SPECIAL("AppUnit #" + ToString(m_ThreadID) + " Running on TID: " + ss.str());

		//m_ResourceUnit->Init();

		while (m_Running) 
		{
			m_ResourceUnit->Run();
		}

		m_ResourceUnit->Stop();
	}

	void AppUnit::Stop()
	{
		MORP_PROFILE_FUNCTION();
		m_Running = false;
	}

	Ref<AppUnit> AppUnit::Create(const uint32& _ThreadID)
	{
		return CreateRef<AppUnit>(_ThreadID);
	}

}