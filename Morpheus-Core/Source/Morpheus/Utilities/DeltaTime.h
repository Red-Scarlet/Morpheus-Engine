#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class DeltaTime
	{
	public:
		DeltaTime(float64 _Time = 0.00f)
			: m_Time(_Time)
		{
		}

		float64 GetSeconds() const { return m_Time; }
		float64 GetMilliseconds() const { return m_Time * 1000; }

		operator float64() const { return m_Time; }

	private:
		float64 m_Time;

	};

}