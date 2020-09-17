#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class Renderpass 
	{
	public:
		virtual ~Renderpass() = default;

	public:
		static Ref<Renderpass> Create();

	};

}