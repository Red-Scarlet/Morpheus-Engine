#include "Morppch.h"
#include "VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanRenderpass::VulkanRenderpass(const RenderpassLayout& _Layout)
		: m_Layout(_Layout)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanRenderpass));

		CreateRenderpass();
		MORP_CORE_WARN("[VULKAN] Renderpass Was Created!");
	}

	VulkanRenderpass::~VulkanRenderpass()
	{
		MORP_CORE_WARN("[VULKAN] Renderpass Was Destoryed!");
	}

	void VulkanRenderpass::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyRenderPass(m_RenderPass);
	}

	void VulkanRenderpass::InvalidateAttachments()
	{
		auto layout = m_Layout.GetElements();
		auto SurfaceFormats = m_Device->GetSurface()->GetStruct();
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
				VkAttachmentDescription ColorAttachment {};
				{
					ColorAttachment.format = VkFormat(SurfaceFormats.ColorFormat);
					ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
					ColorAttachment.loadOp = VkAttachmentLoadOp(LoadType);//VK_ATTACHMENT_LOAD_OP_CLEAR;
					ColorAttachment.storeOp = VkAttachmentStoreOp(StoreType);// VK_ATTACHMENT_STORE_OP_STORE;
					ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					
					if (m_ID == 1) {
						ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

					}
					else {
						ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					}

					ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				}
				m_Attachments.push_back(ColorAttachment);

				VkAttachmentReference ColorAttachmentRef {};
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

				//m_Dependencies.push_back(vk::SubpassDependency(
				//	VK_SUBPASS_EXTERNAL,
				//	0,
				//	vk::PipelineStageFlagBits::eColorAttachmentOutput,
				//	vk::PipelineStageFlagBits::eBottomOfPipe,
				//	vk::AccessFlagBits::eMemoryRead,
				//	vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
				//	vk::DependencyFlagBits::eByRegion
				//));

			} break;
	
			case RenderpassTypes::ATTACHMENT_DEPTH: {
				VkAttachmentDescription DepthAttachment{};
				{
					DepthAttachment.format = VkFormat(SurfaceFormats.DepthFormat);
					DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
					DepthAttachment.loadOp = VkAttachmentLoadOp(LoadType);//VK_ATTACHMENT_LOAD_OP_CLEAR;
					DepthAttachment.storeOp = VkAttachmentStoreOp(StoreType);// VK_ATTACHMENT_STORE_OP_DONT_CARE;
					DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				}
				m_Attachments.push_back(DepthAttachment);

				VkAttachmentReference DepthAttachmentRef {};
				{
					DepthAttachmentRef.attachment = 1;
					DepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				}

				m_DepthReference.push_back(DepthAttachmentRef);

				//m_Dependencies.push_back(vk::SubpassDependency(
				//	0,
				//	VK_SUBPASS_EXTERNAL,
				//	vk::PipelineStageFlagBits::eColorAttachmentOutput,
				//	vk::PipelineStageFlagBits::eBottomOfPipe,
				//	vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
				//	vk::AccessFlagBits::eMemoryRead,
				//	vk::DependencyFlagBits::eByRegion
				//));

			} break;
			}
		}
	}

	void VulkanRenderpass::CreateRenderpass()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		auto Struct = m_Device->GetSurface()->GetStruct();
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

		m_RenderPass = Device.createRenderPass(
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

	Ref<VulkanRenderpass> VulkanRenderpass::VulkanCreate(const RenderpassLayout& _Layout)
	{
		Ref<VulkanRenderpass> s_VulkanRenderpass = CreateRef<VulkanRenderpass>(_Layout);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanRenderpass>(VulkanResourceTypes::VulkanRenderpass, s_VulkanRenderpass);
		return s_VulkanRenderpass;
	}

}