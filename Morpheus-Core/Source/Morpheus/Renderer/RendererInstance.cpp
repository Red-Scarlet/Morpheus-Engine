#include "Morppch.h"
#include "RendererInstance.h"

#include "RendererAPI.h"
#include "Platform/Vulkan/VulkanInstance.h"

namespace Morpheus {

	RendererInstance* RendererInstance::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPIs::None: return nullptr;
			case RendererAPIs::Vulkan: return new VulkanInstance();
		}
	}
	
}