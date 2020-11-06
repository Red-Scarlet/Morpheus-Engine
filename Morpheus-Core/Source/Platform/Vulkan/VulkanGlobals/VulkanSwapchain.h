#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

namespace Morpheus {

	class VulkanSwapchain 
	{
	public:
		VulkanSwapchain();
		~VulkanSwapchain();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::SwapchainKHR& GetSwapchain() { return m_Swapchain; }
		const uint32& GetSize() { return m_BackBufferCount; }
		const vk::Viewport& GetViewport() { return m_Viewport; }
		const vk::Extent2D& GetExtent2D() { return m_SurfaceSize; }
		const vk::Rect2D& GetRenderArea() { return m_RenderArea; }

		//const Vector<VulkanSwapchainBuffer>& GetSwapchainBuffers() { return m_SwapchainBuffers; }
		//void SetSwapchainBuffers(const Vector<VulkanSwapchainBuffer>& _Buffers) { m_SwapchainBuffers = _Buffers; }

	private:
		void CreateSwapchain();

	private:
		Ref<VulkanDevice> m_Device;

		uint32 m_BackBufferCount = 0;

		//Vector<VulkanSwapchainBuffer> m_SwapchainBuffers;
		vk::SwapchainKHR m_Swapchain;
		vk::Rect2D m_RenderArea;
		vk::Extent2D m_SurfaceSize;	//MAYBE ADD TO SURFACE LATER
		vk::Viewport m_Viewport;

		uint32 m_Width = 1280;
		uint32 m_Height = 720;


		uint32 m_ID = 0;

	public:
		static Ref<VulkanSwapchain> Create();
	};

}