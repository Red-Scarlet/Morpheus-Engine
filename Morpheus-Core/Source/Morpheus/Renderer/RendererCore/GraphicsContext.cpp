#include "Morppch.h"
#include "GraphicsContext.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/VULKAN/VulkanCore/VulkanContext.h"

namespace Morpheus {

	Scope<GraphicsContext> GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return CreateScope<VulkanContext>();
		}

		MORP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

