#pragma once

#include "Morpheus/Core/Common.h"
#include "ResourceTypes.h"

namespace Morpheus {

	class ResourceSystem
	{
	public:
		virtual void Update() = 0;

	public:
		Set<Resource> m_Resources;
	};

}