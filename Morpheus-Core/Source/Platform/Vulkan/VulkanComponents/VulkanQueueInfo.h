#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandStack.h"

namespace Morpheus { namespace Vulkan {

	struct VulkanQueueInfo
	{
	public:
		bool Ready = false;
	};

}}