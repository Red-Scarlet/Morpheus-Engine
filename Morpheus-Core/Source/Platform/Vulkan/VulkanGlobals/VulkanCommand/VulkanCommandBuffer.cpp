#include "Morppch.h"
#include "VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanCommandBuffer::VulkanCommandBuffer(const vk::CommandBuffer& _Buffer)
		: m_Buffer(_Buffer)
	{
	}

	void VulkanCommandBuffer::ResetBuffer()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Reset Buffer called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::BeginBuffer()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.begin(vk::CommandBufferBeginInfo());
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Begin Buffer called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::EndBuffer()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.end();
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: End Buffer called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::Copy(const vk::Buffer& _SourceBuffer, const vk::Buffer& _DestinationBuffer, const Vector<vk::BufferCopy>& _CopyRegion)
	{
		m_DefaultData.SourceBuffer = _SourceBuffer;
		m_DefaultData.DestinationBuffer = _DestinationBuffer;
		m_DefaultData.CopyRegion = _CopyRegion;
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.copyBuffer(_Data.SourceBuffer, _Data.DestinationBuffer, _Data.CopyRegion);
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Copy Buffer called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::Function(ExecuteCommand _Command)
	{
		m_ExecutionStack.push_back(_Command);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SetViewport(const vk::Viewport& _Viewport)
	{
		m_DefaultData.Viewport = _Viewport;
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.setViewport(0, 1, &_Data.Viewport);
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Set Viewport called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SetScissor(const vk::Rect2D& _Scissor)
	{
		m_DefaultData.Scissor = _Scissor;
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.setScissor(0, 1, &_Data.Scissor);
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Set Scissor called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SetClearColor(const Vector4& _Clearcolor)
	{
		m_DefaultData.ClearColor = _Clearcolor;
		MORP_CORE_TRACE("[VULKAN] ExecutionStack: Set ClearColor called!");
	}

	void VulkanCommandBuffer::BindPipeline(const Ref<VulkanPipeline>& _Pipeline)
	{
		m_DefaultData.Pipeline = _Pipeline->GetPipeline();
		m_DefaultData.PipelineLayout = _Pipeline->GetLayout();
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			Ref<VulkanPipeline> m_DefaultPipeline = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanPipeline>(VulkanResourceTypes::VulkanPipeline);
			//_Data.PipelineLayout = m_DefaultPipeline->GetLayout();
			_Buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_DefaultPipeline->GetPipeline());
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Bind pipeline called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::BindFramebuffer(const Ref<VulkanFramebuffer>& _Framebuffer, const uint32& _Index)
	{
		m_DefaultData.Framebuffer = _Framebuffer->GetFramebuffer(_Index);
		MORP_CORE_TRACE("[VULKAN] ExecutionStack: Bind Framebuffer called!");
	}

	void VulkanCommandBuffer::BeginRenderpass(const Ref<VulkanRenderpass>& _Renderpass)
	{
		m_DefaultData.Renderpass = _Renderpass->GetRenderpass();
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			
			Vector<vk::ClearValue> ClearValues =
			{
				vk::ClearColorValue(std::array<float32, 4>{	_Data.ClearColor.x, _Data.ClearColor.y, _Data.ClearColor.z, _Data.ClearColor.w}),
				vk::ClearDepthStencilValue(1.0f, 0)
			};
		
			vk::RenderPassBeginInfo RenderpassBeginInfo = {};
			{
				RenderpassBeginInfo.renderPass = _Data.Renderpass;
				RenderpassBeginInfo.framebuffer = _Data.Framebuffer;
				RenderpassBeginInfo.renderArea = _Data.Scissor;
				RenderpassBeginInfo.clearValueCount = ClearValues.size();
				RenderpassBeginInfo.pClearValues = ClearValues.data();
			}

			_Buffer.beginRenderPass(RenderpassBeginInfo, vk::SubpassContents::eInline);
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Begin Renderpass called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::EndRenderpass()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.endRenderPass();
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: End Renderpass called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SubmitVertexArray(const Ref<VulkanVertexArray>& _VertexArray)
	{
		uint32 ID = _VertexArray->GetID();
		m_DefaultData.VertexArrays.emplace_back(ID);
	}

	void VulkanCommandBuffer::DrawIndexed()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			const uint32 Count = _Data.VertexArrays.size();
			struct VertexArrayData {
				vk::Buffer VertexBuffer;
				vk::Buffer IndexBuffer;
				uint32 IndexCount;
				vk::DescriptorSet DescriptorSet;
			};

			Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
			Ref<VulkanResourceCache> ResourceCache = VulkanMemoryManager::GetInstance()->GetResourceCache();
			Ref<VulkanDescriptorPool> DescriptorPool = ResourceCache->Get<VulkanDescriptorPool>(VulkanResourceTypes::VulkanDescriptor);

			Vector<VertexArrayData> VertexArrays;
			for (uint32 i = 0; i < Count; i++) {
				Ref<VulkanVertexArray> VertexArray = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, i);
				Ref<VulkanVertexBuffer> VertexBuffer = ResourceCache->Get<VulkanVertexBuffer>(VulkanResourceTypes::VulkanVertexBuffer, VertexArray->GetVertexBufferID());
				Ref<VulkanIndexBuffer> IndexBuffer = ResourceCache->Get<VulkanIndexBuffer>(VulkanResourceTypes::VulkanIndexBuffer, VertexArray->GetUniformBufferID());
				if (VertexArray->GetBound())
					VertexArrays.push_back({ VertexBuffer->GetBuffer(), IndexBuffer->GetBuffer(), IndexBuffer->GetCount(), DescriptorPool->GetDescriptorSet(i) });
			}

			vk::DeviceSize offsets = 0;
			for (uint32 i = 0; i < Count; i++) {
				_Buffer.bindVertexBuffers(0, 1, &VertexArrays[i].VertexBuffer, &offsets);
				_Buffer.bindIndexBuffer(VertexArrays[i].IndexBuffer, 0, vk::IndexType::eUint32);
				_Buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _Data.PipelineLayout, 0, VertexArrays[i].DescriptorSet, nullptr);
				_Buffer.drawIndexed(VertexArrays[i].IndexCount, 1, 0, 0, 1);
			}
			MORP_CORE_TRACE("[VULKAN] ExecutionStack: Draw Function called!");
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::Compile(bool ClearFlag)	// CHANGE TO BIT TYPE
	{		
		if (m_RecompileFlag) {
			for (uint32 i = 0; i < m_ExecutionStack.size(); i++)
				m_ExecutionStack[i](m_Buffer, m_DefaultData);
			m_ExecutionStack.clear();
			m_RecompileFlag = false;
			MORP_CORE_INFO("[VULKAN] Compiled Buffer!");
		}

		if(ClearFlag)
			m_DefaultData = {};
	}

	const VulkanCommandData& VulkanCommandBuffer::GetCommandData()
	{
		return m_DefaultData;
	}


}