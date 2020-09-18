#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/VertexBuffer.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(const Vector<Vertex>& _Vertices);
		virtual ~VulkanVertexBuffer();

		const VkBuffer& GetBuffer() { return m_VulkanObject.Buffer; }
		const uint32& GetSize() { return m_VulkanObject.Vertices.size(); }

	private:
		void CreateVertexBuffer();

	private:
		struct {
			VkBuffer Buffer;
			VkDeviceMemory Memory;
			Vector<Vertex> Vertices;
		} m_VulkanObject;
	};

}