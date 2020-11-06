#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"
#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

namespace Morpheus {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VertexData* _VertexData, const uint32& _Size);
		virtual ~VulkanVertexBuffer();
		void Destory();

		virtual const uint32& GetID() override { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::Buffer& GetBuffer() { return m_Vertices.Buffer; }

	private:
		uint32 GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties);
		void Submit(const vk::Buffer& StagingBuffer);
		void CreateVertexBuffer();

	private:
		struct {
			vk::DeviceMemory Memory;
			vk::Buffer Buffer;		
		} m_Vertices;

		// TODO: MAKE VERTEXBUFFER AS OBJECT INSIDE THE RENDERER @ MORPHEUS CORE
		VertexData* m_VertexData;
		uint32 m_VertexSize;

		uint32 m_BufferSize = 0;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommand> m_Command;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanVertexBuffer> VulkanCreate(VertexData* _VertexData, const uint32& _Size);

	};

}