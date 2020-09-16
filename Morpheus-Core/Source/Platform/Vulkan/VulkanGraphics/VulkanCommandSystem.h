#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include "VulkanFramebuffer.h"
#include "VulkanGraphicsPipeline.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	struct VulkanCommandInformation 
	{
	public:
		Vector<VkCommandBuffer> Buffers;
		VulkanGraphicsPipeline* Pipeline;
		VulkanRenderpass* Renderpass;
		VulkanFramebuffer* Framebuffer;
		VkClearValue ClearColor;
		VkExtent2D Extent;
		uint32 SmartIndex;
	};

	class VulkanCommandBuffer
	{
	public:
		friend class VulkanCommandSystem;

	public:
		VulkanCommandBuffer();
		~VulkanCommandBuffer() = default;

		void BeginRecord();
		void EndRecord();

		void cbDraw();
		void cbSetViewport();	//TODO
		void cbSetClearcolor(const Vector4& _Color); //TODO

		void cbSetPipeline(VulkanGraphicsPipeline* _Pipeline);
		void cbSetFramebuffer(VulkanFramebuffer* _Framebuffer);
		void cbSetRenderpass(VulkanRenderpass* _Renderpass);

	private:
		void SetupBuffer(const VkCommandBuffer& _Buffer);
		const Vector<VkCommandBuffer>& CompileBuffer();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* VulkanPresentation;
		} m_VulkanCore;

		VulkanCommandInformation m_Info;
		Vector<Function<void(VulkanCommandInformation)>> m_Commands;
	};

	class VulkanCommandPool
	{
	public:
		friend class VulkanCommandSystem;
	public:
		VulkanCommandPool(VulkanLogicalDevice* _lDevice, VulkanPhysicalDevice* _pDevice);
		~VulkanCommandPool();

		const VkCommandPool& GetPool() { return m_VulkanObject.CommandPool; }

	private:
		void CreateCommandPool();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPhysicalDevice* pDevice;
		} m_VulkanCore;

		struct {
			VkCommandPool CommandPool;
		} m_VulkanObject;

	};

	class VulkanCommandSystem 
	{
	public:
		VulkanCommandSystem();
		~VulkanCommandSystem();

		void AllocateBuffer(VulkanCommandBuffer* _Buffer);
		void ComputeBuffer(VulkanCommandBuffer* _Buffer);


		const VkCommandPool& GetCommandPool() { return m_VulkanCommandPool->m_VulkanObject.CommandPool; }
		const VkCommandBuffer& GetCommandBuffer(const uint32& _Index) 
		{ 
			return m_CommandBuffers[_Index];
		}

	private:
		VulkanCommandPool* m_VulkanCommandPool;
		Vector<VkCommandBuffer> m_CommandBuffers;
	};

}