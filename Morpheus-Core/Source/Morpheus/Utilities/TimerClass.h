#pragma once

#include "Morpheus/Core/Common.h"
#include <iostream>
#include <chrono>

namespace Morpheus {

	class TimerClass 
	{
	private:
		TimerClass();
		~TimerClass();

	public:
		static TimerClass* GetInstance();
		void Reset();
		void Tick();

		void SetTimeScale(const FLOAT64& _Scale = 1.00f);
		const FLOAT64& GetTimeScale();
		const FLOAT64& GetDeltaTime();

	private:
		static TimerClass* s_Instance;
		std::chrono::system_clock::time_point m_StartTime;
		std::chrono::duration<FLOAT64> m_DeltaTime;
		FLOAT64 m_TimeScale;

	};

}