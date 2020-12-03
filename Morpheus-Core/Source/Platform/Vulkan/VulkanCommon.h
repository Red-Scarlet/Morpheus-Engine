#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "Morpheus/Mathematics/Mathematics.h"

//#define VULKAN_CORE_LOGGING

#ifdef VULKAN_CORE_LOGGING
#define VULKAN_CORE_TRACE(...)			MORP_CORE_TRACE(__VA_ARGS__)
#define VULKAN_CORE_INFO(...)			MORP_CORE_INFO(__VA_ARGS__)
#define VULKAN_CORE_WARN(...)			MORP_CORE_WARN(__VA_ARGS__)
#define VULKAN_CORE_ERROR(...)			MORP_CORE_ERROR(__VA_ARGS__)
#define VULKAN_CORE_SPECIAL(...)		MORP_CORE_SPECIAL(__VA_ARGS__)
#define VULKAN_CORE_SPECIAL_2(...)		MORP_CORE_SPECIAL_2(__VA_ARGS__)
#else
#define VULKAN_CORE_TRACE(...)
#define VULKAN_CORE_INFO(...)
#define VULKAN_CORE_WARN(...)
#define VULKAN_CORE_ERROR(...)
#define VULKAN_CORE_SPECIAL(...)
#define VULKAN_CORE_SPECIAL_2(...)
#endif

namespace Morpheus {

	struct VulkanBuffer
	{
	public:
		vk::Buffer Buffer;
		vk::DeviceMemory Memory;
	};

	struct VulkanImage
	{
	public:
		VkImage Image;
		VkDeviceMemory Memory;
	};

}

