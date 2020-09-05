#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class DeltaTime
	{
	public:
		DeltaTime(FLOAT64 _Time = 0.00f)
			: m_Time(_Time)
		{
		}

		FLOAT64 GetSeconds() const { return m_Time; }
		FLOAT64 GetMilliseconds() const { return m_Time * 1000; }

		operator FLOAT64() const { return m_Time; }

	private:
		FLOAT64 m_Time;

	};

}