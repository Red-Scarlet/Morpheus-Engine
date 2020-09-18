#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"
#include "Morpheus/Renderer/RendererCore/Pipeline.h"
#include "Morpheus/Renderer/RendererCore/VertexBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanCommandBuffer
	{
	public:
		friend class VulkanCommandSystem;

	public:
		//VulkanCommandBuffer(const VkCommandBuffer& _Buffer, const uint32& _Index);
		VulkanCommandBuffer() = default;

		void BeginRecord();
		void EndRecord();	
		void cbSetViewport();
		void cbSetIndex(const uint32& _Index);
		void cbSetClearcolor(const Vector4& _Color);
		void cbBindPipeline(const Ref<Pipeline>& _Pipeline);
		void cbBeginRenderpass(const Ref<Renderpass>& _Renderpass);
		void cbEndRenderpass(const Ref<Renderpass>& _Renderpass);
		void cbDraw(const Ref<VertexBuffer>& _VertexBuffer);
	
	private:
		const uint32& GetSize() { return m_VulkanObject.Buffers.size(); }
		const VkCommandBuffer& GetBuffer(const uint32& _Index) { return m_VulkanObject.Buffers[_Index]; }
		void PushBuffer(const VkCommandBuffer& _Buffer);

	private:
		struct {
			Vector<VkCommandBuffer> Buffers;
			VkClearValue ClearColor;
			VkExtent2D Extent;
			uint32 Index;
		} m_VulkanObject;

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
	
	struct CommandDescription {
		Ref<VertexBuffer> VertexBuffer;
		Ref<Pipeline> Pipeline;
		Ref<Renderpass> Renderpass;
		Vector4 Color;
	};

	class VulkanCommandSystem 
	{
	public:
		friend class VulkanSynchronization;
		using Command = Function<void(VulkanCommandBuffer*, CommandDescription)>;

	public:
		VulkanCommandSystem();
		~VulkanCommandSystem();

		void AllocateBuffers();
		void DeallocateBuffers();
		void CompileBuffers();

		const VkCommandPool& GetCommandPool();
		const VkCommandBuffer& GetCommandBuffer(const uint32& _Index);

	public:
		void BeginRecord();
		void EndRecord();
		void SetViewport();
		void SetClearcolor(const Vector4& _Color);
		void BindPipeline(const Ref<Pipeline>& _Pipeline);
		void BeginRenderpass(const Ref<Renderpass>& _Renderpass);
		void EndRenderpass(const Ref<Renderpass>& _Renderpass);
		void DrawGeomerty(const Ref<VertexBuffer>& _VertexBuffer);

	private:	
		struct {
			VulkanCommandPool* Pool;
			VulkanCommandBuffer* Buffer;
			Vector<Command> Commands;
		} m_VulkanObject;

		CommandDescription m_Description;
	};

}