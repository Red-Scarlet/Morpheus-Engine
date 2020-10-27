#include "Morppch.h"
#include "GraphicsPipeline.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/VULKAN/VulkanResources/VulkanPipeline.h"

namespace Morpheus {

	Ref<GraphicsPipeline> GraphicsPipeline::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanPipeline::VulkanCreate();
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}
