#include "Morppch.h"
#include "VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

#include "Platform/Vulkan/VulkanCommandSystem/VulkanCommand.h"
#include "Platform/Vulkan/VulkanCommandSystem/VulkanInstructions.h"

namespace Morpheus {

	VulkanFrameBuffer::VulkanFrameBuffer(const Ref<Renderpass>& _Renderpass, const Ref<FrameBufferCache> _Cache)
		: m_Renderpass(std::dynamic_pointer_cast<VulkanRenderpass>(_Renderpass)), m_Cache(_Cache)
	{
		MORP_PROFILE_FUNCTION();

		m_Device = VulkanInstance::GetInstance()->GetLogicalDevice();
		m_Swapchain = VulkanInstance::GetInstance()->GetSurface()->GetSwapchain();
		m_ID = m_Cache->GetCount();

		CreateFrameBuffers();
		VULKAN_CORE_WARN("[VULKAN] Framebuffer #" + ToString(GetID()) + " was Created!");
	}

	VulkanFrameBuffer::~VulkanFrameBuffer()
	{
		for (auto FrameBuffer : m_Framebuffers)
			vkDestroyFramebuffer(m_Device->GetDevice(), FrameBuffer, nullptr);

		VULKAN_CORE_WARN("[VULKAN] Framebuffer Was Destoryed!");
	}

	void* VulkanFrameBuffer::GetImage()
	{
		CopyImage(VulkanInstance::GetInstance()->GetQueue()->GetCurrentFrame());
		return SwapchainImageOutput;
	}

	void VulkanFrameBuffer::Bind()
	{
		m_Cache->SetBoundID(GetID());
	}

	void VulkanFrameBuffer::Unbind()
	{
		m_Cache->SetBoundID(uint32_max);
	}

