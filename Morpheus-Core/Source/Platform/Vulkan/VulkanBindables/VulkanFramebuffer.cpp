#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

#include "VulkanBindingChain.h"

namespace Morpheus {

	VulkanFrameBuffer::VulkanFrameBuffer(const RenderpassLayout& _Layout)
		: m_Layout(_Layout)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_Surface = VulkanMemoryManager::GetInstance()->GetSurface();
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetSwapchain();
		m_ID = VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Count();

		CreateRenderpass();
		CreateFrameBuffers();
		MORP_CORE_WARN("[VULKAN] Framebuffer #" + std::to_string(GetID()) + " was Created!");
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyRenderPass(m_Renderpass);

		for (auto FrameBuffer : m_Framebuffers)
			vkDestroyFramebuffer(Device, FrameBuffer, nullptr);

		MORP_CORE_WARN("[VULKAN] Framebuffer Was Destoryed!");
	}

	void VulkanFrameBuffer::Bind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetFrameBuffer(m_ID);

		uint32 ShaderID = Chain.GetShaderID();
		if (ShaderID != uint32_max)
			VulkanMemoryManager::GetInstance()->GetShaderCache().Get(ShaderID)->SetupPipeline(m_ID);
	}

	void VulkanFrameBuffer::Unbind()
	{
		VulkanBindingChain& Chain = VulkanMemoryManager::GetInstance()->GetBindingChain();
		Chain.SetFrameBuffer(uint32_max);
	}

	void VulkanFrameBuffer::InvalidateAttachments()
	{
		auto layout = m_Layout.GetElements();

		vk::Format ColorFormat = m_Surface->GetColorFormat();
		vk::Format DepthFormat = m_Surface->GetDepthFormat();

		for (uint32 i = 0; i < layout.size(); i++)
		{
			// Attachment & RenderpassInfo
			vk::AttachmentLoadOp LoadType;
			vk::AttachmentStoreOp StoreType;

			switch (layout[i].LoadAttachment)
			{
			case RenderpassAttachment::ATTACHMENT_CLEAR:
				LoadType = vk::AttachmentLoadOp::eClear;
				break;

			case RenderpassAttachment::ATTACHMENT_DONTCARE:
				LoadType = vk::AttachmentLoadOp::eDontCare;
				break;

			case RenderpassAttachment::ATTACHMENT_LOAD:
				LoadType = vk::AttachmentLoadOp::eLoad;
				break;

			case RenderpassAttachment::ATTACHMENT_STORE:
				LoadType = vk::AttachmentLoadOp::eLoad;
				break;

			}

			switch (layout[i].StoreAttachment)
			{
			case RenderpassAttachment::ATTACHMENT_CLEAR:
				StoreType = vk::AttachmentStoreOp::eDontCare;
				break;

			case RenderpassAttachment::ATTACHMENT_DONTCARE:
				StoreType = vk::AttachmentStoreOp::eDontCare;
				break;

			case RenderpassAttachment::ATTACHMENT_LOAD:
				StoreType = vk::AttachmentStoreOp::eStore;
				break;

			case RenderpassAttachment::ATTACHMENT_STORE:
				StoreType = vk::AttachmentStoreOp::eStore;
				break;

			}

			switch (layout[i].Type)
			{
			case RenderpassTypes::ATTACHMENT_COLOR: {
				VkAttachmentDescription ColorAttachment{};
				{
					ColorAttachment.format = VkFormat(ColorFormat);
					ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
					ColorAttachment.loadOp = VkAttachmentLoadOp(LoadType);
					ColorAttachment.storeOp = VkAttachmentStoreOp(StoreType);
					ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

					uint32 Index = GetID();
					if (Index == 0)
						ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					else ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

					ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				}
				m_Attachments.push_back(ColorAttachment);

				VkAttachmentReference ColorAttachmentRef{};
				{
					ColorAttachmentRef.attachment = 0;
					ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				}

				m_ColorReference.push_back(ColorAttachmentRef);


				VkSubpassDependency Dependency = {};
				Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				Dependency.dstSubpass = 0;
				Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				Dependency.srcAccessMask = 0;
				Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				m_Dependencies.push_back(Dependency);

			} break;

			case RenderpassTypes::ATTACHMENT_DEPTH: {
				VkAttachmentDescription DepthAttachment{};
				{
					DepthAttachment.format = VkFormat(DepthFormat);
					DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
					DepthAttachment.loadOp = VkAttachmentLoadOp(LoadType);
					DepthAttachment.storeOp = VkAttachmentStoreOp(StoreType);
					DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				}
				m_Attachments.push_back(DepthAttachment);

				VkAttachmentReference DepthAttachmentRef{};
				{
					DepthAttachmentRef.attachment = 1;
					DepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				}

				m_DepthReference.push_back(DepthAttachmentRef);
				m_Dependencies.push_back(vk::SubpassDependency(
					0,
					VK_SUBPASS_EXTERNAL,
					vk::PipelineStageFlagBits::eColorAttachmentOutput,
					vk::PipelineStageFlagBits::eBottomOfPipe,
					vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
					vk::AccessFlagBits::eMemoryRead,
					vk::DependencyFlagBits::eByRegion
				));

			} break;
			}
		}
	}

	void VulkanFrameBuffer::CreateRenderpass()
	{
		// GET LAYOUT
		vk::Device Device = m_Device->GetLogicalDevice();
		InvalidateAttachments();

		m_SubpassDesc.push_back(vk::SubpassDescription(
			vk::SubpassDescriptionFlags(),
			vk::PipelineBindPoint::eGraphics,
			0,
			nullptr,
			(uint32)m_ColorReference.size(),
			m_ColorReference.data(),
			nullptr,
			m_DepthReference.data(),
			0,
			nullptr
		));

		m_Renderpass = Device.createRenderPass(
			vk::RenderPassCreateInfo(
				vk::RenderPassCreateFlags(),
				(uint32)m_Attachments.size(),
				m_Attachments.data(),
				(uint32)m_SubpassDesc.size(),
				m_SubpassDesc.data(),
				(uint32)m_Dependencies.size(),
				m_Dependencies.data()
			)
		);
	}

	void VulkanFrameBuffer::CreateFrameBuffers()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Format DepthFormat = m_Surface->GetDepthFormat();
		vk::Format ColorFormat = m_Surface->GetColorFormat();
		vk::Extent2D Extent = m_Swapchain->GetExtent2D();
		uint32 Size = m_Swapchain->GetSize();

		m_Images.resize(Size);
		Vector<vk::Image> SwapchainImages = Device.getSwapchainImagesKHR(m_Swapchain->GetSwapchain());
		for (uint32 i = 0; i < Size; i++)
			m_Images[i] = VkImage(SwapchainImages[i]);

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

		vk::RenderPass Renderpass = m_Renderpass;
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
			FramebufferInfo.pAttachments = Attachments;
			VkResult result = vkCreateFramebuffer(Device, &FramebufferInfo, nullptr, &m_Framebuffers[i]);
			MORP_CORE_ASSERT(result, "Failed to create Framebuffer!");
		}

		// Might need to cast into Renderpass Again?
	}

	Ref<VulkanFrameBuffer> VulkanFrameBuffer::Make(const RenderpassLayout& _Layout)
	{
		Ref<VulkanFrameBuffer> s_VulkanFramebuffer = CreateRef<VulkanFrameBuffer>(_Layout);
		VulkanMemoryManager::GetInstance()->GetFrameBufferCache().Add(s_VulkanFramebuffer->GetID(), s_VulkanFramebuffer);
		return s_VulkanFramebuffer;
	}

}