#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererInstance.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanInstance : public RendererInstance
	{
	public:
		VulkanInstance();
		virtual ~VulkanInstance();

	private: 
		VkInstance m_Instance;
	};

}