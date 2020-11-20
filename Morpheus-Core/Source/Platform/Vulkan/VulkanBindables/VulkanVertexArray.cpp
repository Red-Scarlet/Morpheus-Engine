#include "Morppch.h"
#include "VulkanVertexArray.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

#include "VulkanBindingChain.h"

namespace Morpheus {

	VulkanVertexArray::VulkanVertexArray()
	{
		m_UniformBufferID = uint32_max;
		m_ID = VulkanMemoryManager::GetInstance()->GetVertexArrayCache().Count();
		MORP_CORE_WARN("[VULKAN] VertexArray #" + std::to_string(GetID()) + " was Created!");
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
		MORP_CORE_WARN("[VULKAN] VertexArray Was Destoryed!");
	}

	void VulkanVertexArray::Bind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetVertexArray(m_ID);

		uint32 ShaderID = Chain.GetShaderID();
		if (ShaderID != uint32_max)
			if(m_UniformBufferID == uint32_max)
				SetUniformBuffer(VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID)->SetupUniform());
	}

	void VulkanVertexArray::Unbind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetVertexArray(uint32_max);
	}

	void VulkanVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		m_VertexBuffer = _VertexBuffer;
	}

	void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		m_IndexBuffer = _IndexBuffer;
	}

	void VulkanVertexArray::SetUniformBuffer(const uint32& _UniformBufferID)
	{
		m_UniformBufferID = _UniformBufferID;
	}

	Ref<VulkanVertexArray> VulkanVertexArray::Make()
	{
		Ref<VulkanVertexArray> s_VulkanVertexArray = CreateRef<VulkanVertexArray>();
		VulkanMemoryManager::GetInstance()->GetVertexArrayCache().Add(s_VulkanVertexArray->GetID(), s_VulkanVertexArray);
		return s_VulkanVertexArray;
	}

}

