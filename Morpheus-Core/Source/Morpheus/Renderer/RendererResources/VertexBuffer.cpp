#include "Morppch.h"
#include "VertexBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"

namespace Morpheus {

	Ref<VertexBuffer> VertexBuffer::Create(QuadVertex* _Data, const uint32& _Size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanVertexBuffer::Make(_Data, _Size);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const uint32& _Size)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanVertexBuffer::Make(_Size);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}