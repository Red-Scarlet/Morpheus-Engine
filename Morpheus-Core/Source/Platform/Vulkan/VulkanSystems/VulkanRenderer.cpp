#include "Morppch.h"
#include "VulkanRenderer.h"

namespace Morpheus { namespace Vulkan {

	VulkanRenderer::VulkanRenderer()
	{
	}

	void VulkanRenderer::Init()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanRenderer::Update()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanRenderer::Stop()
	{
		MORP_PROFILE_FUNCTION();
	}

	bool VulkanRenderer::Process(const Resource& _Resource)
	{
		return false;
	}
	
	Ref<VulkanRenderer> VulkanRenderer::Create()
	{
		return CreateRef<VulkanRenderer>();
	}

}}