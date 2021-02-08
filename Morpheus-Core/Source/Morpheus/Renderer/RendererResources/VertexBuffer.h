#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererTypes.h"

namespace Morpheus {

	class VertexBuffer
	{
	public:
		VertexBuffer(float* _Vertices, const uint32& _Size);
		virtual ~VertexBuffer();
		operator Resource() const { return m_Resource; }

	private:
		Resource m_Resource;

	public:
		static Ref<VertexBuffer> Create(float* _Vertices, const uint32& _Size);
	};

}