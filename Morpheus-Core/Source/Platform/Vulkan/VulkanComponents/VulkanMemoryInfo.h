#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/ResourceManager/ResourceTypes.h"

namespace Morpheus { namespace Vulkan {

	class IVulkanResource
	{
	public:
		virtual ~IVulkanResource() = default;
	};

	struct VulkanMemoryInfo
	{
	public:
		Ref<IVulkanResource> Instance;
		bool Created = false;
	};

}}