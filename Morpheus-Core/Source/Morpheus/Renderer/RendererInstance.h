#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class RendererInstance
	{
	public:
		virtual ~RendererInstance() = default;

	public:
		static RendererInstance* Create();
	};

}