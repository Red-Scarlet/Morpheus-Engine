#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"
#include "Morpheus/Renderer/RendererCore/Pipeline.h"
#include "Morpheus/Renderer/RendererCore/Framebuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	struct VulkanCommandInformation 
	{
	public:
		Vector<VkCommandBuffer> Buffers;
		Ref<Pipeline> Pipeline;
		Ref<Renderpass> Renderpass;
		Ref<Framebuffer> Framebuffer;

		VkClearValue ClearColor;
		VkExtent2D Extent;
		uint32 SmartIndex;
	};

	class VulkanCommandBuffer
	{
	public:
		friend class VulkanCommandSystem;

	public:
		VulkanCommandBuffer(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation);
		~VulkanCommandBuffer() = default;

		void BeginRecord();
		void EndRecord();
		
		void cbDraw();
		void cbSetViewport();	//TODO
		void cbSetClearcolor(const Vector4& _Color); //TODO

		void cbSetRenderpass(const Ref<Renderpass>& _Renderpass);
		void cbSetPipeline(const Ref<Pipeline>& _Pipeline);
		void cbSetFramebuffer(const Ref<Framebuffer>& _Framebuffer);

	private:
		void SetupBuffer(const VkCommandBuffer& _Buffer);
		void FreeBuffers(VulkanCommandPool* _Pool);
		const Vector<VkCommandBuffer>& CompileBuffer();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
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
		friend class VulkanSynchronization;

	public:
		VulkanCommandSystem();
		~VulkanCommandSystem();

		void AllocateBuffers(VulkanCommandBuffer* _Buffer);
		void ComputeBuffers(VulkanCommandBuffer* _Buffer);
		void ResetBuffers(VulkanCommandBuffer* _Buffer);

		const VkCommandPool& GetCommandPool() { return m_VulkanCommandPool->m_VulkanObject.CommandPool; }
		const VkCommandBuffer& GetCommandBuffer(const uint32& _Index) 
		{ return m_CommandBuffers[_Index]; }

	private:
		const Ref<Renderpass>& GetRenderpass() { return m_Renderpass; }

	private:
		Ref<Renderpass> m_Renderpass;
		VulkanCommandPool* m_VulkanCommandPool;
		Vector<VkCommandBuffer> m_CommandBuffers;
	};

}