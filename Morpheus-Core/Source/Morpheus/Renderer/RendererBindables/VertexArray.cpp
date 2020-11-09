#include "Morppch.h"
#include "VertexArray.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"

namespace Morpheus {

	Ref<VertexArray> Morpheus::VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanVertexArray::Make();
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}


