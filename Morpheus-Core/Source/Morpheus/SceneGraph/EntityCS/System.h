#pragma once

#include "Morpheus/Core/Common.h"
#include "EntityCSTypes.h"

#include <set>

namespace Morpheus {

	class System
	{
	public:
		std::set<Entity> m_Entities;
	};

}