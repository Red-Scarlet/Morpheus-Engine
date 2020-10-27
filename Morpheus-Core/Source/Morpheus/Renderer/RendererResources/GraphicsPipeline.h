#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class GraphicsPipeline
	{
	public:
		virtual ~GraphicsPipeline() = default;

	public:
		static Ref<GraphicsPipeline> Create();
	};

}