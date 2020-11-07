#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"
#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

namespace Morpheus {

	struct VulkanBuffer
	{
	public:
		vk::DeviceMemory Memory;
		vk::Buffer Buffer;
	};

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(const Memory32& _Data, const uint32& _Size);
		VulkanVertexBuffer(const uint32& _Size);
		virtual ~VulkanVertexBuffer();

		virtual const uint32& GetID() override { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }
		
		virtual const VertexAttributeLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const VertexAttributeLayout& _Layout) override { m_Layout = _Layout; }
		virtual void SetData(const Memory32& _Data, const uint32& _Size) override;
		const vk::Buffer& GetBuffer() { return m_VulkanBuffer.Buffer; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void Submit(const vk::Buffer& StagingBuffer);
		void CreateVertexBuffer();
		VulkanBuffer CreateStagingBuffer();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommand> m_Command;
		uint32 m_ID = 0;

		VertexAttributeLayout m_Layout;
		VulkanBuffer m_VulkanBuffer;
		Memory32 m_Data;
		uint32 m_Size;

	public:
		static Ref<VulkanVertexBuffer> VulkanCreate(const Memory32& _Data, const uint32& _Size);
		static Ref<VulkanVertexBuffer> VulkanCreate(const uint32& _Size);
		static Ref<VulkanVertexBuffer> VulkanDestory();

	};

}