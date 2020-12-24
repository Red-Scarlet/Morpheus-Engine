#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

	public:
		static Ref<IndexBuffer> Create(uint32* _Indices, const uint32& _Size);
	};

}