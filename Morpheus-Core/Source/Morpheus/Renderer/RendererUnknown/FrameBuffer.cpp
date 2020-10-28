#include "Morppch.h"
#include "FrameBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/VULKAN/VulkanResources/VulkanFramebuffer.h"

namespace Morpheus {

	Ref<FrameBuffer> FrameBuffer::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanFramebuffer::VulkanCreate();
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}