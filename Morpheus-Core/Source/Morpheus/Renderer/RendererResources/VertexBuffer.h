#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	struct QuadVertex
	{
		Vector2 Position;
		Vector2 TexCoord;
		Vector3 Color;
	};

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void SetData(const Memory32& _Data, const uint32& _Size) = 0;
		virtual const uint32& GetID() const = 0;

	public:
		static Ref<VertexBuffer> Create(QuadVertex* _Data, const uint32& _Size);
		static Ref<VertexBuffer> Create(const uint32& _Size);

	};

}