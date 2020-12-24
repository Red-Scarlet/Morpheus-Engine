#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Morpheus/Renderer/RendererBindables/Shader.h"
#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

namespace Morpheus {

	class VulkanUniformMessage
	{
	public:
		uint8* GetData() { return m_Data.data(); }
		void ResizeData(const uint32& _Size) { m_Data.resize(_Size); }
		const uint32& GetSize() { return m_Data.size(); }

	public:
		template<typename DataType>
		friend VulkanUniformMessage& operator<< (VulkanUniformMessage& _Message, const DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
			uint32 i = _Message.GetSize();
			_Message.ResizeData(_Message.GetSize() + sizeof(DataType));
			std::memcpy(_Message.GetData() + i, &_Data, sizeof(DataType));
			return _Message;
		}

		template<typename DataType>
		friend VulkanUniformMessage& operator>> (VulkanUniformMessage& _Message, DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
			uint32 i = _Message.GetSize() - sizeof(DataType);
			std::memcpy(&_Data, _Message.GetData() + i, sizeof(DataType));
			_Message.ResizeData(i);
			return _Message;
		}

	private:
		Vector<uint8> m_Data;
	};

	#define VULKAN_UNIFORM_BUFFER_CACHE_TYPE VulkanCacheType(2)
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		typedef VulkanCacheMember<VulkanUniformBuffer> UniformBufferCache;

	public:
		VulkanUniformBuffer(const UniformAttributeLayout& _Layout, const Ref<UniformBufferCache>& _Cache);
		virtual ~VulkanUniformBuffer();
		const uint32& GetID() const { return m_ID; }

		virtual void SetInt(const String& _Name, const uint32& _Value) override;
		virtual void SetFloat(const String& _Name, const float32& _Value) override;
		virtual void SetFloat3(const String& _Name, const Vector3& _Value) override;
		virtual void SetFloat4(const String& _Name, const Vector4& _Value) override;
		virtual void SetMat4(const String& _Name, const Matrix4& _Matrix) override;

		void UpdateUniformBuffer();
		void UpdateBufferData(VulkanUniformMessage& _Message);
		VkWriteDescriptorSet UpdateDescriptorSet(const VkDescriptorSet& _DescriptorSet);
		const Boolean& IsCompiled() const { return m_Compiled; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<UniformBufferCache> m_Cache;
		Ref<VulkanLogicalDevice> m_Device;

		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
		VkDescriptorBufferInfo m_BufferInfo;

		UniformAttributeLayout m_Layout;
		mutable UnorderedMap<String, AnyData> m_UniformCache;

		Boolean m_Compiled = false;
		uint32 m_ID;

	public:
		static Ref<VulkanUniformBuffer> Make(const UniformAttributeLayout& _Layout);
	};

}