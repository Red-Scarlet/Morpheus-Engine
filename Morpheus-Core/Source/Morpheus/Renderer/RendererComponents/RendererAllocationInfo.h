#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"

namespace Morpheus {

	enum class RendererResourceTypes : uint8
	{
		RENDERER_NONE,
		RENDERER_VERTEX_BUFFER,
		RENDERER_INDEX_BUFFER,
		RENDERER_RENDER_GRAPH
	};

	struct RendererAllocationInfo
	{
	public:
		RendererResourceTypes Type = RendererResourceTypes::RENDERER_NONE;
		Resource Next = uint32_max;
		AnyData Data = nullptr;
	};

}