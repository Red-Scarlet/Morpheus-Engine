#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

#include "Morpheus/Renderer/RendererPipeline/RenderBuffer.h"

namespace Morpheus { namespace Vulkan {

	class VulkanRenderBuffer
	{
	public:
		VulkanRenderBuffer(const RenderBufferElement& _RenderElement);
		virtual ~VulkanRenderBuffer();

		const VkImage& GetImage() { return m_Image; }
		const VkImageView& GetImageView() { return m_ImageView; }
		const RenderBufferElement& GetInfo() { return m_Info; }
	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<VulkanDevice> m_Device;
		RenderBufferElement m_Info;

		VkImage m_Image;
		VkImageView m_ImageView;
		VkDeviceMemory m_Memory;
		
	public:
		static Ref<VulkanRenderBuffer> Create(const RenderBufferElement& _RenderElement);
		static void Destroy(const Ref<VulkanRenderBuffer>& _RenderBuffer);
	};

}}