	void VulkanFrameBuffer::CreateFrameBuffers()
	{
		VkFormat ColorFormat = m_Swapchain->GetFormat();
		VkExtent2D Extent = m_Swapchain->GetExtent();
		uint32 Size = m_Swapchain->GetImageCount();

		m_Images.resize(Size);
		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_Swapchain->GetSwapchain(), &Size, m_Images.data());

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
			VkResult result = vkCreateImageView(m_Device->GetDevice(), &CreateInfo, nullptr, &m_Views[i]);
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
			VkResult result = vkCreateFramebuffer(m_Device->GetDevice(), &FramebufferInfo, nullptr, &m_Framebuffers[i]);
			MORP_CORE_ASSERT(result, "Failed to create Framebuffer!");
		}
	}

	void VulkanFrameBuffer::CopyImage(const uint32& _Index)
	{
		VkImage SourceImage = m_Images[_Index];
		VkExtent2D Extent = m_Swapchain->GetExtent();

		VkImage DstImage = {};
		VkDeviceMemory DstImageMemory = {};

		VkImageCreateInfo ImageInfo = {};
		ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ImageInfo.imageType = VK_IMAGE_TYPE_2D;
		ImageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		ImageInfo.extent.width = Extent.width;
		ImageInfo.extent.height = Extent.height;
		ImageInfo.extent.depth = 1;
		ImageInfo.arrayLayers = 1;
		ImageInfo.mipLevels = 1;
		ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		ImageInfo.tiling = VK_IMAGE_TILING_LINEAR;
		ImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VkResult result = vkCreateImage(m_Device->GetDevice(), &ImageInfo, nullptr, &DstImage);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create Image!");

		VkMemoryRequirements ImageMemRequirements = {};
		vkGetImageMemoryRequirements(m_Device->GetDevice(), DstImage, &ImageMemRequirements);
		uint32 ImageMemoryIndex = m_Device->FindMemoryType(ImageMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkMemoryAllocateInfo AllocInfo = {};
		AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		AllocInfo.allocationSize = ImageMemRequirements.size;
		AllocInfo.memoryTypeIndex = ImageMemoryIndex;

		vkAllocateMemory(m_Device->GetDevice(), &AllocInfo, nullptr, &DstImageMemory);
		vkBindImageMemory(m_Device->GetDevice(), DstImage, DstImageMemory, 0);

		Ref<VulkanExecutionStack> Executor = VulkanExecutionStack::Make();
		Executor->MakeCommand<CommandBeginBuffer>(VulkanCommandInstructions::BeginBuffer);

		Transition(Executor,
			DstImage,
			0,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		Transition(Executor,
			SourceImage,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });


		Copy(Executor, SourceImage, DstImage, Extent.width, Extent.height);

		Transition(Executor, 
			DstImage,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		Transition(Executor,
			SourceImage,
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

		Executor->MakeCommand<CommandEndBuffer>(VulkanCommandInstructions::EndBuffer);
		Ref<VulkanCommandBuffer> Command = Executor->Compile();
		Executor->Submit(Command, ExecutionSubmitFlags::VULKAN_SUBMIT_NOW | ExecutionSubmitFlags::VULKAN_RESET | ExecutionSubmitFlags::VULKAN_RELEASE);

		VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
		VkSubresourceLayout subResourceLayout;
		vkGetImageSubresourceLayout(m_Device->GetDevice(), DstImage, &subResource, &subResourceLayout);

		const char* pData;
		vkMapMemory(m_Device->GetDevice(), DstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&pData);
		pData += subResourceLayout.offset;

		// SetData
		SwapchainImageOutput = (void*)pData;

		// Clean up resources
		vkUnmapMemory(m_Device->GetDevice(), DstImageMemory);
		vkFreeMemory(m_Device->GetDevice(), DstImageMemory, nullptr);
		vkDestroyImage(m_Device->GetDevice(), DstImage, nullptr);
	}

	void VulkanFrameBuffer::Transition(const Ref<VulkanExecutionStack>& _ExecutionStack, 
		const VkImage& _Image,
		const VkAccessFlags& srcAccessMask,
		const VkAccessFlags& dstAccessMask, 
		const VkImageLayout& oldImageLayout,
		const VkImageLayout& newImageLayout,
		const VkPipelineStageFlags& srcStageMask,
		const VkPipelineStageFlags& dstStageMask,
		const VkImageSubresourceRange& subresourceRange)
	{
		VkImageMemoryBarrier Barrier = {};
		Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		Barrier.oldLayout = oldImageLayout;
		Barrier.newLayout = newImageLayout;
		Barrier.image = _Image;
		Barrier.srcAccessMask = srcAccessMask;
		Barrier.dstAccessMask = dstAccessMask;
		Barrier.subresourceRange = subresourceRange;

		VkPipelineStageFlags SourceStage = srcStageMask;
		VkPipelineStageFlags DestinationStage = dstStageMask;

		_ExecutionStack->MakeCommand<CommandBindBarrier>(VulkanCommandInstructions::BindBarrier)->PopulateData(Barrier, SourceStage, DestinationStage);
	}

	void VulkanFrameBuffer::Copy(const Ref<VulkanExecutionStack>& _ExecutionStack, 
		const VkImage& _SourceImage,
		const VkImage& _DesinationImage, 
		const uint32& _Width, 
		const uint32& _Height)
	{
		VkImageCopy Region = {};
		Region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		Region.srcSubresource.layerCount = 1;
		Region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		Region.dstSubresource.layerCount = 1;
		Region.extent.width = _Width;
		Region.extent.height = _Height;
		Region.extent.depth = 1;

		_ExecutionStack->MakeCommand<CommandCopyImageToImage>(VulkanCommandInstructions::CopyImageToImage)->PopulateData(_SourceImage, _DesinationImage, Region);
	}

	Ref<VulkanFrameBuffer> VulkanFrameBuffer::Make(const Ref<Renderpass>& _Renderpass)
	{
		Ref<FrameBufferCache> s_Cache = VulkanCache<VulkanFrameBuffer>::Get(VULKAN_FRAMEBUFFER_CACHE_TYPE);
		Ref<VulkanFrameBuffer> s_VulkanFramebuffer = CreateRef<VulkanFrameBuffer>(_Renderpass, s_Cache);
		s_Cache->Add(s_VulkanFramebuffer->GetID(), s_VulkanFramebuffer);
		return s_VulkanFramebuffer;
	}

}