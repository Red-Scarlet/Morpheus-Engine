#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandStack.h"

namespace Morpheus { namespace Vulkan {

	enum class VulkanRendererType
	{
		VULKAN_VERTEX_ARRAY,
		VULKAN_RENDER_GRAPH
	};

	struct VulkanRendererInfo
	{
	public:
		VulkanRendererType Type;
	};

}}