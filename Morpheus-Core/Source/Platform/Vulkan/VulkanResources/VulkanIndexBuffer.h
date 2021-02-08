#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

namespace Morpheus { namespace Vulkan {

	class VulkanIndexBuffer : public IVulkanResource
	{
	public:
		VulkanIndexBuffer(const AnyData& _Data);
		virtual ~VulkanIndexBuffer();

		const VkBuffer& GetBuffer() { return m_Buffer; }
		uint32 GetCount() { return m_Data.Size / sizeof(uint32); }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		void Submit(const VkBuffer& _Buffer);
		
	private:
		Ref<VulkanDevice> m_Device;
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
		RendererBufferData m_Data;
		
	public:
		static Ref<VulkanIndexBuffer> Create(const AnyData& _Data);
		static void Destroy(const Ref<VulkanIndexBuffer>& _VertexBuffer);
	};

}}