#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class TextureBuffer
	{
	public:
		virtual ~TextureBuffer() = default;

	public:
		static Ref<TextureBuffer> Create();

	};

}