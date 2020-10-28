#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

namespace Morpheus {

	VulkanFramebuffer::VulkanFramebuffer()
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_Swapchain = VulkanResourceCache::GetInstance()->Get<VulkanSwapchain>(VulkanResourceType::Swapchain);
		m_Renderpass = VulkanResourceCache::GetInstance()->Get<VulkanRenderpass>(VulkanResourceType::Renderpass);

		CreateFramebuffer();
		MORP_CORE_WARN("[VULKAN] Framebuffer Was Created!");	
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		MORP_CORE_WARN("[VULKAN] Framebuffer Was Destoryed!");
	}

	void VulkanFramebuffer::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		auto SwapchainBuffers = m_Swapchain->GetSwapchainBuffers();

		Device.freeMemory(m_DepthImageMemory);
		Device.destroyImage(m_DepthImage);
		if (!SwapchainBuffers.empty())
			Device.destroyImageView(SwapchainBuffers[0].Views[1]);

		for (size_t i = 0; i < SwapchainBuffers.size(); i++) {
			Device.destroyImageView(SwapchainBuffers[i].Views[0]);
			Device.destroyFramebuffer(SwapchainBuffers[i].Framebuffer);
		}
	}

	uint32 VulkanFramebuffer::GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties)
	{
		auto GpuMemoryProps = _PhysicalDevice.getMemoryProperties();
		for (uint32_t i = 0; i < GpuMemoryProps.memoryTypeCount; i++) {
			if ((_TypeBits & 1) == 1)
				if ((GpuMemoryProps.memoryTypes[i].propertyFlags & _Properties) == _Properties)
					return i;
			_TypeBits >>= 1;
		}
		return 0;
	}

	void VulkanFramebuffer::CreateFramebuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

		vk::Extent2D SurfaceSize = m_Swapchain->GetExtent2D();
		vk::SwapchainKHR Swapchain = m_Swapchain->GetSwapchain();

		auto Struct = m_Device->GetSurface()->GetStruct();
		auto SwapchainBuffers = m_Swapchain->GetSwapchainBuffers();
		
		vk::RenderPass Renderpass = m_Renderpass->GetRenderpass();


		// Create Depth Image Data
		m_DepthImage = Device.createImage(
			vk::ImageCreateInfo(
				vk::ImageCreateFlags(),
				vk::ImageType::e2D,
				Struct.DepthFormat,
				vk::Extent3D(SurfaceSize.width, SurfaceSize.height, 1),
				1U,
				1U,
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc,
				vk::SharingMode::eExclusive,
				1,
				&QueueFamilyIndex,
				vk::ImageLayout::eUndefined
			)
		);

		vk::MemoryRequirements depthMemoryReq = Device.getImageMemoryRequirements(m_DepthImage);

		// Search through GPU memory properies to see if this can be device local.

		m_DepthImageMemory = Device.allocateMemory(
			vk::MemoryAllocateInfo(
				depthMemoryReq.size,
				GetMemoryTypeIndex(PhysicalDevice, depthMemoryReq.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
			)
		);

		Device.bindImageMemory(
			m_DepthImage,
			m_DepthImageMemory,
			0
		);

		vk::ImageView depthImageView = Device.createImageView(
			vk::ImageViewCreateInfo(
				vk::ImageViewCreateFlags(),
				m_DepthImage,
				vk::ImageViewType::e2D,
				Struct.DepthFormat,
				vk::ComponentMapping(),
				vk::ImageSubresourceRange(
					vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil,
					0,
					1,
					0,
					1
				)
			)
		);

		Vector<vk::Image> swapchainImages = Device.getSwapchainImagesKHR(Swapchain);
		
		for (uint32 i = 0; i < swapchainImages.size(); i++)
		{
			SwapchainBuffers[i].Image = swapchainImages[i];

			// Color
			SwapchainBuffers[i].Views[0] =
				Device.createImageView(
					vk::ImageViewCreateInfo(
						vk::ImageViewCreateFlags(),
						swapchainImages[i],
						vk::ImageViewType::e2D,
						Struct.ColorFormat,
						vk::ComponentMapping(),
						vk::ImageSubresourceRange(
							vk::ImageAspectFlagBits::eColor,
							0,
							1,
							0,
							1
						)
					)
				);

			// Depth
			SwapchainBuffers[i].Views[1] = depthImageView;

			SwapchainBuffers[i].Framebuffer = Device.createFramebuffer(
				vk::FramebufferCreateInfo(
					vk::FramebufferCreateFlags(),
					Renderpass,
					(uint32)SwapchainBuffers[i].Views.size(),
					SwapchainBuffers[i].Views.data(),
					SurfaceSize.width, SurfaceSize.height,
					1
				)
			);

			m_Swapchain->SetSwapchainBuffers(SwapchainBuffers);
		}

	}

	Ref<VulkanFramebuffer> VulkanFramebuffer::VulkanCreate()
	{
		Ref<VulkanFramebuffer> s_VulkanFramebuffer = CreateRef<VulkanFramebuffer>();
		VulkanResourceCache::GetInstance()->Submit<VulkanFramebuffer>(VulkanResourceType::FrameBuffer, s_VulkanFramebuffer);
		return s_VulkanFramebuffer;
	}

}