#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"

#include "Morpheus/Renderer/RendererResources/TextureBuffer.h"

namespace Morpheus {

	class VulkanTextureBuffer : public TextureBuffer
	{
	public:
		VulkanTextureBuffer();
		virtual ~VulkanTextureBuffer();
		void Destory();

		virtual const uint32& GetID() override { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void CreateTextureBuffer();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		uint32 m_ID = 0;

	public:
		static Ref<VulkanTextureBuffer> VulkanCreate();
	};

}