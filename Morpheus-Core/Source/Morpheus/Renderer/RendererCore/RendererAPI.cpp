#include "Morppch.h"
#include "RendererAPI.h"

#include "Platform/VULKAN/VulkanCore/VulkanRendererAPI.h"

namespace Morpheus {

	RendererAPI::API RendererAPI::s_RenderAPI = RendererAPI::API::Vulkan;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_RenderAPI)
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return CreateScope<VulkanRendererAPI>();
		}

		MORP_CORE_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}