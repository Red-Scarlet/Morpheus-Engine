#include "Morppch.h"
#include "Renderpass.h"

#include "Morpheus/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"

namespace Morpheus {

	Ref<Renderpass> Renderpass::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanRenderpass>();
		}

		MORP_CORE_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}

