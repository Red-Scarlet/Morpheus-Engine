#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"

#include "Morpheus/Renderer/RendererResources/VertexBuffer.h"

#include "VulkanResource.h"

namespace Morpheus {

	class VulkanVertexBuffer : public VulkanResource, public VertexBuffer
	{
	public:
		VulkanVertexBuffer(QuadVertex* _Data, const uint32& _Size);
		VulkanVertexBuffer(const uint32& _Size);
		virtual ~VulkanVertexBuffer();
	
	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const vk::Buffer& GetBuffer() { return m_VulkanBuffer.Buffer; }
		virtual void SetData(const Memory32& _Data, const uint32& _Size) override;
		const bool& GetCompleted() { return m_Completed; }

	private:
		void Submit(const VulkanBuffer& _Staging);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanCommandSystem> m_CommandSystem;

		VulkanBuffer m_VulkanBuffer;
		QuadVertex* m_Data;
		uint32 m_Size;

		bool m_Completed = false;

	public:
		static Ref<VulkanVertexBuffer> Make(QuadVertex* _Data, const uint32& _Size);
		static Ref<VulkanVertexBuffer> Make(const uint32& _Size);

	};

}