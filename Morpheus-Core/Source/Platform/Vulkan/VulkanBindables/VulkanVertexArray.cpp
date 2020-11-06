#include "Morppch.h"
#include "VulkanVertexArray.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"

namespace Morpheus {

	VulkanVertexArray::VulkanVertexArray()
	{
		SetID(VulkanMemoryManager::GetInstance()->GetBindableCache()->GetNextBindableID(VulkanBindableTypes::VulkanVertexArray));
		m_Identifier.Type = VulkanBindableTypes::VulkanVertexArray;
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
		
	}

	void VulkanVertexArray::Bind()
	{
		String str = "[VULKAN] VertexArray #" + std::to_string(m_Identifier.ID) + " was Binded!";
		MORP_CORE_TRACE(str);
		// Message to tell that it is binded!
		Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
		if (!m_Identifier.Bounded) {
			m_Identifier.Bounded = true;
			if (BindableCache->CheckBindingAny()) {
				Ref<VulkanShader> Shader = BindableCache->Get<VulkanShader>(VulkanBindableTypes::VulkanShader, BindableCache->GetPresentID());
				Shader->AddToBindables(m_Identifier);
				Shader->CompileUniform(m_UniformBuffer->GetID());
			}
			else BindableCache->AddAppending(m_Identifier);
		} else {
		}
	}

	void VulkanVertexArray::SetVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		m_VertexBuffer = _VertexBuffer;
	}

	void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		m_IndexBuffer = _IndexBuffer;
	}

	void VulkanVertexArray::SetUniformBuffer(const Ref<UniformBuffer>& _UniformBuffer)
	{
		m_UniformBuffer = _UniformBuffer;

	}

	const Ref<UniformBuffer>& VulkanVertexArray::GetUniformBuffer() const
	{
		return m_UniformBuffer;
	}

	const Ref<VertexBuffer>& VulkanVertexArray::GetVertexBuffer() const
	{
		return m_VertexBuffer;
	}

	const Ref<IndexBuffer>& VulkanVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	Ref<VulkanVertexArray> VulkanVertexArray::VulkanCreate()
	{
		Ref<VulkanVertexArray> s_VulkanVertexArray = CreateRef<VulkanVertexArray>();
		VulkanMemoryManager::GetInstance()->GetBindableCache()->Submit<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, s_VulkanVertexArray);
		return s_VulkanVertexArray;
	}

}

