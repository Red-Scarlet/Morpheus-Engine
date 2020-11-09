#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanFramebuffer::VulkanFramebuffer(const Ref<Renderpass> _Renderpass, bool Depth)
		: VulkanBindable(VulkanBindableTypes::VulkanFramebuffer), m_CreateDepthImages(Depth)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Surface = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSurface>(VulkanGlobalTypes::VulkanSurface);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		m_Renderpass = std::dynamic_pointer_cast<VulkanRenderpass>(_Renderpass);
		SetID(VulkanMemoryManager::GetInstance()->GetBindableCache()->GetNextBindableID(VulkanBindableTypes::VulkanFramebuffer));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Framebuffer #" + std::to_string(GetID()) + " was Created!");
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] Framebuffer Was Destoryed!");
	}

	void VulkanFramebuffer::Bind()
	{
		MORP_CORE_TRACE("[VULKAN] Framebuffer #" + std::to_string(GetID()) + " was Binded!");
		Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
		SetBound(true);

		uint32 BoundedShader = BindableCache->CheckBound(VulkanBindableTypes::VulkanShader);
		if (BoundedShader == uint32_max); {
			BindableCache->DisableBindings(VulkanBindableTypes::VulkanFramebuffer);
			SetAppending();
			SetBound(true);
			return;
		}

		Ref<VulkanShader> Shader = BindableCache->Get<VulkanShader>(VulkanBindableTypes::VulkanShader, BoundedShader);
		Shader->AppendBinding(GetIdentifier());		
	}

	void VulkanFramebuffer::VulkanCreate()
	{
		CreateFramebuffers();
	}

	void VulkanFramebuffer::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
	}

	void VulkanFramebuffer::CreateFramebuffers()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Format DepthFormat = m_Surface->GetDepthFormat();
		vk::Format ColorFormat = m_Surface->GetColorFormat();		
		vk::Extent2D Extent = m_Swapchain->GetExtent2D();
		uint32 Size = m_Swapchain->GetSize();

		// IMG CREATION
		m_Images.resize(Size);
		Vector<vk::Image> SwapchainImages = Device.getSwapchainImagesKHR(m_Swapchain->GetSwapchain());
		for (uint32 i = 0; i < Size; i++)
			m_Images[i] = VkImage(SwapchainImages[i]);

		// IMG VIEW CREATION
		m_Views.resize(Size);

		for (uint32 i = 0; i < Size; i++) {
			VkImageViewCreateInfo CreateInfo{};
			{
				CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				CreateInfo.image = m_Images[i];
				CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				CreateInfo.format = VkFormat(ColorFormat);
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

	Ref<VulkanFramebuffer> VulkanFramebuffer::Make(const Ref<Renderpass> _Renderpass, bool Depth)
	{
		Ref<VulkanFramebuffer> s_VulkanFramebuffer = CreateRef<VulkanFramebuffer>(_Renderpass, Depth);
		VulkanMemoryManager::GetInstance()->GetBindableCache()->Submit(s_VulkanFramebuffer);
		return s_VulkanFramebuffer;
	}

}