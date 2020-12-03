#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanCommandAllocator.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanQueue.h"

#include "Platform/Vulkan/VulkanCore/VulkanValidation.h"

namespace Morpheus {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();
		virtual void Init() override;
		virtual void Destory() override;
		virtual void End() override;

	private:
		Ref<VulkanInstance> m_Instance;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanCommandAllocator> m_CommandSystem;
		Ref<VulkanQueue> m_Queue;

	};

}