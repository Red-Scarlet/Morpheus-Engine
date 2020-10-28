#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"

#include "Morpheus/Renderer/RendererResources/FrameBuffer.h"

#include "Platform/VULKAN/VulkanDevice.h"
#include "Platform/VULKAN/VulkanSurface.h"
#include "Platform/VULKAN/VulkanSwapchain.h"

#include "VulkanRenderpass.h"

namespace Morpheus {

	class VulkanFramebuffer : public FrameBuffer
	{
	public:
		VulkanFramebuffer();
		virtual ~VulkanFramebuffer();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void CreateFramebuffer();

	private:
		vk::Image m_DepthImage;
		vk::DeviceMemory m_DepthImageMemory;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanRenderpass> m_Renderpass;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanFramebuffer> VulkanCreate();
	};

}