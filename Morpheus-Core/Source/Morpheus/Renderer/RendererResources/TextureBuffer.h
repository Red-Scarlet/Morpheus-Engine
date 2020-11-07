#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class TextureBuffer
	{
	public:
		virtual ~TextureBuffer() = default;

		virtual const uint32& GetID() = 0;

	public:
		static Ref<TextureBuffer> Create();

	};

}