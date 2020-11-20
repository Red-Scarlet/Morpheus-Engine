#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"

namespace Morpheus {

	class VulkanSwapchain
	{
	public:
		VulkanSwapchain();
		virtual ~VulkanSwapchain();

	private:
		void VulkanCreate();
		void VulkanDestory();

	public:
		const vk::SwapchainKHR& GetSwapchain() { return m_Swapchain; }
		const vk::Extent2D& GetExtent2D() { return m_SurfaceSize; }
		const vk::Rect2D& GetRenderArea() { return m_RenderArea; }
		const vk::Viewport& GetViewport() { return m_Viewport; }
		const uint32& GetSize() { return m_BackBufferCount; }

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;

		vk::SwapchainKHR m_Swapchain;
		vk::Extent2D m_SurfaceSize;
		vk::Rect2D m_RenderArea;
		vk::Viewport m_Viewport;

		uint32 m_Width = 1280;
		uint32 m_Height = 720;
		uint32 m_BackBufferCount = 0;

	public:
		static Ref<VulkanSwapchain> Make();

	};

}