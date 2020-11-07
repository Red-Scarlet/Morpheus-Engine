#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"

namespace Morpheus {

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32* _Indices, const uint32& _Size);
		virtual ~VulkanIndexBuffer();
		void Destory();

		virtual const uint32& GetID() override { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::Buffer& GetBuffer() { return m_Indices.Buffer; }
		const uint32& GetCount() { return m_Indices.Count; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void Submit(const vk::Buffer& StagingBuffer);
		void CreateIndexBuffer();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommand> m_Command;

		struct {
			vk::DeviceMemory Memory;
			vk::Buffer Buffer;
			uint32_t Count;
		} m_Indices;

		uint32* m_IndexData;
		uint32 m_IndexSize;

		uint32 m_BufferSize = 0;	// TODO: REMOVE LATER

		uint32 m_ID = 0;

	public:
		static Ref<VulkanIndexBuffer> VulkanCreate(uint32* _Indices, const uint32& _Size);	// TODO: ADD SUPPORT FOR VECTOR<uint32>
	};

}