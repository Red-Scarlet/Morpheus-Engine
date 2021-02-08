#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Morpheus/Renderer/RendererTypes.h"

namespace Morpheus {

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32* _Indices, const uint32& _Size);
		virtual ~IndexBuffer();
		operator Resource() const { return m_Resource; }

	private:
		Resource m_Resource;

	public:
		static Ref<IndexBuffer> Create(uint32* _Indices, const uint32& _Size);
	};

}