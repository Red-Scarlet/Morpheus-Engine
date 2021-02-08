#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

namespace Morpheus { namespace Vulkan {

	class VulkanVertexBuffer : public IVulkanResource
	{
	public:
		VulkanVertexBuffer(const AnyData& _Data);
		virtual ~VulkanVertexBuffer();

		const VkBuffer& GetBuffer() { return m_Buffer; }

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
		static Ref<VulkanVertexBuffer> Create(const AnyData& _Data);
		static void Destroy(const Ref<VulkanVertexBuffer>& _VertexBuffer);
	};

}}