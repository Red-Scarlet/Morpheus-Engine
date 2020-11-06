#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanFramebuffer::VulkanFramebuffer(const Ref<Renderpass> _Renderpass, bool Depth)
		: m_CreateDepthImages(Depth)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		m_Renderpass = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanRenderpass>(VulkanResourceTypes::VulkanRenderpass, _Renderpass->GetID());
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanFrameBuffer));

		CreateSwapchainBuffers();
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

		Device.freeMemory(m_DepthImageMemory);
		Device.destroyImage(m_DepthImage);
		if (!m_SwapchainBuffers.empty())
			Device.destroyImageView(m_SwapchainBuffers[0].Views[1]);

		for (size_t i = 0; i < m_SwapchainBuffers.size(); i++) {
			Device.destroyImageView(m_SwapchainBuffers[i].Views[0]);
			Device.destroyFramebuffer(m_SwapchainBuffers[i].Framebuffer);
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

	void VulkanFramebuffer::CreateSwapchainBuffers()
	{
		uint32 Size = m_Swapchain->GetSize();
		m_SwapchainBuffers.resize(Size);
	}

	void VulkanFramebuffer::CreateFramebuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

		vk::Extent2D SurfaceSize = m_Swapchain->GetExtent2D();
		vk::SwapchainKHR Swapchain = m_Swapchain->GetSwapchain();

		auto Struct = m_Device->GetSurface()->GetStruct();		
		vk::RenderPass Renderpass = m_Renderpass->GetRenderpass();

		vk::ImageView depthImageView;

		if(m_CreateDepthImages) {
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

			depthImageView = Device.createImageView(
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
		}

		Vector<vk::Image> swapchainImages = Device.getSwapchainImagesKHR(Swapchain);
		
		for (uint32 i = 0; i < swapchainImages.size(); i++)
		{
			m_SwapchainBuffers[i].Image = swapchainImages[i];

			// Color
			m_SwapchainBuffers[i].Views[0] =
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
			if(m_CreateDepthImages)
				m_SwapchainBuffers[i].Views[1] = depthImageView;

			m_SwapchainBuffers[i].Framebuffer = Device.createFramebuffer(
				vk::FramebufferCreateInfo(
					vk::FramebufferCreateFlags(),
					Renderpass,
					(uint32)m_SwapchainBuffers[i].Views.size(),
					m_SwapchainBuffers[i].Views.data(),
					SurfaceSize.width, SurfaceSize.height,
					1
				)
			);
		}

	}

	void VulkanFramebuffer::CreateFramebufferN()
	{
		uint32 Size = m_Swapchain->GetSize();
		vk::Format Format = m_Device->GetSurface()->GetStruct().ColorFormat;
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Extent2D Extent = m_Swapchain->GetExtent2D();

		// IMG CREATION
		m_Images.resize(Size);
		Vector<vk::Image> SwapchainImages = Device.getSwapchainImagesKHR(m_Swapchain->GetSwapchain());
		for (uint32 i = 0; i < Size; i++)
			m_Images[i] = VkImage(SwapchainImages[i]);

		// IMG VIEW CREATION
		m_Views.resize(Size);

		for (uint32 i = 0; i < Size; i++) {
			VkImageViewCreateInfo CreateInfo {};
			{
				CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				CreateInfo.image = m_Images[i];
				CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				CreateInfo.format = VkFormat(Format);
				CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				
				CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				CreateInfo.subresourceRange.baseMipLevel = 0;
				CreateInfo.subresourceRange.levelCount = 1;
				CreateInfo.subresourceRange.baseArrayLayer = 0;
				CreateInfo.subresourceRange.layerCount = 1;
			}

			VkResult result = vkCreateImageView(Device, &CreateInfo, nullptr, &m_Views[i]);
			MORP_CORE_ASSERT(result, "Failed to create Imageview!");
		}

		// FBO CREATION
		vk::RenderPass Renderpass = m_Renderpass->GetRenderpass();
		VkFramebufferCreateInfo FramebufferInfo{};
		{
			FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferInfo.renderPass = Renderpass;
			FramebufferInfo.attachmentCount = 1;
			FramebufferInfo.width = Extent.width;
			FramebufferInfo.height = Extent.height;
			FramebufferInfo.layers = 1;
		}

		m_Framebuffers.resize(Size);
		for (uint32 i = 0; i < Size; i++) {
			VkImageView Attachments[] = { m_Views[i] };
			FramebufferInfo.pAttachments = Attachments;	// Renderpass?

			VkResult result = vkCreateFramebuffer(Device, &FramebufferInfo, nullptr, &m_Framebuffers[i]);
			MORP_CORE_ASSERT(result, "Failed to create Framebuffer!");
		}
	}

	Ref<VulkanFramebuffer> VulkanFramebuffer::VulkanCreate(const Ref<Renderpass> _Renderpass, bool Depth)
	{
		Ref<VulkanFramebuffer> s_VulkanFramebuffer = CreateRef<VulkanFramebuffer>(_Renderpass, Depth);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanFramebuffer>(VulkanResourceTypes::VulkanFrameBuffer, s_VulkanFramebuffer);
		return s_VulkanFramebuffer;
	}

}