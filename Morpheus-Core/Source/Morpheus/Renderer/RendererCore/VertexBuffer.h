#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	struct Vertex
	{
	public:
		Vector3 Position;
		Vector4 Color;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

	public:
		static Ref<VertexBuffer> Create(const Vector<Vertex>& _Vertices);
	};

}