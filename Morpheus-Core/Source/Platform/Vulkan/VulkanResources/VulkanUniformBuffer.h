#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"

#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

#include "VulkanResource.h"

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

	class VulkanUniformBuffer : public VulkanResource, public UniformBuffer
	{
	public:
		VulkanUniformBuffer(const BufferLayout& _Layout);
		virtual ~VulkanUniformBuffer();
	
	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const bool& GetCompiled() { return CompiledUniform; }
		void SetCompiled(const bool& _State) { CompiledUniform = _State; }

		virtual void SetLayout(const BufferLayout& _Layout) override;
		virtual const BufferLayout& GetLayout() const override
		{ return m_BufferLayout; }

		const vk::DescriptorBufferInfo& GetDescriptorBuffer()
		{ return m_Uniform.Descriptor; }

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		struct {
			vk::DeviceMemory Memory;
			vk::Buffer Buffer;
			vk::DescriptorBufferInfo Descriptor;
		}  m_Uniform;

		BufferLayout m_BufferLayout;
		bool CompiledUniform = false;

	public:
		static Ref<VulkanUniformBuffer> Make(const BufferLayout& _Layout);
	};

}