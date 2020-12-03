#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Morpheus/Renderer/RendererBindables/Shader.h"

namespace Morpheus {

	class VulkanUniformMessage
	{
	public:
		Vector<uint8> Data;

	public:
		template<typename DataType>
		friend VulkanUniformMessage& operator<< (VulkanUniformMessage& _Message, const DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			uint32 i = _Message.Data.size();
			_Message.Data.resize(_Message.Data.size() + sizeof(DataType));
			std::memcpy(_Message.Data.data() + i, &_Data, sizeof(DataType));

			return _Message;
		}

		template<typename DataType>
		friend VulkanUniformMessage& operator>> (VulkanUniformMessage& _Message, DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			uint32 i = _Message.Data.size() - sizeof(DataType);
			std::memcpy(&_Data, _Message.Data.data() + i, sizeof(DataType));
			_Message.Data.resize(i);

			return _Message;
		}
	};

	// Dispatch from the shader that this uniform will use.
	class VulkanUniformBuffer
	{
	public:
		VulkanUniformBuffer(const ShaderAttributeLayout& _Layout, const vk::DescriptorSet& _Set);
		virtual ~VulkanUniformBuffer();
		const uint32& GetID() const { return m_ID; }

		const vk::DescriptorSet& GetDescriptorSet() { return m_DescriptorSet; }

		void UpdateBufferData(const VulkanUniformMessage& _Message);
		vk::WriteDescriptorSet UpdateDescriptorSet();

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<VulkanDevice> m_Device;

		ShaderAttributeLayout m_Layout;
		VulkanBuffer m_Buffer;
		vk::DescriptorBufferInfo m_BufferInfo;
		vk::DescriptorSet m_DescriptorSet;
		uint32 m_ID;

	public:
		static Ref<VulkanUniformBuffer> Make(const ShaderAttributeLayout& _Layout, const vk::DescriptorSet& _Set);
	};

	class VulkanUniformBufferCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanUniformBuffer>& _VertexBuffer)
		{ m_Cache[_ID] = _VertexBuffer; m_Count++; }

		const Ref<VulkanUniformBuffer>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find UniformBuffer #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanUniformBuffer>> m_Cache;
		uint32 m_Count = 0;
	};

}