#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class TextureBuffer
	{
	public:
		virtual ~TextureBuffer() = default;

		virtual const uint32& GetID() const = 0;

	public:
		static Ref<TextureBuffer> Create(const String& _Filepath);

	};

}