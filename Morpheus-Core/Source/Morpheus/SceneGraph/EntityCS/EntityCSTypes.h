#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	using Entity = uint32;
	const Entity MAX_ENTITIES = 5000;

	using ComponentType = uint8;
	const ComponentType MAX_COMPONENTS = 32;

	using Signature = Bitset<MAX_COMPONENTS>;

}