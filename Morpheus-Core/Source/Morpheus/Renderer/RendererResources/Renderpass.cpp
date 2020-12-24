#include "Morppch.h"
#include "Renderpass.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
//#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

namespace Morpheus {

	Ref<Renderpass> Renderpass::Create(const RenderpassLayout& _Layout)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			//case RendererAPI::API::Vulkan:  return VulkanRenderpass::Make(_Layout);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}