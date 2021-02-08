#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Flush() = 0;

	public:
		static Scope<GraphicsContext> Create();
	};

}