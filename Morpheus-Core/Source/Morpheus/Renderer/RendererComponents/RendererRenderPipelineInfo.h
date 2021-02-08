#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererPipeline/RenderPipeline.h"

namespace Morpheus {

	struct RendererRenderPipelineInfo
	{
	public:
		Vector<Tuple<String, RenderPipelineStruct>> RenderPipelines;
	};

}