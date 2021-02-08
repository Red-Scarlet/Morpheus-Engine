#include "Morppch.h"
#include "VertexArray.h"

#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/Renderer/RendererComponents/RendererVertexArrayInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"

namespace Morpheus {

	VertexArray::VertexArray()
	{
		m_Resource = ResourceCommand::CreateResource();

		RendererResourceInfo ResourceInfo = {};
		ResourceCommand::AddComponent<RendererResourceInfo>(m_Resource, ResourceInfo);

		RendererVertexArrayInfo VertexArrayInfo = {};
		ResourceCommand::AddComponent<RendererVertexArrayInfo>(m_Resource, VertexArrayInfo);
	}

	VertexArray::~VertexArray()
	{
	}

	void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& _VertexBuffer)
	{
		RendererVertexArrayInfo& VertexArrayInfo = ResourceCommand::GetComponent<RendererVertexArrayInfo>(m_Resource);
		VertexArrayInfo.VertexBuffer = *_VertexBuffer;

		RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(m_Resource);
		ResourceInfo.Changed |= 0x01;
	}

	void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _IndexBuffer)
	{
		RendererVertexArrayInfo& VertexArrayInfo = ResourceCommand::GetComponent<RendererVertexArrayInfo>(m_Resource);
		VertexArrayInfo.IndexBuffer = *_IndexBuffer;

		RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(m_Resource);
		ResourceInfo.Changed |= 0x02;
	}

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<VertexArray>();
	}


}