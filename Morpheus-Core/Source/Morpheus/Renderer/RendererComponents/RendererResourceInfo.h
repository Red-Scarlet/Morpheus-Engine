#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"

namespace Morpheus {

	struct RendererResourceInfo
	{
	public:
		RendererResourceTypes Type = RendererResourceTypes::RENDERER_NONE;
		uint32 Changed = 0;
	};

}