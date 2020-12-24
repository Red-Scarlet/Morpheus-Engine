#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanBuffer.h"
#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"

namespace Morpheus { namespace Vulkan {

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32* _Indices, const uint32& _Size);
		virtual ~VulkanIndexBuffer();

		const VkBuffer& GetBuffer() { return m_Buffer->GetBuffer(); }
		uint32 GetCount() { return m_Size / sizeof(uint32); }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<VulkanBuffer> m_Buffer;
		uint32* m_Data;
		uint32 m_Size;

	public:
		static Ref<VulkanIndexBuffer> Make(uint32* _Indices, const uint32& _Size);
	};

}}