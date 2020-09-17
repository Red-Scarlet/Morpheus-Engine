#include "Morppch.h"
#include "Framebuffer.h"

#include "Morpheus/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanFramebuffer.h"

namespace Morpheus {

	Ref<Framebuffer> Framebuffer::Create(const Ref<Renderpass>& _Renderpass)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return CreateRef<VulkanFramebuffer>(_Renderpass);
		}

		MORP_CORE_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}
