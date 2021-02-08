#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererPipeline/RenderTask.h"

namespace Morpheus {

	struct RendererRenderTaskInfo
	{
	public:
		Vector<Tuple<String, RenderTaskLayout>> RenderTasks;
	};

}