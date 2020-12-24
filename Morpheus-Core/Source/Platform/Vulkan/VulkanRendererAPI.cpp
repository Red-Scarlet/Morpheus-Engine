#include "Morppch.h"
#include "VulkanRendererAPI.h"

namespace Morpheus { namespace Vulkan {

	void VulkanRendererAPI::Init()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanRendererAPI::Shutdown()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanRendererAPI::Flush()
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanRendererAPI::SetClearColor(const Vector4& _ClearColor)
	{
		MORP_PROFILE_FUNCTION();
	}

	void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& _VertexArray)
	{
		MORP_PROFILE_FUNCTION();
	}

	RendererAPI::RendererStats VulkanRendererAPI::GetRendererStats()
	{
		MORP_PROFILE_FUNCTION();

		RendererAPI::RendererStats Data;
		return Data;
	}

}}