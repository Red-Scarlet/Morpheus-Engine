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

		void SetTimeScale(const float64& _Scale = 1.00f);
		const float64& GetTimeScale();
		const float64& GetDeltaTime();

	private:
		static TimerClass* s_Instance;
		std::chrono::system_clock::time_point m_StartTime;
		std::chrono::duration<float64> m_DeltaTime;
		float64 m_TimeScale;

	};

}