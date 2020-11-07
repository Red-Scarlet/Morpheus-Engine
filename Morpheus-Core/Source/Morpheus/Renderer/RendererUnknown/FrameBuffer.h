#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererUnknown/Renderpass.h"

namespace Morpheus {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void* GetImage(uint32 id) = 0;

	public:
		static Ref<FrameBuffer> Create(const Ref<Renderpass> _Renderpass);
	};

}