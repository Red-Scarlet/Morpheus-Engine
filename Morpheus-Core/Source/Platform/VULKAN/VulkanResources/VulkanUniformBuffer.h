#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"

#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

namespace Morpheus {

	class VulkanUniformMessage
	{
	public:
		Vector<uint8> Body;

	public:
		template<typename DataType>
		friend VulkanUniformMessage& operator<< (VulkanUniformMessage& _Message, const DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t i = _Message.Body.size();
			_Message.Body.resize(_Message.Body.size() + sizeof(DataType));
			std::memcpy(_Message.Body.data() + i, &_Data, sizeof(DataType));

			return _Message;
		}

		template<typename DataType>
		friend VulkanUniformMessage& operator>> (VulkanUniformMessage& _Message, DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t i = _Message.Body.size() - sizeof(DataType);
			std::memcpy(&_Data, _Message.Body.data() + i, sizeof(DataType));
			_Message.Body.resize(i);

			return _Message;
		}
	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(const BufferLayout& _Layout);
		virtual ~VulkanUniformBuffer();
		void Destory();

		virtual const uint32& GetID() override { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }
		
		const bool& GetCompiled() { return CompiledUniform; }
		void SetCompiled(const bool& _State) { CompiledUniform = _State; }

		virtual void SetLayout(const BufferLayout& _Layout) override;
		virtual const BufferLayout& GetLayout() const override
		{ return m_BufferLayout; }

		const vk::DescriptorBufferInfo& GetDescriptorBuffer()
		{ return m_Uniform.Descriptor; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void CreateUniformBuffer();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		struct {
			vk::DeviceMemory Memory;
			vk::Buffer Buffer;
			vk::DescriptorBufferInfo Descriptor;
		}  m_Uniform;

		BufferLayout m_BufferLayout;

		uint32 m_ID = 0;
		bool CompiledUniform = false;

	public:
		static Ref<VulkanUniformBuffer> VulkanCreate(const BufferLayout& _Layout);
	};

}