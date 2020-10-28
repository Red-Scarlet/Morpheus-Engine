#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

	public:
		static Ref<FrameBuffer> Create();
	};

}