#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererUnknown/Renderpass.h"

namespace Morpheus {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

	public:
		static Ref<FrameBuffer> Create(const Ref<Renderpass> _Renderpass);
	};

}