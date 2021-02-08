#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	using Resource = uint32;
	const Resource MaxResources = 64;

	using ResourceComponent = uint8;
	const ResourceComponent MaxResourceComponents = 32;
	using ResourceSignature = Bitset<MaxResourceComponents>;

}