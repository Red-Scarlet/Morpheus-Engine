#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanBuffer.h"
#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"

namespace Morpheus { namespace Vulkan {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(QuadVertex* _Data, const uint32& _Size);
		virtual ~VulkanVertexBuffer();	

		const VkBuffer& GetBuffer() { return m_Buffer->GetBuffer(); }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<VulkanBuffer> m_Buffer;
		QuadVertex* m_Data;
		uint32 m_Size;

	public:
		static Ref<VulkanVertexBuffer> Make(QuadVertex* _Data, const uint32& _Size);
	};

}}