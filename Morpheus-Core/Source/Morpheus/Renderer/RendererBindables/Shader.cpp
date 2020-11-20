#include "Morppch.h"
#include "Shader.h"

#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"

namespace Morpheus {

	Ref<Shader> Shader::Create(const ShaderAttributeLayout& _Layout, const String& _VertexPath, const String& _FragmentPath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    MORP_CORE_ASSERT(MORP_ERROR, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::Vulkan:  return VulkanShader::Make(_Layout, _VertexPath, _FragmentPath);
		}
		
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererAPI!");
		return nullptr;
	}

}

