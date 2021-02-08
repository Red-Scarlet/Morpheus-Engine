#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Morpheus {

	class VertexArray
	{
	public:
		VertexArray();
		virtual ~VertexArray();
		const Resource& GetResource() { return m_Resource; }

		void AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer);
		void SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer);

	private:
		Resource m_Resource;

	public:
		static Ref<VertexArray> Create();
	};

}