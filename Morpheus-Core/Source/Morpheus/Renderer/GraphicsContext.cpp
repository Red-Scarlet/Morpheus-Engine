#include "Morppch.h"
#include "GraphicsContext.h"

#include "RendererAPI.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Morpheus {

	GraphicsContext* GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIs::None: return nullptr;
			case RendererAPIs::Vulkan: return new VulkanContext();
		}
	}
	
}