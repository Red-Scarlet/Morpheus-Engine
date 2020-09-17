#include "Morppch.h"
#include "Pipeline.h"

#include "Morpheus/Renderer/RendererAPI.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanGraphicsPipeline.h"

namespace Morpheus {

	Ref<Pipeline> Pipeline::Create(const Ref<Renderpass>& _Renderpass)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    MORP_CORE_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::Vulkan:  return CreateRef<VulkanGraphicsPipeline>(_Renderpass);
		}

		MORP_CORE_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}

