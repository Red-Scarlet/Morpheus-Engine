#include "Morppch.h"
#include "TextureBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
//#include "Platform/Vulkan/VulkanResources/VulkanTextureBuffer.h"

namespace Morpheus {

	Ref<TextureBuffer> TextureBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			//case RendererAPI::API::Vulkan:  return VulkanTextureBuffer::VulkanCreate();
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}


}

