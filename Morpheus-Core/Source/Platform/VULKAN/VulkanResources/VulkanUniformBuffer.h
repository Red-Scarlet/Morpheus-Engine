#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"

#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

#include "Platform/VULKAN/VulkanDevice.h"
#include "Platform/VULKAN/VulkanSwapchain.h"

#include "VulkanDescriptorPool.h"

namespace Morpheus {

	struct VulkanUniformData
	{
	public:
		Matrix4 ProjectionMatrix;
		Matrix4 ViewMatrix;
		Matrix4 TransformMatrix;
	};

	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(const BufferLayout& _Layout);
		virtual ~VulkanUniformBuffer();
		void Destory();

		virtual const uint32& GetID() override { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		virtual void SetLayout(const BufferLayout& _Layout) override;
		virtual const BufferLayout& GetLayout() const override
		{ return m_BufferLayout; }

		const vk::DescriptorBufferInfo& GetDescriptorBuffer()
		{
			return m_Uniform.Descriptor;
		}

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void CreateUniformBuffer();

	private:
		VulkanUniformData m_UniformData;
		BufferLayout m_BufferLayout;

		struct {
			vk::DeviceMemory Memory;
			vk::Buffer Buffer;
			vk::DescriptorBufferInfo Descriptor;
		}  m_Uniform;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		//Ref<VulkanDescriptorPool> m_DescriptorPool;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanUniformBuffer> VulkanCreate(const BufferLayout& _Layout);
	};

}