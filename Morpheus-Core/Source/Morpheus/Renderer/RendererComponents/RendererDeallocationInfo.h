#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"

namespace Morpheus {

	enum RendererResourceFlags : uint8
	{
		RENDERER_NONE = 0x00,
		RENDERER_REMOVE_RESOURCE = 0x01
	};

	struct RendererDeallocationInfo
	{
	public:
		RendererResourceTypes Type = RendererResourceTypes::RENDERER_NONE;
		RendererResourceFlags Flags = RendererResourceFlags::RENDERER_NONE;
	};

}