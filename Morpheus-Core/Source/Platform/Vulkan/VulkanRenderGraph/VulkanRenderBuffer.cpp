#include "Morppch.h"
#include "VulkanRenderBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	VulkanRenderBuffer::VulkanRenderBuffer(const RenderBufferElement& _RenderElement)
	{
		m_Device = VulkanInstance::GetInstance()->GetDevice(0);
		m_Info = _RenderElement;

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] RenderBuffer Was Created!");
	}

	VulkanRenderBuffer::~VulkanRenderBuffer()
	{
		VULKAN_CORE_WARN("[VULKAN] RenderBuffer Was Destoryed!");
	}

	void VulkanRenderBuffer::VulkanCreate()
	{
		MORP_PROFILE_FUNCTION();

		VkImageAspectFlags AspectMask = {};
		VkImageLayout ImageLayout = {};
		VkImageUsageFlagBits Usage = GetVulkanUsageFromEngine(m_Info.Usage);
		VkFormat Format = {};

		if (Usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
			AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			Format = GetVulkanFormatFromEngine(m_Info.Format);
		}
		if (Usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
			AspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			ImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			Format = m_Device->FindFormatType({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
		}
		VULKAN_CORE_ASSERT(!(AspectMask > 0), "[VULKAN] RenderBuffer: AspectMask Error!");

		//albedoImage = createImage(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
		//	VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		//	VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, width, height);

		VkImageCreateInfo ImageCreateInfo = {};
		ImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		ImageCreateInfo.format = Format;
		ImageCreateInfo.extent.width = 1280;
		ImageCreateInfo.extent.height = 720;
		ImageCreateInfo.extent.depth = 1;
		ImageCreateInfo.mipLevels = 1;
		ImageCreateInfo.arrayLayers = 1;
		ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		ImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		ImageCreateInfo.usage = Usage | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		VkResult result = vkCreateImage(m_Device->GetLogical(), &ImageCreateInfo, nullptr, &m_Image);
		VULKAN_CORE_ASSERT(result, "[VULKAN] CreateImage Failure!");

		VkMemoryRequirements MemoryRequirements = {};
		vkGetImageMemoryRequirements(m_Device->GetLogical(), m_Image, &MemoryRequirements);
		uint32 ImageMemoryIndex = m_Device->FindMemoryType(MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkMemoryAllocateInfo ImageAllocInfo = {};
		ImageAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		ImageAllocInfo.allocationSize = MemoryRequirements.size;
		ImageAllocInfo.memoryTypeIndex = ImageMemoryIndex;

		result = vkAllocateMemory(m_Device->GetLogical(), &ImageAllocInfo, nullptr, &m_Memory);
		VULKAN_CORE_ASSERT(result, "[VULKAN] ImageAllocateMemory Failure!");

		result = vkBindImageMemory(m_Device->GetLogical(), m_Image, m_Memory, 0);
		VULKAN_CORE_ASSERT(result, "[VULKAN] BindImageMemory Failure!");

		VkImageViewCreateInfo ImageViewCreateInfo = {};
		ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ImageViewCreateInfo.format = GetVulkanFormatFromEngine(m_Info.Format);
		ImageViewCreateInfo.subresourceRange = {};
		ImageViewCreateInfo.subresourceRange.aspectMask = AspectMask;
		ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		ImageViewCreateInfo.subresourceRange.levelCount = 1;
		ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		ImageViewCreateInfo.subresourceRange.layerCount = 1;
		ImageViewCreateInfo.image = m_Image;
		result = vkCreateImageView(m_Device->GetLogical(), &ImageViewCreateInfo, nullptr, &m_ImageView);
		VULKAN_CORE_ASSERT(result, "[VULKAN] CreateImageView Failure!");
	}

	void VulkanRenderBuffer::VulkanDestory()
	{
		if (m_ImageView != nullptr)
			vkDestroyImageView(m_Device->GetLogical(), m_ImageView, nullptr);

		if (m_Image != nullptr) {
			vkDestroyImage(m_Device->GetLogical(), m_Image, nullptr);
			vkFreeMemory(m_Device->GetLogical(), m_Memory, nullptr);
		}
	}

	Ref<VulkanRenderBuffer> VulkanRenderBuffer::Create(const RenderBufferElement& _RenderElement)
	{
		return CreateRef<VulkanRenderBuffer>(_RenderElement);
	}

	void VulkanRenderBuffer::Destroy(const Ref<VulkanRenderBuffer>& _RenderBuffer)
	{
		_RenderBuffer->VulkanDestory();
	}

}}