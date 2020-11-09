#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	struct VulkanBuffer
	{
	public:
		vk::DeviceMemory Memory;
		vk::Buffer Buffer;
	};	// Make into struct types .h

}

