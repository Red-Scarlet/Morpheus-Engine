#include "Morppch.h"
#include "FrameBuffer.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"

namespace Morpheus {

	Ref<FrameBuffer> FrameBuffer::Create(const RenderpassLayout& _Layout)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanFrameBuffer::Make(_Layout);
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}