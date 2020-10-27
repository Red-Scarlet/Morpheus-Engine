#include "Morppch.h"
#include "VulkanVertexArray.h"

#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

namespace Morpheus {

	VulkanVertexArray::VulkanVertexArray()
	{
		//m_ID = VulkanResourceCache::GetInstance()->GetNextBindingID(VulkanBindables::VertexArray);
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
	}

	void VulkanVertexArray::Bind()
	{
		//if (!VulkanResourceCache::GetInstance()->CheckBinding(VulkanBindables::VertexArray, m_ID)) {
		//	BindingPairs VBO = MakeBindingPairs(VulkanBindables::VertexArray, m_ID, VulkanResourceType::VertexBuffer, m_VertexBuffer->GetID());
		//	BindingPairs IBO = MakeBindingPairs(VulkanBindables::VertexArray, m_ID, VulkanResourceType::IndexBuffer, m_IndexBuffer->GetID());
		//	BindingPairs UBO = MakeBindingPairs(VulkanBindables::VertexArray, m_ID, VulkanResourceType::UniformBuffer, m_UniformBuffer->GetID());
		//
		//	VulkanResourceCache::GetInstance()->SetBindedPairs(VBO);
		//	VulkanResourceCache::GetInstance()->SetBindedPairs(IBO);
		//	VulkanResourceCache::GetInstance()->SetBindedPairs(UBO);
		//	VulkanResourceCache::GetInstance()->SetBinding(VulkanBindables::VertexArray, m_ID, true);
		//}

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
		return CreateRef<VulkanVertexArray>();
	}

}

