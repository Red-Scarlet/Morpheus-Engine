#include "Morppch.h"
#include "VulkanTextureBuffer.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommand.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandList.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanExecutionStack.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

// TODO: FIX 
#include <Include/stb_image.h>

namespace Morpheus {

	VulkanTextureBuffer::VulkanTextureBuffer(const String& _Filename)
		: m_Filepath(_Filename)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetCommandSystem();
		m_ID = VulkanMemoryManager::GetInstance()->GetTextureBufferCache().Count();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] TextureBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanTextureBuffer::~VulkanTextureBuffer()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] TextureBuffer Was Destoryed!");
	}

	vk::WriteDescriptorSet VulkanTextureBuffer::UpdateDescriptorSet(const VkDescriptorSet& _DescriptorSet)
	{
		m_HasUpdated = true;

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
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();

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

		// START VULKAN CODE

		uint32 BufferSize = m_Width * m_Height * 4; //TODO CHANGE 4 BASED ON RGBA|RGB
		VulkanBuffer VulkanStagingBuffer = {};

		vk::BufferCreateInfo StagingCreateInfo = {};
		{
			StagingCreateInfo.flags = vk::BufferCreateFlags();
			StagingCreateInfo.size = BufferSize;
			StagingCreateInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
			StagingCreateInfo.sharingMode = vk::SharingMode::eExclusive;
			StagingCreateInfo.queueFamilyIndexCount = 1;
			StagingCreateInfo.pQueueFamilyIndices = &QueueFamilyIndex;
			VulkanStagingBuffer.Buffer = Device.createBuffer(StagingCreateInfo);
		}

		vk::MemoryRequirements StagingMemoryRequirements = Device.getBufferMemoryRequirements(VulkanStagingBuffer.Buffer);
		uint32 StagingMemoryIndex = m_Device->GetMemoryTypeIndex(StagingMemoryRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		VulkanStagingBuffer.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(StagingMemoryRequirements.size, StagingMemoryIndex));

		void* Data = Device.mapMemory(VulkanStagingBuffer.Memory, 0, BufferSize, vk::MemoryMapFlags());
		memcpy(Data, pixels, BufferSize);
		Device.unmapMemory(VulkanStagingBuffer.Memory);
		Device.bindBufferMemory(VulkanStagingBuffer.Buffer, VulkanStagingBuffer.Memory, 0);
		stbi_image_free(pixels);

		VkImageCreateInfo ImageInfo = {};
		{
			ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			ImageInfo.imageType = VK_IMAGE_TYPE_2D;
			ImageInfo.extent.width = m_Width;
			ImageInfo.extent.height = m_Height;
			ImageInfo.extent.depth = 1;
			ImageInfo.mipLevels = 1;
			ImageInfo.arrayLayers = 1;
			ImageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;					// TODO: CHANGE LATER
			ImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;					// TODO: CHANEG LATER
			ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			ImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		VkResult result = vkCreateImage(Device, &ImageInfo, nullptr, &m_VulkanImage.Image);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create Image!");

		VkMemoryRequirements ImageMemRequirements;
		vkGetImageMemoryRequirements(Device, m_VulkanImage.Image, &ImageMemRequirements);
		uint32 ImageMemoryIndex = m_Device->GetMemoryTypeIndex(ImageMemRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_VulkanImage.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(ImageMemRequirements.size, ImageMemoryIndex));
		vkBindImageMemory(Device, m_VulkanImage.Image, m_VulkanImage.Memory, 0);

		Transition(m_VulkanImage.Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		Submit(VulkanStagingBuffer);
		Transition(m_VulkanImage.Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		Device.destroyBuffer(VulkanStagingBuffer.Buffer);
		Device.freeMemory(VulkanStagingBuffer.Memory);

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
		vk::Device Device = m_Device->GetLogicalDevice();

		vkDestroySampler(Device, m_Sampler, nullptr);
		vkDestroyImageView(Device, m_ImageView, nullptr);
		vkDestroyImage(Device, m_VulkanImage.Image, nullptr);
		vkFreeMemory(Device, m_VulkanImage.Memory, nullptr);
	}

	void VulkanTextureBuffer::CreateImageView()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		VkImageViewCreateInfo ViewInfo = {};
		{
			ViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ViewInfo.image = m_VulkanImage.Image;
			ViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ViewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			ViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ViewInfo.subresourceRange.baseMipLevel = 0;
			ViewInfo.subresourceRange.levelCount = 1;
			ViewInfo.subresourceRange.baseArrayLayer = 0;
			ViewInfo.subresourceRange.layerCount = 1;
		}

		VkResult result = vkCreateImageView(Device, &ViewInfo, nullptr, &m_ImageView);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create ImageView!");

	}

	void VulkanTextureBuffer::CreateSampler()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		VkSamplerCreateInfo SamplerInfo = {};
		{
			SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			SamplerInfo.magFilter = VK_FILTER_LINEAR;
			SamplerInfo.minFilter = VK_FILTER_LINEAR;

			SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

			SamplerInfo.anisotropyEnable = VK_FALSE;
			SamplerInfo.maxAnisotropy = 16.0f;

			SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			SamplerInfo.unnormalizedCoordinates = VK_FALSE;

			SamplerInfo.compareEnable = VK_FALSE;
			SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

			SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			SamplerInfo.mipLodBias = 0.0f;
			SamplerInfo.minLod = 0.0f;
			SamplerInfo.maxLod = 0.0f;
		}

		VkResult result = vkCreateSampler(Device, &SamplerInfo, nullptr, &m_Sampler);
		MORP_CORE_ASSERT(result, "[VULKAN] Failed to create ImageView!");
	}

	void VulkanTextureBuffer::Transition(const VkImage& Image, const VkFormat& _Format, const VkImageLayout& _OldLayout, const VkImageLayout& _NewLayout)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();

		VkImageMemoryBarrier Barrier = {};
		{
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
		}

		VkPipelineStageFlags SourceStage;
		VkPipelineStageFlags DestinationStage;

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

		Ref<VulkanCommandBuffer> CommandBuffer = VulkanCommandBuffer::Make(m_CommandSystem->Allocate());
		Ref<VulkanExecutionStack> Executor = VulkanExecutionStack::Make();
		Executor->AppendCommand(VulkanCommandList::BeginBuffer::Create(CommandBuffer));

		Ref<CommandBindBarrier> BindBarrier = CommandBindBarrier::Create(CommandBuffer);
		BindBarrier->PopulateData(Barrier, SourceStage, DestinationStage);
		Executor->AppendCommand(BindBarrier);

		Executor->AppendCommand(VulkanCommandList::EndBuffer::Create(CommandBuffer));
		Executor->Compile();

		Vector<vk::SubmitInfo> SubmitInfos = { vk::SubmitInfo(0, nullptr, nullptr, 1, &vk::CommandBuffer(CommandBuffer->GetBuffer()), 0, nullptr) };
		vk::Fence fence = Device.createFence(vk::FenceCreateInfo());
		Queue.submit(SubmitInfos, fence);
		Device.waitForFences(1, &fence, VK_TRUE, UINT_MAX);
		Device.destroyFence(fence);
		m_CommandSystem->Deallocate(CommandBuffer->GetBuffer());
	}

	void VulkanTextureBuffer::Submit(const VulkanBuffer& _Staging)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();

		VkBufferImageCopy Region = {};
		{
			Region.bufferOffset = 0;
			Region.bufferRowLength = 0;
			Region.bufferImageHeight = 0;
			Region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			Region.imageSubresource.mipLevel = 0;
			Region.imageSubresource.baseArrayLayer = 0;
			Region.imageSubresource.layerCount = 1;
			Region.imageOffset = { 0, 0, 0 };
			Region.imageExtent = { m_Width, m_Height, 1 };
		}

		Ref<VulkanCommandBuffer> CommandBuffer = VulkanCommandBuffer::Make(m_CommandSystem->Allocate());
		Ref<VulkanExecutionStack> Executor = VulkanExecutionStack::Make();
		Executor->AppendCommand(VulkanCommandList::BeginBuffer::Create(CommandBuffer));

		Ref<CommandCopyImage> CopyImage = CommandCopyImage::Create(CommandBuffer);
		CopyImage->PopulateData(_Staging.Buffer, m_VulkanImage.Image, Region);
		Executor->AppendCommand(CopyImage);

		Executor->AppendCommand(VulkanCommandList::EndBuffer::Create(CommandBuffer));
		Executor->Compile();

		Vector<vk::SubmitInfo> SubmitInfos = { vk::SubmitInfo(0, nullptr, nullptr, 1, &vk::CommandBuffer(CommandBuffer->GetBuffer()), 0, nullptr) };
		vk::Fence fence = Device.createFence(vk::FenceCreateInfo());
		Queue.submit(SubmitInfos, fence);
		Device.waitForFences(1, &fence, VK_TRUE, UINT_MAX);
		Device.destroyFence(fence);
		m_CommandSystem->Deallocate(CommandBuffer->GetBuffer());
	}

	Ref<VulkanTextureBuffer> VulkanTextureBuffer::Make(const String& _Filename)
	{
		Ref<VulkanTextureBuffer> s_VulkanTextureBuffer = CreateRef<VulkanTextureBuffer>(_Filename);
		VulkanMemoryManager::GetInstance()->GetTextureBufferCache().Add(s_VulkanTextureBuffer->GetID(), s_VulkanTextureBuffer);
		return s_VulkanTextureBuffer;
	}

}