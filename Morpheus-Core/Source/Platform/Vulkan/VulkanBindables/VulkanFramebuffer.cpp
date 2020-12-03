#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

#include "VulkanBindingChain.h"

namespace Morpheus {

	VulkanFrameBuffer::VulkanFrameBuffer(const Ref<Renderpass>& _Renderpass)
		: m_Renderpass(std::dynamic_pointer_cast<VulkanRenderpass>(_Renderpass))
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_Surface = VulkanMemoryManager::GetInstance()->GetSurface();
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();
		m_ID = VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Count();

		CreateFrameBuffers();
		VULKAN_CORE_WARN("[VULKAN] Framebuffer #" + std::to_string(GetID()) + " was Created!");
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		for (auto FrameBuffer : m_Framebuffers)
			vkDestroyFramebuffer(Device, FrameBuffer, nullptr);

		VULKAN_CORE_WARN("[VULKAN] Framebuffer Was Destoryed!");
	}

	void VulkanFrameBuffer::Bind(const uint32& _Slot)
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();

		//uint32 ShaderID = Chain.GetShaderID();
		//if (ShaderID != uint32_max)
		//	VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID)->SetupPipeline(m_ID);

		Chain.SetFrameBuffer(m_ID);
	}

	void VulkanFrameBuffer::Unbind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetFrameBuffer(uint32_max);
	}

	void VulkanFrameBuffer::CreateFrameBuffers()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		
		VkFormat DepthFormat = VkFormat(m_Surface->GetDepthFormat());
		VkFormat ColorFormat = VkFormat(m_Surface->GetColorFormat());
		VkExtent2D Extent = VkExtent2D(m_Swapchain->GetExtent2D());
		uint32 Size = m_Swapchain->GetSize();

		m_Images.resize(Size);
		Vector<vk::Image> SwapchainImages = Device.getSwapchainImagesKHR(m_Swapchain->GetSwapchain());
		for (uint32 i = 0; i < Size; i++)
			m_Images[i] = VkImage(SwapchainImages[i]);

		m_Views.resize(Size);

		for (uint32 i = 0; i < Size; i++) {
			VkImageViewCreateInfo CreateInfo = {};
			CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			CreateInfo.image = m_Images[i];
			CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			CreateInfo.format = ColorFormat;
			CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			CreateInfo.subresourceRange.baseMipLevel = 0;
			CreateInfo.subresourceRange.levelCount = 1;
			CreateInfo.subresourceRange.baseArrayLayer = 0;
			CreateInfo.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(Device, &CreateInfo, nullptr, &m_Views[i]);
			MORP_CORE_ASSERT(result, "Failed to create Imageview!");
		}

		VkFramebufferCreateInfo FramebufferInfo = {};
		FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferInfo.renderPass = m_Renderpass->GetRenderpass();
		FramebufferInfo.attachmentCount = 1;
		FramebufferInfo.width = Extent.width;
		FramebufferInfo.height = Extent.height;
		FramebufferInfo.layers = 1;

		m_Framebuffers.resize(Size);
		for (uint32 i = 0; i < Size; i++) {
			VkImageView Attachments[] = { m_Views[i] };
			FramebufferInfo.pAttachments = Attachments;
			VkResult result = vkCreateFramebuffer(Device, &FramebufferInfo, nullptr, &m_Framebuffers[i]);
			MORP_CORE_ASSERT(result, "Failed to create Framebuffer!");
		}
	}

	Ref<VulkanFrameBuffer> VulkanFrameBuffer::Make(const Ref<Renderpass>& _Renderpass)
	{
		Ref<VulkanFrameBuffer> s_VulkanFramebuffer = CreateRef<VulkanFrameBuffer>(_Renderpass);
		VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Add(s_VulkanFramebuffer->GetID(), s_VulkanFramebuffer);
		return s_VulkanFramebuffer;
	}

}