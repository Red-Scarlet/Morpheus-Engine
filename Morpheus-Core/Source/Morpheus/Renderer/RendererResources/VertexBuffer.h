#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	struct QuadVertex
	{
	public:
		Vector2 Position;
		Vector2 TexCoord;
		Vector3 Color;
	};

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() = default;

	public:
		static Ref<VertexBuffer> Create(QuadVertex* _Data, const uint32& _Size);

	};

}