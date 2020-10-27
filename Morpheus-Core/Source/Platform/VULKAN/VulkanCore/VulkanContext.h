#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"

#include "Platform/VULKAN/VulkanInstance.h"
#include "Platform/VULKAN/VulkanDevice.h"
#include "Platform/VULKAN/VulkanSwapchain.h"
#include "Platform/VULKAN/VulkanCommand.h"

namespace Morpheus {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext();
		virtual void Init() override;
		virtual void Destory() override;

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

	};

}