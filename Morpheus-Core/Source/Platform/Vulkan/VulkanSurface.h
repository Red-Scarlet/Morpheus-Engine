// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"

namespace Morpheus {

	class VulkanSurface
	{
	public:
		VulkanSurface(VulkanInstance* Instance);
		~VulkanSurface();

		VkSurfaceKHR GetSurface() { return m_Surface; }

	private:
		void CreateSurface();

	private:
		VulkanInstance* m_VulkanInstance;
		VkSurfaceKHR m_Surface;
	};

}