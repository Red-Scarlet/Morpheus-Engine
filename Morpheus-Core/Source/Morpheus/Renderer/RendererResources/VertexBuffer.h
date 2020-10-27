#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	struct VertexData
	{
		float32 Position[3];
		float32 Color[3];
	};

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual const uint32& GetID() = 0;

	public:
		static Ref<VertexBuffer> Create(VertexData* _VertexData, const uint32& _Size);
	};

}