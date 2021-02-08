#include "Morppch.h"
#include "VulkanUpdator.h"
#include "Morpheus/Renderer/RendererComponents/RendererAllocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererDeallocationInfo.h"
#include "Morpheus/Renderer/RendererComponents/RendererResourceInfo.h"
#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderGraph.h"

namespace Morpheus { namespace Vulkan {

	static void UpdateVulkan(const RendererResourceTypes& _Type, const Ref<IVulkanResource>& _IResource)
	{
		switch (_Type)
		{
		case RendererResourceTypes::RENDERER_RENDER_GRAPH:			VulkanRenderGraph::Update(std::dynamic_pointer_cast<VulkanRenderGraph>(_IResource));
			return;
		}

		//VULKAN_CORE_ASSERT(VULKAN_ERROR, "[VULKAN] Cannot update unknown resource type!");
	}

	VulkanUpdator::VulkanUpdator()
	{
	}

	void VulkanUpdator::Init()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanUpdator::Update()
	{
		MORP_PROFILE_FUNCTION();

		for (auto const& resource : m_Resources) {
			auto const& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(resource);
			if (ResourceInfo.Changed != 0x00) Process(resource);
		}

	}

	void VulkanUpdator::Stop()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanUpdator::Process(const Resource& _Resource)
	{
		if (ResourceCommand::ContainsComponent<VulkanMemoryInfo>(_Resource) == true) 
			if (ResourceCommand::ContainsComponent<RendererAllocationInfo>(_Resource) == false) 
				if (ResourceCommand::ContainsComponent<RendererDeallocationInfo>(_Resource) == false) {
					RendererResourceInfo& ResourceInfo = ResourceCommand::GetComponent<RendererResourceInfo>(_Resource);
					VulkanMemoryInfo& Memory = ResourceCommand::GetComponent<VulkanMemoryInfo>(_Resource);
					UpdateVulkan(ResourceInfo.Type, Memory.Instance);
					ResourceInfo.Changed = 0x00;
				}
		
	}
	
	Ref<VulkanUpdator> VulkanUpdator::Create()
	{
		return CreateRef<VulkanUpdator>();
	}

}}