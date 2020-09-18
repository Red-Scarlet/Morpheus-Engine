#include "Morppch.h"
#include "VertexBuffer.h"

#include "Morpheus/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanVertexBuffer.h"

namespace Morpheus {

	

	Ref<VertexBuffer> VertexBuffer::Create(const Vector<Vertex>& _Vertices)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanVertexBuffer>(_Vertices);
		}

		MORP_CORE_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}