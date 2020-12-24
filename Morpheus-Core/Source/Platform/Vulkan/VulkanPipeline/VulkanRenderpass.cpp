#include "Morppch.h"
#include "VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	VulkanRenderpass::VulkanRenderpass(const RenderpassLayout& _Layout, const Ref<RenderpassCache>& _Cache)
		: m_Layout(_Layout), m_Cache(_Cache)
	{
		MORP_PROFILE_FUNCTION();

		m_Device = VulkanInstance::GetInstance()->GetLogicalDevice();
		m_ID = m_Cache->GetCount();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Renderpass #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanRenderpass::~VulkanRenderpass()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] Renderpass Was Destoryed!");
	}

	void VulkanRenderpass::VulkanCreate()
	{
		Vector<RenderpassElement> Elements = m_Layout.GetElements();

		Vector<VkAttachmentDescription> Attachments;
		Vector<VkSubpassDescription> Descriptions;
		Vector<VkSubpassDependency> Dependencies;

		for (auto& Element : Elements) {
			VkAttachmentDescription Attachment = CreateAttachment(Element);
			VkAttachmentReference Reference = CreateReference(Element);

			if (Element.Next != nullptr)
				VkAttachmentReference NextReference = CreateReference(*Element.Next);

			VkSubpassDescription Description = CreateDescription(Element, Reference);
			VkSubpassDependency Dependency = CreateDependency(Element);

			Attachments.push_back(Attachment);
			Descriptions.push_back(Description);
			Dependencies.push_back(Dependency);
		}

		VkRenderPassCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		CreateInfo.attachmentCount = Attachments.size();
		CreateInfo.pAttachments = Attachments.data();
		CreateInfo.subpassCount = Descriptions.size();
		CreateInfo.pSubpasses = Descriptions.data();
		CreateInfo.dependencyCount = Dependencies.size();
		CreateInfo.pDependencies = Dependencies.data();

		VkResult result = vkCreateRenderPass(m_Device->GetDevice(), &CreateInfo, nullptr, &m_Renderpass);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create Renderpass!");
	}

	void VulkanRenderpass::VulkanDestory()
	{
		vkDestroyRenderPass(m_Device->GetDevice(), m_Renderpass, nullptr);
	}

	VkAttachmentDescription VulkanRenderpass::CreateAttachment(const RenderpassElement& _Element)
	{
		VkFormat Format = {};
		VkAttachmentLoadOp LoadOp = {};
		VkAttachmentStoreOp StoreOp = {};
		VkImageLayout InitialLayout = {};
		VkImageLayout FinalLayout = {};

		VkFormat ColorFormat = VulkanInstance::GetInstance()->GetSurface()->GetSwapchain()->GetFormat();

		switch (_Element.Format)
		{
			case RenderpassFormat::ATTACHMENT_DEFAULT: Format = ColorFormat; break;
			case RenderpassFormat::ATTACHMENT_R8G8B8A8: Format = VK_FORMAT_R8G8B8A8_SNORM; break;
			case RenderpassFormat::ATTACHMENT_R32G32B32A32: Format = VK_FORMAT_R32G32B32A32_UINT; break;
			case RenderpassFormat::ATTACHMENT_D32: Format = VK_FORMAT_D32_SFLOAT; break;
		}

		switch (_Element.Load)
		{
			case RenderpassLoad::ATTACHMENT_CLEAR: LoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; break;
			case RenderpassLoad::ATTACHMENT_LOAD: LoadOp = VK_ATTACHMENT_LOAD_OP_LOAD; break;
			case RenderpassLoad::ATTACHMENT_DONTCARE: LoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; break;
		}

		switch (_Element.Store)
		{
			case RenderpassStore::ATTACHMENT_STORE: StoreOp = VK_ATTACHMENT_STORE_OP_STORE; break;
			case RenderpassStore::ATTACHMENT_DONTCARE: StoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; break;
		}

		switch (_Element.Initial)
		{
			case RenderpassImage::ATTACHMENT_UNDEFINED: InitialLayout = VK_IMAGE_LAYOUT_UNDEFINED; break;
			case RenderpassImage::ATTACHMENT_PRESENT: InitialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; break;
		}

		switch (_Element.Final)
		{
			case RenderpassImage::ATTACHMENT_UNDEFINED: FinalLayout = VK_IMAGE_LAYOUT_UNDEFINED; break;
			case RenderpassImage::ATTACHMENT_PRESENT: FinalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; break;
		}

		VkAttachmentDescription Attachment = {};
		Attachment.format = Format;
		Attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		Attachment.loadOp = LoadOp;
		Attachment.storeOp = StoreOp;
		Attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		Attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		Attachment.finalLayout = FinalLayout;
		Attachment.initialLayout = InitialLayout;

		return Attachment;
	}

	VkAttachmentReference VulkanRenderpass::CreateReference(const RenderpassElement& _Element)
	{
		VkImageLayout Layout = {};

		switch (_Element.Layout)
		{
			case RenderpassAttachmentLayout::ATTACHMENT_COLOR_OPTIMAL: Layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; break;
			case RenderpassAttachmentLayout::ATTACHMENT_DEPTH_STENCIL_OPTIMAL: Layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; break;
			case RenderpassAttachmentLayout::ATTACHMENT_DEPTH_STENCIL_READ: Layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL; break;
			case RenderpassAttachmentLayout::ATTACHMENT_SHADER_READ: Layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; break;
		}

		VkAttachmentReference Reference = {};
		Reference.attachment = _Element.Attachment;
		Reference.layout = Layout;

		return Reference;
	}

	// TODO MAKE MORE DYANMIC
	VkSubpassDescription VulkanRenderpass::CreateDescription(const RenderpassElement& _Element, const VkAttachmentReference& _Reference)
	{
		VkSubpassDescription Description = {};

		Description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		Description.inputAttachmentCount = 0;
		Description.pInputAttachments = nullptr;
		Description.colorAttachmentCount = 1;
		Description.pColorAttachments = &_Reference;
		Description.pDepthStencilAttachment = nullptr;
		Description.pPreserveAttachments = nullptr;
		Description.preserveAttachmentCount = 0;
		Description.pPreserveAttachments = nullptr;

		return Description;
	}

	VkSubpassDependency VulkanRenderpass::CreateDependency(const RenderpassElement& _Element)
	{

		VkSubpassDependency Dependency = {};
		Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		Dependency.dstSubpass = 0;
		Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		Dependency.srcAccessMask = 0;
		Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		//Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		//Dependency.dstSubpass = 0;
		//Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//Dependency.srcAccessMask = 0;
		//Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;

		//VkSubpassDependency Dependency = {};
		//Dependency.srcSubpass = static_cast<uint32>(_Element.Attachment);
		//Dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
		//Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		//Dependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		//Dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		//Dependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		//Dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
		//
		//if (_Element.Next != nullptr)
		//	Dependency.dstSubpass = static_cast<uint32>(_Element.Next->Attachment);
		//else Dependency.dstSubpass = VK_SUBPASS_EXTERNAL;

		return Dependency;
	}

	Ref<VulkanRenderpass> VulkanRenderpass::Make(const RenderpassLayout& _Layout)
	{
		Ref<RenderpassCache> s_Cache = VulkanCache<VulkanRenderpass>::Get(VULKAN_RENDERPASS_CACHE_TYPE);
		Ref<VulkanRenderpass> s_VulkanRenderpass = CreateRef<VulkanRenderpass>(_Layout, s_Cache);
		s_Cache->Add(s_VulkanRenderpass->GetID(), s_VulkanRenderpass);
		return s_VulkanRenderpass;
	}

}