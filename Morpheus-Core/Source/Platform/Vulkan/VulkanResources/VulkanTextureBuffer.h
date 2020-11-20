#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"

#include "Morpheus/Renderer/RendererResources/TextureBuffer.h"

namespace Morpheus {

	enum class TextureEnum : uint8
	{ TX_NONE = 0, TX_RGBA, TX_RGBA8, TX_RGB8, TX_RGB };

	class VulkanTextureBuffer : public TextureBuffer
	{
	public:
		VulkanTextureBuffer(const String& _Filepath);
		virtual ~VulkanTextureBuffer();
		virtual const uint32& GetID() const override { return m_ID; }

	private:
		void VulkanCreate();
		void VulkanDestory();

		void CreateImageView();
		void CreateSampler();

		void Transition(const VkImage& Image, const VkFormat& _Format, const VkImageLayout& _OldLayout, const VkImageLayout& _NewLayout);
		void Submit(const VulkanBuffer& _Staging);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommandSystem> m_CommandSystem;

		VulkanImage m_VulkanImage;
		VkImageView m_ImageView;
		VkSampler m_Sampler;

		uint32 m_Width, m_Height;
		TextureEnum m_InternalFormat, m_DataFormat;	 

		String m_Filepath, m_Name;
		uint32 m_ID;

	public:
		static Ref<VulkanTextureBuffer> Make(const String& _Filepath);
	};

	class VulkanTextureBufferCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanTextureBuffer>& _VertexBuffer)
		{ m_Cache[_ID] = _VertexBuffer; m_Count++; }

		const Ref<VulkanTextureBuffer>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find TextureBuffer #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanTextureBuffer>> m_Cache;
		uint32 m_Count = 0;
	};

}