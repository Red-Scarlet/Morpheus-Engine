#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"

#include "Morpheus/Renderer/RendererResources/IndexBuffer.h"

#include "VulkanResource.h"

namespace Morpheus {

	class VulkanIndexBuffer : public VulkanResource, public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32* _Indices, const uint32& _Size);
		virtual ~VulkanIndexBuffer();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const vk::Buffer& GetBuffer() { return m_VulkanBuffer.Buffer; }
		const uint32& GetCount() { return m_Count; }

	private:
		void Submit(const VulkanBuffer& _Staging);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommandSystem> m_CommandSystem;

		VulkanBuffer m_VulkanBuffer;
		uint32* m_Data;
		uint32 m_Count;
		uint32 m_Size;

	public:
		static Ref<VulkanIndexBuffer> Make(uint32* _Indices, const uint32& _Size);
	};

}