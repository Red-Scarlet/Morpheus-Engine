#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"

namespace Morpheus {

	struct RendererVertexArrayInfo
	{
	public:
		Resource VertexBuffer = uint32_max;
		Resource IndexBuffer = uint32_max;
	};

}