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

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

	public:
		static Ref<VulkanTextureBuffer> Make();
	};

}