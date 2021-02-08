#include "Morppch.h"
#include "VulkanAllocator.h"

#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderGraph.h"

namespace Morpheus { namespace Vulkan {

	static Ref<IVulkanResource> AllocateResource(const RendererResourceTypes& _Type, const AnyData& _Data, const Resource& _Resource)
	{
		switch (_Type)
		{
			case RendererResourceTypes::RENDERER_VERTEX_BUFFER:		return VulkanVertexBuffer::Create(_Data);
			case RendererResourceTypes::RENDERER_INDEX_BUFFER:		return VulkanIndexBuffer::Create(_Data);
			case RendererResourceTypes::RENDERER_RENDER_GRAPH:		return VulkanRenderGraph::Create(_Data, _Resource);
		}

		VULKAN_CORE_ASSERT(VULKAN_ERROR, "[VULKAN] Cannot allocate unknown resource type!");
		return nullptr;
	}

	VulkanAllocator::VulkanAllocator()
	{
	}

	void VulkanAllocator::Init()
	{
		MORP_PROFILE_FUNCTION();

		ResourceCommand::RegisterComponent<VulkanMemoryInfo>();
	}

	void VulkanAllocator::Update()
	{
		MORP_PROFILE_FUNCTION();

		Vector<Resource> Allocated;
		for (auto const& resource : m_Resources)
			if (Process(resource))
				Allocated.emplace_back(resource);
		
		for (auto const& resource : Allocated)
			ResourceCommand::RemoveComponent<RendererAllocationInfo>(resource);
		Allocated.clear();
	}

	void VulkanAllocator::Stop()
	{
		MORP_PROFILE_FUNCTION();
	}
	
	bool VulkanAllocator::Process(const Resource& _Resource)
	{
		MORP_PROFILE_FUNCTION();

		if (ResourceCommand::ContainsComponent<VulkanMemoryInfo>(_Resource) == false) {
			auto const& AllocInfo = ResourceCommand::GetComponent<RendererAllocationInfo>(_Resource);
			if (AllocInfo.Next != uint32_max) Process(AllocInfo.Next);
			VulkanMemoryInfo Memory = { AllocateResource(AllocInfo.Type, AllocInfo.Data, _Resource), true };
			ResourceCommand::AddComponent<VulkanMemoryInfo>(_Resource, Memory);
			VULKAN_CORE_INFO("[RESOURCE] Resource was Allocated!");
			return true;
		}

		return false;
	}

	Ref<VulkanAllocator> VulkanAllocator::Create()
	{
		return CreateRef<VulkanAllocator>();
	}

}}