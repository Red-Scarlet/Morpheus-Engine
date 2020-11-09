#include "Morppch.h"
#include "VulkanVertexArray.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanVertexArray::VulkanVertexArray()
		: VulkanBindable(VulkanBindableTypes::VulkanVertexArray)
	{
		SetID(VulkanMemoryManager::GetInstance()->GetBindableCache()->GetNextBindableID(VulkanBindableTypes::VulkanVertexArray));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] VertexArray #" + std::to_string(GetID()) + " was Created!");
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
		MORP_CORE_WARN("[VULKAN] VertexArray Was Destoryed!");
	}


	void VulkanVertexArray::VulkanCreate()
	{
	}

	void VulkanVertexArray::VulkanDestory()
	{
	}

	void VulkanVertexArray::Bind()
	{
		MORP_CORE_TRACE("[VULKAN] VertexArray #" + std::to_string(GetID()) + " was Binded!");
		Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
		SetBound(true);

		uint32 BoundedShader = BindableCache->CheckBound(VulkanBindableTypes::VulkanShader);
		if (BoundedShader == uint32_max) {
			SetAppending();
			return;
		}

		Ref<VulkanShader> Shader = BindableCache->Get<VulkanShader>(VulkanBindableTypes::VulkanShader, BoundedShader);
		Shader->AppendBinding(GetIdentifier());
		Shader->CompileUniform(std::dynamic_pointer_cast<VulkanUniformBuffer>(m_UniformBuffer)->GetID());
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

	const uint32& VulkanVertexArray::GetVertexBufferID() const
	{
		return std::dynamic_pointer_cast<VulkanVertexBuffer>(m_VertexBuffer)->GetID();
	}

	const uint32& VulkanVertexArray::GetIndexBufferID() const
	{
		return std::dynamic_pointer_cast<VulkanIndexBuffer>(m_IndexBuffer)->GetID();
	}

	const uint32& VulkanVertexArray::GetUniformBufferID() const
	{
		return std::dynamic_pointer_cast<VulkanUniformBuffer>(m_UniformBuffer)->GetID();
	}

	Ref<VulkanVertexArray> VulkanVertexArray::Make()
	{
		Ref<VulkanVertexArray> s_VulkanVertexArray = CreateRef<VulkanVertexArray>();
		VulkanMemoryManager::GetInstance()->GetBindableCache()->Submit(s_VulkanVertexArray);
		return s_VulkanVertexArray;
	}

}

