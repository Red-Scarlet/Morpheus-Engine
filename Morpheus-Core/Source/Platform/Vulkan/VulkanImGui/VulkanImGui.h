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

#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanQueue.h"

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

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		Ref<VulkanFrameBuffer> m_Framebuffer;
		Ref<VulkanCommandSystem> m_CommandSystem;
		Ref<VulkanQueue> m_Queue;

		VkDescriptorPool m_DescriptorPool;
		Vector<VkCommandBuffer> m_CommandBuffers;
	};

}