#include "Morppch.h"
#include "IndexBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/Vulkan/VulkanResource/VulkanIndexBuffer.h"

namespace Morpheus {

	Ref<IndexBuffer> IndexBuffer::Create(uint32* _Indices, const uint32& _Size)
	{
		using namespace Vulkan;

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanIndexBuffer::Make(_Indices, _Size);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}
