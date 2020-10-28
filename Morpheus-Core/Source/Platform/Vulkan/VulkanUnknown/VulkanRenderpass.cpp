#include "Morppch.h"
#include "VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanResource.h"

namespace Morpheus {

	VulkanRenderpass::VulkanRenderpass()
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);

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

	void VulkanRenderpass::CreateRenderpass()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		auto Struct = m_Device->GetSurface()->GetStruct();

		Vector<vk::AttachmentDescription> attachmentDescriptions =
		{
			vk::AttachmentDescription(
				vk::AttachmentDescriptionFlags(),
				Struct.ColorFormat,
				vk::SampleCountFlagBits::e1,
				vk::AttachmentLoadOp::eClear,
				vk::AttachmentStoreOp::eStore,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::ePresentSrcKHR
			),

			vk::AttachmentDescription(
				vk::AttachmentDescriptionFlags(),
				Struct.DepthFormat,
				vk::SampleCountFlagBits::e1,
				vk::AttachmentLoadOp::eClear,
				vk::AttachmentStoreOp::eDontCare,
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentStoreOp::eDontCare,
				vk::ImageLayout::eUndefined,
				vk::ImageLayout::eDepthStencilAttachmentOptimal
			)
		};

		Vector<vk::AttachmentReference> colorReferences =
		{
			vk::AttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal)
		};

		Vector<vk::AttachmentReference> depthReferences = {
			vk::AttachmentReference(1, vk::ImageLayout::eDepthStencilAttachmentOptimal)
		};

		Vector<vk::SubpassDescription> subpasses =
		{
			vk::SubpassDescription(
				vk::SubpassDescriptionFlags(),
				vk::PipelineBindPoint::eGraphics,
				0,
				nullptr,
				(uint32)colorReferences.size(),
				colorReferences.data(),
				nullptr,
				depthReferences.data(),
				0,
				nullptr
			)
		};

		Vector<vk::SubpassDependency> dependencies =
		{
			vk::SubpassDependency(
				~0U,
				0,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::PipelineStageFlagBits::eColorAttachmentOutput,
				vk::AccessFlagBits::eMemoryRead,
				vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
				vk::DependencyFlagBits::eByRegion
			),
			vk::SubpassDependency(
				0,
				~0U,
				vk::PipelineStageFlagBits::eColorAttachmentOutput,
				vk::PipelineStageFlagBits::eBottomOfPipe,
				vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
				vk::AccessFlagBits::eMemoryRead,
				vk::DependencyFlagBits::eByRegion
			)
		};

		m_RenderPass = Device.createRenderPass(
			vk::RenderPassCreateInfo(
				vk::RenderPassCreateFlags(),
				(uint32)attachmentDescriptions.size(),
				attachmentDescriptions.data(),
				(uint32)subpasses.size(),
				subpasses.data(),
				(uint32)dependencies.size(),
				dependencies.data()
			)
		);
	}

	Ref<VulkanRenderpass> VulkanRenderpass::VulkanCreate()
	{
		Ref<VulkanRenderpass> s_VulkanRenderpass = CreateRef<VulkanRenderpass>();
		VulkanResourceCache::GetInstance()->Submit<VulkanRenderpass>(VulkanResourceType::Renderpass, s_VulkanRenderpass);
		return s_VulkanRenderpass;
	}

}