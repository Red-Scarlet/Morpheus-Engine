#include "Morppch.h"
#include "VulkanTextureBuffer.h"

#include "Platform/Vulkan/VulkanCommandSystem/VulkanCommand.h"
#include "Platform/Vulkan/VulkanCommandSystem/VulkanInstructions.h"
#include "Platform/Vulkan/VulkanCommandSystem/VulkanExecutionStack.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

// TODO: FIX 
#include <Include/stb_image.h>

namespace Morpheus {

	VulkanTextureBuffer::VulkanTextureBuffer(const String& _Filename, const Ref<TextureBufferCache>& _Cache)
		: m_Filepath(_Filename), m_Cache(_Cache)
	{
		MORP_PROFILE_FUNCTION();

		m_Device = VulkanInstance::GetInstance()->GetLogicalDevice();
		m_ID = m_Cache->GetCount();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] TextureBuffer #" + ToString(GetID()) + " Was Created!");
	}

	VulkanTextureBuffer::~VulkanTextureBuffer()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] TextureBuffer Was Destoryed!");
	}

	VkWriteDescriptorSet VulkanTextureBuffer::UpdateDescriptorSet(const VkDescriptorSet& _DescriptorSet)
	{
		m_Compiled = true;

		VkWriteDescriptorSet DescriptorWrite = {};
		DescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		DescriptorWrite.dstSet = _DescriptorSet;
		DescriptorWrite.dstBinding = 1;
		DescriptorWrite.dstArrayElement = 0;
		DescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DescriptorWrite.descriptorCount = 1;
		DescriptorWrite.pImageInfo = &m_BufferInfo;

		return DescriptorWrite;
	}

	void VulkanTextureBuffer::VulkanCreate()
	{
		int _width, _height, _channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* pixels = stbi_load(m_Filepath.c_str(), &_width, &_height, &_channels, STBI_rgb_alpha);	// FORCES ALPHA
		MORP_CORE_ASSERT(!pixels, "[STB_IMAGE] Failed to load Image!");
		m_Width = _width;
		m_Height = _height;

		TextureEnum _InternalFormat = TextureEnum::TX_NONE, _DataFormat = TextureEnum::TX_NONE;
		if (_channels == 4) {
			_InternalFormat = TextureEnum::TX_RGBA8;
			_DataFormat = TextureEnum::TX_RGBA;
		}
		else if (_channels == 3) {
			_InternalFormat = TextureEnum::TX_RGB8;
			_DataFormat = TextureEnum::TX_RGB;
		}

		m_InternalFormat = _InternalFormat;
		m_DataFormat = _DataFormat;

		MORP_CORE_ASSERT(m_InternalFormat == TextureEnum::TX_NONE || m_DataFormat == TextureEnum::TX_NONE, "Format not supported!");

		// TODO CHANGE 4 BASED ON RGBA|RGB
		uint32 BufferSize = m_Width * m_Height * 4;

		// Beginning of Staging

		VkBuffer StagingBuffer = {};
		VkDeviceMemory StagingMemory = {};

		VkBufferCreateInfo StagingCreateInfo = {};
		StagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		StagingCreateInfo.size = BufferSize;
		StagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		StagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = vkCreateBuffer(m_Device->GetDevice(), &StagingCreateInfo, nullptr, &StagingBuffer);
		MORP_CORE_ASSERT(result, "[VULKAN] TextureBuffer Staging Create Failure!");

		VkMemoryRequirements StagingMemoryRequirements = {};
		vkGetBufferMemoryRequirements(m_Device->GetDevice(), StagingBuffer, &StagingMemoryRequirements);
		uint32 StagingMemoryIndex = m_Device->FindMemoryType(StagingMemoryRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkMemoryAllocateInfo StagingAllocInfo = {};
		StagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		StagingAllocInfo.allocationSize = StagingMemoryRequirements.size;
		StagingAllocInfo.memoryTypeIndex = StagingMemoryIndex;
		result = vkAllocateMemory(m_Device->GetDevice(), &StagingAllocInfo, nullptr, &StagingMemory);
		MORP_CORE_ASSERT(result, "[VULKAN] TextureBuffer Staging Allocate Failure!");

		result = vkBindBufferMemory(m_Device->GetDevice(), StagingBuffer, StagingMemory, 0);
		MORP_CORE_ASSERT(result, "[VULKAN] TextureBuffer Staging Bind Failure!");

		void* pData = nullptr;
		vkMapMemory(m_Device->GetDevice(), StagingMemory, 0, BufferSize, 0, &pData);
		memcpy(pData, pixels, BufferSize);
		vkUnmapMemory(m_Device->GetDevice(), StagingMemory);
		stbi_image_free(pixels);

		VkImageCreateInfo ImageInfo = {};
		ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		ImageInfo.imageType = VK_IMAGE_TYPE_2D;
		ImageInfo.extent.width = m_Width;
		ImageInfo.extent.height = m_Height;
		ImageInfo.extent.depth = 1;
		ImageInfo.mipLevels = 1;
		ImageInfo.arrayLayers = 1;
		ImageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;//
		ImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;//
		ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;//
		ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		result = vkCreateImage(m_Device->GetDevice(), &ImageInfo, nullptr, &m_Image);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create Image!");

		VkMemoryRequirements ImageMemRequirements = {};
		vkGetImageMemoryRequirements(m_Device->GetDevice(), m_Image, &ImageMemRequirements);
		uint32 ImageMemoryIndex = m_Device->FindMemoryType(ImageMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		
		VkMemoryAllocateInfo AllocInfo = {};
		AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		AllocInfo.allocationSize = ImageMemRequirements.size;
		AllocInfo.memoryTypeIndex = ImageMemoryIndex;

		vkAllocateMemory(m_Device->GetDevice(), &AllocInfo, nullptr, &m_Memory);
		vkBindImageMemory(m_Device->GetDevice(), m_Image, m_Memory, 0);

		Transition(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		Submit(StagingBuffer);
		Transition(m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(m_Device->GetDevice(), StagingBuffer, nullptr);
		vkFreeMemory(m_Device->GetDevice(), StagingMemory, nullptr);

		// To Extract the name from filepath
		auto __LastSlash = m_Filepath.find_last_of("/\\");
		__LastSlash = __LastSlash == std::string::npos ? 0 : __LastSlash + 1;
		auto __LastDot = m_Filepath.rfind('.');
		auto __Count = __LastDot == std::string::npos ? m_Filepath.size() - __LastSlash : __LastDot - __LastSlash;
		m_Name = m_Filepath.substr(__LastSlash, __Count);

		VULKAN_CORE_WARN("[VULKAN] TextureLoaded: " + m_Name);
		
		CreateImageView();
		CreateSampler();

		m_BufferInfo.imageView = m_ImageView;
		m_BufferInfo.sampler = m_Sampler;
		m_BufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void VulkanTextureBuffer::VulkanDestory()
	{
		vkDestroySampler(m_Device->GetDevice(), m_Sampler, nullptr);
		vkDestroyImageView(m_Device->GetDevice(), m_ImageView, nullptr);
		vkDestroyImage(m_Device->GetDevice(), m_Image, nullptr);
		vkFreeMemory(m_Device->GetDevice(), m_Memory, nullptr);
	}

	void VulkanTextureBuffer::CreateImageView()
	{
		VkImageViewCreateInfo ViewInfo = {};
		ViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ViewInfo.image = m_Image;
		ViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ViewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		ViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ViewInfo.subresourceRange.baseMipLevel = 0;
		ViewInfo.subresourceRange.levelCount = 1;
		ViewInfo.subresourceRange.baseArrayLayer = 0;
		ViewInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(m_Device->GetDevice(), &ViewInfo, nullptr, &m_ImageView);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create ImageView!");
	}

	void VulkanTextureBuffer::CreateSampler()
	{
		VkSamplerCreateInfo SamplerInfo = {};
		SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		SamplerInfo.magFilter = VK_FILTER_LINEAR;
		SamplerInfo.minFilter = VK_FILTER_LINEAR;
		SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		SamplerInfo.anisotropyEnable = VK_FALSE;// VK_TRUE;
		SamplerInfo.maxAnisotropy = 16.0f;// properties.limits.maxSamplerAnisotropy;
		SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		SamplerInfo.unnormalizedCoordinates = VK_FALSE;
		SamplerInfo.compareEnable = VK_FALSE;
		SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		VkResult result = vkCreateSampler(m_Device->GetDevice(), &SamplerInfo, nullptr, &m_Sampler);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create ImageView!");
	}

	void VulkanTextureBuffer::Transition(const VkImage& Image, const VkFormat& _Format, const VkImageLayout& _OldLayout, const VkImageLayout& _NewLayout)
	{
		VkImageMemoryBarrier Barrier = {};
		Barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		Barrier.oldLayout = _OldLayout;
		Barrier.newLayout = _NewLayout;
		Barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		Barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		Barrier.image = Image;
		Barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		Barrier.subresourceRange.baseMipLevel = 0;
		Barrier.subresourceRange.levelCount = 1;
		Barrier.subresourceRange.baseArrayLayer = 0;
		Barrier.subresourceRange.layerCount = 1;
		
		VkPipelineStageFlags SourceStage = {};
		VkPipelineStageFlags DestinationStage = {};
		if (_OldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			Barrier.srcAccessMask = 0;
			Barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			SourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			DestinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} else if (_OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			Barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			Barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			SourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			DestinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} else MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Unsupported layout transition!")

		Ref<VulkanExecutionStack> Executor = VulkanExecutionStack::Make();
		Executor->MakeCommand<CommandBeginBuffer>(VulkanCommandInstructions::BeginBuffer);
		Executor->MakeCommand<CommandBindBarrier>(VulkanCommandInstructions::BindBarrier)->PopulateData(Barrier, SourceStage, DestinationStage);
		Executor->MakeCommand<CommandEndBuffer>(VulkanCommandInstructions::EndBuffer);
		Ref<VulkanCommandBuffer> Command = Executor->Compile();
		Executor->Submit(Command, ExecutionSubmitFlags::VULKAN_SUBMIT_NOW | ExecutionSubmitFlags::VULKAN_RESET | ExecutionSubmitFlags::VULKAN_RELEASE);
	}

	void VulkanTextureBuffer::Submit(const VkBuffer& _Buffer)
	{
		VkBufferImageCopy Region = {};
		Region.bufferOffset = 0;
		Region.bufferRowLength = 0;
		Region.bufferImageHeight = 0;
		Region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		Region.imageSubresource.mipLevel = 0;
		Region.imageSubresource.baseArrayLayer = 0;
		Region.imageSubresource.layerCount = 1;
		Region.imageOffset = { 0, 0, 0 };
		Region.imageExtent = { m_Width, m_Height, 1 };
		
		Ref<VulkanExecutionStack> Executor = VulkanExecutionStack::Make();
		Executor->MakeCommand<CommandBeginBuffer>(VulkanCommandInstructions::BeginBuffer);
		Executor->MakeCommand<CommandCopyImage>(VulkanCommandInstructions::CopyImage)->PopulateData(_Buffer, m_Image, Region);
		Executor->MakeCommand<CommandEndBuffer>(VulkanCommandInstructions::EndBuffer);
		Ref<VulkanCommandBuffer> Command = Executor->Compile();
		Executor->Submit(Command, ExecutionSubmitFlags::VULKAN_SUBMIT_NOW | ExecutionSubmitFlags::VULKAN_RESET | ExecutionSubmitFlags::VULKAN_RELEASE);
	}

	Ref<VulkanTextureBuffer> VulkanTextureBuffer::Make(const String& _Filename)
	{
		Ref<TextureBufferCache> s_Cache = VulkanCache<VulkanTextureBuffer>::Get(VULKAN_TEXTURE_BUFFER_CACHE_TYPE);
		Ref<VulkanTextureBuffer> s_VulkanTextureBuffer = CreateRef<VulkanTextureBuffer>(_Filename, s_Cache);
		s_Cache->Add(s_VulkanTextureBuffer->GetID(), s_VulkanTextureBuffer);
		return s_VulkanTextureBuffer;
	}

}