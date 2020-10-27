#include "Morppch.h"
#include "VertexBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/VULKAN/VulkanResources/VulkanVertexBuffer.h"

namespace Morpheus {

	Ref<VertexBuffer> VertexBuffer::Create(VertexData* _VertexData, const uint32& _Size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanVertexBuffer::VulkanCreate(_VertexData, _Size);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}