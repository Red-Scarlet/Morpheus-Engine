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
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::BeginBuffer()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			vk::CommandBufferBeginInfo BeginInfo = {};
			_Buffer.begin(BeginInfo);
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::EndBuffer()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			if (_Data.RenderpassCheck)
				_Buffer.endRenderPass();
			_Buffer.end();
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::CopyBuffer(const vk::Buffer& _SourceBuffer, const vk::Buffer& _DestinationBuffer, const Vector<vk::BufferCopy>& _CopyRegion)
	{
		m_DefaultData.SourceBuffer = _SourceBuffer;
		m_DefaultData.DestinationBuffer = _DestinationBuffer;
		m_DefaultData.CopyRegion = _CopyRegion;
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.copyBuffer(_Data.SourceBuffer, _Data.DestinationBuffer, _Data.CopyRegion);
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::CopyImage(const VkBuffer& _SourceBuffer, const VkImage& _DestinationImage, const VkBufferImageCopy& _CopyRegion)
	{
		m_DefaultData.SourceBuffer = _SourceBuffer;
		m_DefaultData.DestinationImage = _DestinationImage;
		m_DefaultData.ImageRegion = _CopyRegion;

		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			vkCmdCopyBufferToImage(_Buffer, _Data.SourceBuffer, _Data.DestinationImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &_Data.ImageRegion);
		};

		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::BindBarrier(const VkImageMemoryBarrier& _Barrier, const VkPipelineStageFlags& _SourceStage, const VkPipelineStageFlags& _DestinationStage)
	{
		m_DefaultData.Barrier = _Barrier;
		m_DefaultData.SourceStage = _SourceStage;
		m_DefaultData.DestinationStage = _DestinationStage;

		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			vkCmdPipelineBarrier(_Buffer, _Data.SourceStage, _Data.DestinationStage, 0, 0, nullptr, 0, nullptr, 1, &_Data.Barrier);
		};

		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SetClearColor(const Vector4& _Clearcolor)
	{
		m_DefaultData.ClearColor = _Clearcolor;
	}

	void VulkanCommandBuffer::SetViewport(const vk::Viewport& _Viewport)
	{
		m_DefaultData.Viewport = _Viewport;
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.setViewport(0, 1, &_Data.Viewport);
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SetScissor(const vk::Rect2D& _Scissor)
	{
		m_DefaultData.Scissor = _Scissor;
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.setScissor(0, 1, &_Data.Scissor);
		};
		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::BindFramebuffer(const Ref<VulkanFrameBuffer>& _Framebuffer, const uint32& _Index)
	{
		// Begin the render pass
		m_DefaultData.Framebuffer = _Framebuffer->GetFrameBuffer(_Index);
		m_DefaultData.Renderpass = _Framebuffer->GetRenderpass();
		m_DefaultData.RenderpassCheck = true;

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
		};

		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::BindShader(const Ref<VulkanShader>& _Shader)
	{
		m_DefaultData.Pipeline = _Shader->GetPipeline();
		m_DefaultData.PipelineLayout = _Shader->GetPipelineLayout();

		for (uint32 i = 0; i < _Shader->GetDescriptorCount(); i++)
			m_DefaultData.DescriptorSets.push_back(_Shader->GetDescriptorSet(i));

		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {
			_Buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _Data.Pipeline);
		};

		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::SubmitVertexArray(const uint32& _VertexArray)
	{
		MORP_CORE_INFO("[DEBUG] VAO SUBMITTED: " + std::to_string(_VertexArray));
		m_DefaultData.VertexArrays.emplace_back(_VertexArray);
	}

	void VulkanCommandBuffer::DrawIndexed()
	{
		auto Func = [](vk::CommandBuffer _Buffer, VulkanCommandData _Data) {	
			const uint32 Count = _Data.VertexArrays.size();
			struct VertexArrayData {
				vk::Buffer VertexBuffer;
				vk::Buffer IndexBuffer;
				uint32 IndexCount;
				uint32 DescriptorSet;
			};

			Vector<VertexArrayData> VertexArrays;
			for (uint32 i = 0; i < Count; i++) {
				Ref<VulkanVertexArray> VertexArray = VulkanMemoryManager::GetInstance()->GetVertexArrayCache().Get(_Data.VertexArrays[i]);
				vk::Buffer VertexBuffer = VulkanMemoryManager::GetInstance()->GetVertexBufferCache().Get(VertexArray->GetVertexBufferID())->GetBuffer();
				vk::Buffer IndexBuffer = VulkanMemoryManager::GetInstance()->GetIndexBufferCache().Get(VertexArray->GetIndexBufferID())->GetBuffer();
				uint32 IndexCount = VulkanMemoryManager::GetInstance()->GetIndexBufferCache().Get(VertexArray->GetIndexBufferID())->GetCount();
				uint32 DescriptorSet = VertexArray->GetUniformBufferID();
				VertexArrays.push_back({ VertexBuffer, IndexBuffer, IndexCount, DescriptorSet });
			}

			vk::DeviceSize offsets = 0;
			for (uint32 i = 0; i < Count; i++) {
				_Buffer.bindVertexBuffers(0, 1, &VertexArrays[i].VertexBuffer, &offsets);
				_Buffer.bindIndexBuffer(VertexArrays[i].IndexBuffer, 0, vk::IndexType::eUint32);
				_Buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _Data.PipelineLayout, 0, _Data.DescriptorSets[VertexArrays[i].DescriptorSet], nullptr);
				_Buffer.drawIndexed(VertexArrays[i].IndexCount, 1, 0, 0, 1);
			}
		};

		m_ExecutionStack.push_back(Func);
		m_RecompileFlag = true;
	}

	void VulkanCommandBuffer::Compile(bool ClearFlag)
	{		
		if (m_RecompileFlag) {
			for (uint32 i = 0; i < m_ExecutionStack.size(); i++)
				m_ExecutionStack[i](m_Buffer, m_DefaultData);
			m_ExecutionStack.clear();
			m_RecompileFlag = false;
		}
		//if(ClearFlag) m_DefaultData = {};
	}

}