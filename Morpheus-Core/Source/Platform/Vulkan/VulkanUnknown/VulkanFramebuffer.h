#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"

#include "Morpheus/Renderer/RendererUnknown/FrameBuffer.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "VulkanRenderpass.h"

namespace Morpheus {

	struct VulkanSwapchainBuffer
	{
	public:
		vk::Image Image;
		std::array<vk::ImageView, 2> Views;
		vk::Framebuffer Framebuffer;
	};

	class VulkanFramebuffer : public FrameBuffer
	{
	public:
		VulkanFramebuffer(const Ref<Renderpass> _Renderpass, bool Depth);
		virtual ~VulkanFramebuffer();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const Vector<VulkanSwapchainBuffer>& GetSwapchainBuffers() { return m_SwapchainBuffers; }
		const Vector<VkFramebuffer>& GetFramebuffers() { return m_Framebuffers; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void CreateSwapchainBuffers();
		void CreateFramebuffer();
		void CreateFramebufferN();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanRenderpass> m_Renderpass;

		vk::Image m_DepthImage;
		vk::DeviceMemory m_DepthImageMemory;
		uint32 m_ID = 0;

		Vector<VulkanSwapchainBuffer> m_SwapchainBuffers;


		Vector<VkFramebuffer> m_Framebuffers;
		Vector<VkImageView> m_Views;
		Vector<VkImage> m_Images;

		bool m_CreateDepthImages = true;

	public:
		static Ref<VulkanFramebuffer> VulkanCreate(const Ref<Renderpass> _Renderpass, bool Depth = true);
	};

}