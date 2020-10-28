#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

namespace Morpheus {

	struct VulkanSwapchainBuffer
	{
	public:
		vk::Image Image;
		std::array<vk::ImageView, 2> Views;
		vk::Framebuffer Framebuffer;
	};

	class VulkanSwapchain 
	{
	public:
		VulkanSwapchain();
		~VulkanSwapchain();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::SwapchainKHR& GetSwapchain() { return m_Swapchain; }
		const uint32& GetSize() { return m_SwapchainBuffers.size(); }
		const vk::Viewport& GetViewport() { return m_Viewport; }
		const vk::Extent2D& GetExtent2D() { return m_SurfaceSize; }
		const vk::Rect2D& GetRenderArea() { return m_RenderArea; }

		const Vector<VulkanSwapchainBuffer>& GetSwapchainBuffers() { return m_SwapchainBuffers; }
		void SetSwapchainBuffers(const Vector<VulkanSwapchainBuffer>& _Buffers) { m_SwapchainBuffers = _Buffers; }

	private:
		void CreateSwapchain();

	private:
		Vector<VulkanSwapchainBuffer> m_SwapchainBuffers;
		vk::SwapchainKHR m_Swapchain;
		vk::Rect2D m_RenderArea;
		vk::Extent2D m_SurfaceSize;	//MAYBE ADD TO SURFACE LATER
		vk::Viewport m_Viewport;

		uint32 m_Width = 1280;
		uint32 m_Height = 720;

		Ref<VulkanDevice> m_Device;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanSwapchain> Create();
	};

}