#include "Morppch.h"
#include "UniformBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
//#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"

namespace Morpheus {

	Ref<UniformBuffer> UniformBuffer::Create(const UniformAttributeLayout& _Layout)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			//case RendererAPI::API::Vulkan:  return VulkanUniformBuffer::Make(_Layout);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}


}

