#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"

namespace Morpheus {

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

	public:
		static Ref<Pipeline> Create(const Ref<Renderpass>& _Renderpass);

	};

}