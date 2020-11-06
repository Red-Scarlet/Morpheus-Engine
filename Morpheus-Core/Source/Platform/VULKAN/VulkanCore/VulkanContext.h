#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanRenderQueue.h"

namespace Morpheus {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();
		virtual void Init() override;
		virtual void Destory() override;

		virtual void Render() override;

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanRenderQueue> m_RenderQueue;

		Ref<VulkanSynchronization> m_Sync;

	};

}