#include "Morppch.h"
#include "IndexBuffer.h"

#include "Morpheus/ResourceManager/ResourceCommand.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"

namespace Morpheus {

	IndexBuffer::IndexBuffer(uint32* _Indices, const uint32& _Size)
	{
		m_Resource = ResourceCommand::CreateResource();

		RendererResourceInfo ResourceInfo = {};
		ResourceCommand::AddComponent<RendererResourceInfo>(m_Resource, ResourceInfo);

		RendererAllocationInfo AllocInfo = {};
		AllocInfo.Type = RendererResourceTypes::RENDERER_INDEX_BUFFER;
		AllocInfo.Data = RendererBufferData(_Indices, _Size);
		ResourceCommand::AddComponent<RendererAllocationInfo>(m_Resource, AllocInfo);
	}

	IndexBuffer::~IndexBuffer()
	{
		RendererDeallocationInfo DeallocInfo = {};
		DeallocInfo.Type = RendererResourceTypes::RENDERER_INDEX_BUFFER;
		DeallocInfo.Flags = RendererResourceFlags::RENDERER_REMOVE_RESOURCE;
		ResourceCommand::AddComponent<RendererDeallocationInfo>(m_Resource, DeallocInfo);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32* _Indices, const uint32& _Size)
	{
		return CreateRef<IndexBuffer>(_Indices, _Size);
	}

}