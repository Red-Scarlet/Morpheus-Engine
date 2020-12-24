#include "Morppch.h"
#include "VulkanVertexArray.h"

#include "VulkanShader.h"

namespace Morpheus {

	VulkanVertexArray::VulkanVertexArray(const Ref<VertexArrayCache>& _Cache)
		: m_Cache(_Cache)
	{
		MORP_PROFILE_FUNCTION();

		m_ID = m_Cache->GetCount();
		VULKAN_CORE_WARN("[VULKAN] VertexArray #" + ToString(GetID()) + " was Created!");
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
		VULKAN_CORE_WARN("[VULKAN] VertexArray Was Destoryed!");
	}

	void VulkanVertexArray::Bind()
	{
		m_Cache->SetBoundID(GetID());

		Ref<VulkanShader::ShaderCache> _ShaderCache = VulkanCache<VulkanShader>::Get(VULKAN_SHADER_CACHE_TYPE);
		_ShaderCache->GetBound();
		if (auto shader = _ShaderCache->GetBound()) {
			shader->UpdateUniformBuffer(std::dynamic_pointer_cast<VulkanUniformBuffer>(m_UniformBuffer), m_ID);
			shader->UpdateTextureBuffer(std::dynamic_pointer_cast<VulkanTextureBuffer>(m_TextureBuffer), m_ID);
			if (m_ShaderCompiled == false) { shader->VulkanCompile(m_ID); m_ShaderCompiled = true; m_Compiled = false; }
		}

		if (m_UniformBuffer != nullptr)
			std::dynamic_pointer_cast<VulkanUniformBuffer>(m_UniformBuffer)->UpdateUniformBuffer();
	}

	void VulkanVertexArray::Unbind()
	{
		m_Cache->SetBoundID(uint32_max);
	}

	void VulkanVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		m_VertexBuffer = _VertexBuffer;
		m_Compiled = false;
	}

	void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		m_IndexBuffer = _IndexBuffer;
		m_Compiled = false;
	}

	void VulkanVertexArray::SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer)
	{
		m_UniformBuffer = _UniformBuffer;
		m_Compiled = false;
	}

	void VulkanVertexArray::SetTextureBuffer(const Ref<TextureBuffer>& _TextureBuffer)
	{
		m_TextureBuffer = _TextureBuffer;
		m_Compiled = false;
	}

	Ref<VulkanVertexArray> VulkanVertexArray::Make()
	{
		Ref<VertexArrayCache> s_Cache = VulkanCache<VulkanVertexArray>::Get(VULKAN_VERTEX_ARRAY_CACHE_TYPE);
		Ref<VulkanVertexArray> s_VulkanVertexArray = CreateRef<VulkanVertexArray>(s_Cache);
		s_Cache->Add(s_VulkanVertexArray->GetID(), s_VulkanVertexArray);
		return s_VulkanVertexArray;
	}

}