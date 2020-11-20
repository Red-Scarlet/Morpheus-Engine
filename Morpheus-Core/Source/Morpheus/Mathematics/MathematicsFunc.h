#pragma once

#include "Morpheus/Core/Common.h"

#define MORP_PI 3.14159265358f

namespace Morpheus {

	inline floatm ToRadians(floatm _Degrees)
	{
		return (floatm)(_Degrees * (MORP_PI / 180.0f));
	}

	inline floatm ToDegrees(floatm _Radians)
	{
		return (floatm)(_Radians * (180.0f / MORP_PI));
	}

}
