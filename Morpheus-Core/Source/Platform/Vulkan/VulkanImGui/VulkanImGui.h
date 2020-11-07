#pragma once

#include "Morpheus/Core/Common.h"

#include <vulkan/vulkan.hpp>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_vulkan.h>

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanRenderQueue.h"

#include "Platform/Vulkan/VulkanUnknown/VulkanRenderpass.h"
#include "Platform/Vulkan/VulkanUnknown/VulkanFramebuffer.h"

namespace Morpheus {

	class VulkanImGui
	{
	public:
		VulkanImGui();

		void Init();
		void InitCommands();
		void SetupCommands();

	private:
		void CreateDescriptorPool();
		void CreateCommandBuffer();

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		Ref<VulkanRenderpass> m_Renderpass;
		Ref<VulkanFramebuffer> m_Framebuffer;

		Ref<VulkanRenderQueue> m_RenderQueue;

		VkDescriptorPool m_DescriptorPool;
		VkCommandPool m_CommandPool;
		Vector<VkCommandBuffer> m_CommandBuffers;
	};

}