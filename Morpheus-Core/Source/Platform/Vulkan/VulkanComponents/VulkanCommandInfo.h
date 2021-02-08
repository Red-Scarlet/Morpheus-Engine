#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandStack.h"

namespace Morpheus { namespace Vulkan {

	struct VulkanCommandInfo
	{
	public:
		Ref<VulkanCommandBuffer> Buffer;
		Ref<VulkanCommandStack> Stack;
		bool Processed;
	};

}}