#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Draw() = 0;
		virtual void Stop() = 0;

	public:
		static GraphicsContext* Create();
	};

}