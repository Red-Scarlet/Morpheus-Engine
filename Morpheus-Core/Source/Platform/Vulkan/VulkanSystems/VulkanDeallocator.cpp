#include "Morppch.h"
#include "VulkanDeallocator.h"

#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderGraph.h"

namespace Morpheus { namespace Vulkan {

	static void DeallocateResource(const RendererResourceTypes& _Type, const Ref<IVulkanResource>& _IResource)
	{
		switch (_Type)
		{
			case RendererResourceTypes::RENDERER_VERTEX_BUFFER:		VulkanVertexBuffer::Destroy(std::dynamic_pointer_cast<VulkanVertexBuffer>(_IResource));
				return;
			case RendererResourceTypes::RENDERER_INDEX_BUFFER:		VulkanIndexBuffer::Destroy(std::dynamic_pointer_cast<VulkanIndexBuffer>(_IResource));
				return;
			case RendererResourceTypes::RENDERER_RENDER_GRAPH:		VulkanRenderGraph::Destroy(std::dynamic_pointer_cast<VulkanRenderGraph>(_IResource));
				return;
		}

		VULKAN_CORE_ASSERT(VULKAN_ERROR, "[VULKAN] Cannot deallocate unknown resource type!");
	}

	VulkanDeallocator::VulkanDeallocator()
	{
	}

	void VulkanDeallocator::Init()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanDeallocator::Update()
	{
		MORP_PROFILE_FUNCTION();

		Vector<Resource> Deallocated;
		for (auto const& resource : m_Resources)
			if (Process(resource))
				Deallocated.emplace_back(resource);
		
		for (auto const& resource : Deallocated) {
			auto const& DeallocInfo = ResourceCommand::GetComponent<RendererDeallocationInfo>(resource);
			ResourceCommand::RemoveComponent<RendererDeallocationInfo>(resource);
			if (DeallocInfo.Flags & RendererResourceFlags::RENDERER_REMOVE_RESOURCE)
				ResourceCommand::DestroyResource(resource);
		}
		Deallocated.clear();
	}

	void VulkanDeallocator::Stop()
	{
		MORP_PROFILE_FUNCTION();
	}

	bool VulkanDeallocator::Process(const Resource& _Resource)
	{			
		if (ResourceCommand::ContainsComponent<VulkanMemoryInfo>(_Resource) == true)
			if (ResourceCommand::ContainsComponent<RendererAllocationInfo>(_Resource) == false) {
				auto const& DeallocInfo = ResourceCommand::GetComponent<RendererDeallocationInfo>(_Resource);
				auto const& Memory = ResourceCommand::GetComponent<VulkanMemoryInfo>(_Resource);
				DeallocateResource(DeallocInfo.Type, Memory.Instance);
				ResourceCommand::RemoveComponent<VulkanMemoryInfo>(_Resource);
				VULKAN_CORE_INFO("[RESOURCE] Resource was Deallocated!");
				return true;
		}

		return false;
	}
	
	Ref<VulkanDeallocator> VulkanDeallocator::Create()
	{
		return CreateRef<VulkanDeallocator>();
	}

}}