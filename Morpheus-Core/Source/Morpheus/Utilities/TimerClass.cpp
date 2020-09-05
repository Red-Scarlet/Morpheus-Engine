#include "Morppch.h"
#include "TimerClass.h"

namespace Morpheus {

	TimerClass* TimerClass::s_Instance = nullptr;

	TimerClass::TimerClass()
	{
		Reset();
		m_TimeScale = 1.00f;
		m_DeltaTime = std::chrono::duration<FLOAT64>(0.00f);
	}

	TimerClass::~TimerClass()
	{
		delete s_Instance;
	}

	TimerClass* TimerClass::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new TimerClass();
		return s_Instance;
	}

	void TimerClass::Reset()
	{
		m_StartTime = std::chrono::system_clock::now();
	}

	void TimerClass::Tick()
	{
		m_DeltaTime = std::chrono::system_clock::now() - m_StartTime;
	}

	void TimerClass::SetTimeScale(const FLOAT64& _Scale)
	{ m_TimeScale = _Scale; }

	const FLOAT64& TimerClass::GetTimeScale()
	{ return m_TimeScale; }

	const FLOAT64& TimerClass::GetDeltaTime()
	{ return m_DeltaTime.count(); }

}