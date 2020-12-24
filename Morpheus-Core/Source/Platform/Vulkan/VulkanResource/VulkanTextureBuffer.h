#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Morpheus/Renderer/RendererResources/TextureBuffer.h"

namespace Morpheus {

	enum class TextureEnum : uint8
	{ TX_NONE = 0, TX_RGBA, TX_RGBA8, TX_RGB8, TX_RGB };

	#define VULKAN_TEXTURE_BUFFER_CACHE_TYPE VulkanCacheType(3)
	class VulkanTextureBuffer : public TextureBuffer
	{
	public:
		typedef VulkanCacheMember<VulkanTextureBuffer> TextureBufferCache;

	public:
		VulkanTextureBuffer(const String& _Filepath, const Ref<TextureBufferCache>& _Cache);

		virtual ~VulkanTextureBuffer();
		virtual const uint32& GetID() const override { return m_ID; }

		VkWriteDescriptorSet UpdateDescriptorSet(const VkDescriptorSet& _DescriptorSet);
		const Boolean& IsCompiled() const { return m_Compiled; }

	private:
		void VulkanCreate();
		void VulkanDestory();

		void CreateImageView();
		void CreateSampler();

		void Transition(const VkImage& Image, const VkFormat& _Format, const VkImageLayout& _OldLayout, const VkImageLayout& _NewLayout);
		void Submit(const VkBuffer& _Buffer);

	private:
		Ref<VulkanLogicalDevice> m_Device;
		Ref<TextureBufferCache> m_Cache;

		VkImage m_Image;
		VkDeviceMemory m_Memory;

		VkImageView m_ImageView;
		VkSampler m_Sampler;
		VkDescriptorImageInfo m_BufferInfo;

		uint32 m_Width, m_Height;
		TextureEnum m_InternalFormat, m_DataFormat;	 

		String m_Filepath, m_Name;
		Boolean m_Compiled = false;
		uint32 m_ID;

	public:
		static Ref<VulkanTextureBuffer> Make(const String& _Filepath);
	};

}