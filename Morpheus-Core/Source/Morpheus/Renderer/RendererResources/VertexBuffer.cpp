#include "Morppch.h"
#include "VertexBuffer.h"

#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"

namespace Morpheus {

	VertexBuffer::VertexBuffer(float* _Vertices, const uint32& _Size)
	{
		m_Resource = ResourceCommand::CreateResource();

		RendererResourceInfo ResourceInfo = {};
		ResourceCommand::AddComponent<RendererResourceInfo>(m_Resource, ResourceInfo);

		RendererAllocationInfo AllocInfo = {};
		AllocInfo.Type = RendererResourceTypes::RENDERER_VERTEX_BUFFER;
		AllocInfo.Data = RendererBufferData(_Vertices, _Size);
		ResourceCommand::AddComponent<RendererAllocationInfo>(m_Resource, AllocInfo);
	}

	VertexBuffer::~VertexBuffer()
	{
		RendererDeallocationInfo DeallocInfo = {};
		DeallocInfo.Type = RendererResourceTypes::RENDERER_VERTEX_BUFFER;
		DeallocInfo.Flags = RendererResourceFlags::RENDERER_REMOVE_RESOURCE;
		ResourceCommand::AddComponent<RendererDeallocationInfo>(m_Resource, DeallocInfo);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* _Vertices, const uint32& _Size)
	{
		return CreateRef<VertexBuffer>(_Vertices, _Size);
	}

}