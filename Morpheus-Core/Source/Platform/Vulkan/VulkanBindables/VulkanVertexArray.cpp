#include "Morppch.h"
#include "VulkanVertexArray.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanVertexArray::VulkanVertexArray()
	{
		m_ID = VulkanMemoryManager::GetInstance()->GetVertexArrayCache().Count();
		VULKAN_CORE_WARN("[VULKAN] VertexArray #" + std::to_string(GetID()) + " was Created!");
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
		VULKAN_CORE_WARN("[VULKAN] VertexArray Was Destoryed!");
	}

	void VulkanVertexArray::Bind(const uint32& _Slot)
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		// Check to see if there is a shader bound.
		uint32 ShaderID = Chain.GetShaderID();
		if (ShaderID != uint32_max) {
			Ref<VulkanShader> Shader = VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID);
			if(m_UniformBuffer == nullptr)
				m_UniformBuffer = Shader->AllocateUniformBuffer(m_ID);
			Shader->UpdateUniformBuffer(m_UniformBuffer);
			Shader->UpdateTextureBuffer(std::dynamic_pointer_cast<VulkanTextureBuffer>(m_TextureBuffer), m_ID);
			if (m_ShaderCompiled == false) { Shader->VulkanCompile(m_ID); m_ShaderCompiled = true; m_Compiled = false; }
		}
		Chain.SetVertexArray(m_ID);
	}

	void VulkanVertexArray::Unbind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		// Check to see if there is a shader bound.
		uint32 ShaderID = Chain.GetShaderID();
		if (ShaderID != uint32_max) {
			VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID)->DeallocateUniformBuffer(m_UniformBuffer);
		}

		Chain.SetVertexArray(uint32_max);
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

	void VulkanVertexArray::SetTextureBuffer(const Ref<TextureBuffer>& _TextureBuffer)
	{
		m_TextureBuffer = _TextureBuffer;
		m_Compiled = false;
	}

	Ref<VulkanVertexArray> VulkanVertexArray::Make()
	{
		Ref<VulkanVertexArray> s_VulkanVertexArray = CreateRef<VulkanVertexArray>();
		VulkanMemoryManager::GetInstance()->GetVertexArrayCache().Add(s_VulkanVertexArray->GetID(), s_VulkanVertexArray);
		return s_VulkanVertexArray;
	}

}