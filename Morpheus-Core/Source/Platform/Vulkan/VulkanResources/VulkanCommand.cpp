#include "Morppch.h"
#include "VulkanCommand.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanResources/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanUnknown/VulkanRenderpass.h"
#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

#include "Platform/Vulkan/VulkanResource.h"

namespace Morpheus {

	VulkanCommand::VulkanCommand()
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_Swapchain = VulkanResourceCache::GetInstance()->Get<VulkanSwapchain>(VulkanResourceType::Swapchain);

		CreateCommandPool();
		MORP_CORE_WARN("[VULKAN] Command System Was Created!");
	}

	VulkanCommand::~VulkanCommand()
	{
		FreeBuffers();
		Destory();
		MORP_CORE_ERROR("[VULKAN] Command System Was Destoryed!");
	}

	// TODO: Update the SetupCommands when new objects are created in runtime.
	void VulkanCommand::SetupCommands()
	{
		MORP_CORE_WARN("[VULKAN] Command System compilation has Started!");
		VulkanResourceCache* ResourceCache = VulkanResourceCache::GetInstance();
		Ref<VulkanDescriptorPool> _VulkanDescriptorPool = ResourceCache->Get<VulkanDescriptorPool>(VulkanResourceType::DescriptorPool);
		Ref<VulkanRenderpass> _VulkanRenderpass = ResourceCache->Get<VulkanRenderpass>(VulkanResourceType::Renderpass);
		Ref<VulkanPipeline> _VulkanPipeline = ResourceCache->Get<VulkanPipeline>(VulkanResourceType::Pipeline);

		auto SwapchainBuffers = m_Swapchain->GetSwapchainBuffers();
		vk::Rect2D RenderArea = m_Swapchain->GetRenderArea();
		vk::Viewport Viewport = m_Swapchain->GetViewport();

		vk::RenderPass Renderpass = _VulkanRenderpass->GetRenderpass();
		vk::Pipeline Pipeline = _VulkanPipeline->GetPipeline();
		vk::PipelineLayout PipelineLayout = _VulkanPipeline->GetLayout();

		Vector<vk::DescriptorSet> DSets;
		uint32 NumUniforms = VulkanResourceCache::GetInstance()->GetResourceCount(VulkanResourceType::UniformBuffer);
		for (uint32 i = 0; i < NumUniforms; i++)
			DSets.push_back(_VulkanDescriptorPool->GetDescriptorSet(i));

		Vector<vk::ClearValue> ClearValues =
		{
			vk::ClearColorValue(std::array<float32, 4>{0.2f, 0.2f, 0.2f, 1.0f}),
			vk::ClearDepthStencilValue(1.0f, 0)
		};

		struct VertexArray {
			vk::Buffer VertexBuffer;
			vk::Buffer IndexBuffer;
			uint32 IndexCount;
		};

		uint32 VertexArrayCount = ResourceCache->GetResourceCount(VulkanResourceType::VertexBuffer);
		Vector<VertexArray> VertexArrays;
		for (uint32 i = 0; i < VertexArrayCount; i++) {
			Ref<VulkanVertexBuffer> CurrentVertexBuffer = ResourceCache->Get<VulkanVertexBuffer>(VulkanResourceType::VertexBuffer, i);
			Ref<VulkanIndexBuffer> CurrentIndexBuffer = ResourceCache->Get<VulkanIndexBuffer>(VulkanResourceType::IndexBuffer, i);
			VertexArray Data;
			Data.VertexBuffer = CurrentVertexBuffer->GetBuffer();
			Data.IndexBuffer = CurrentIndexBuffer->GetBuffer();
			Data.IndexCount = CurrentIndexBuffer->GetCount();
			VertexArrays.push_back(Data);
		}

		for (uint32 i = 0; i < m_CommandBuffers.size(); i++) {
			vk::CommandBuffer& cmd = m_CommandBuffers[i];
		
			cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
			cmd.begin(vk::CommandBufferBeginInfo());

			vk::RenderPassBeginInfo RenderpassBeginInfo {};
			{
				RenderpassBeginInfo.renderPass = Renderpass;
				RenderpassBeginInfo.framebuffer = SwapchainBuffers[i].Framebuffer;
				RenderpassBeginInfo.renderArea = RenderArea;
				RenderpassBeginInfo.clearValueCount = (uint32)ClearValues.size();
				RenderpassBeginInfo.pClearValues = ClearValues.data();
			}

			cmd.beginRenderPass(RenderpassBeginInfo, vk::SubpassContents::eInline);
			cmd.setViewport(0, 1, &Viewport);
			cmd.setScissor(0, 1, &RenderArea);

			// Shared pipeline, e.g. if all objects are using the same shaders
			cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, Pipeline);

			// Put draw calls for all objects into this command buffer
			vk::DeviceSize offsets = 0;
			for (uint32 j = 0; j < VertexArrays.size(); j++){
				cmd.bindVertexBuffers(0, 1, &VertexArrays[j].VertexBuffer, &offsets);
				cmd.bindIndexBuffer(VertexArrays[j].IndexBuffer, 0, vk::IndexType::eUint32);

				//cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, PipelineLayout, 0, , nullptr);
				cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, PipelineLayout, 0, DSets[j], nullptr);
				cmd.drawIndexed(VertexArrays[j].IndexCount, 1, 0, 0, 1);
			}

			cmd.endRenderPass();
			cmd.end();
		}
	}

	void VulkanCommand::FreeBuffers()
	{
		MORP_CORE_WARN("[VULKAN] Command System Was Destoryed!");

		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeCommandBuffers(m_CommandPool, m_CommandBuffers);
	}

	void VulkanCommand::Destory()
	{
		MORP_CORE_WARN("[VULKAN] Command System Was Destoryed!");

		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyCommandPool(m_CommandPool);
	}

	void VulkanCommand::CreateCommandPool()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

		vk::CommandPoolCreateInfo CreateInfo {};
		{
			CreateInfo.flags = vk::CommandPoolCreateFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
			CreateInfo.queueFamilyIndex = QueueFamilyIndex;
		}

		m_CommandPool = Device.createCommandPool(CreateInfo);
		CreateCommandBuffers();
	}

	void VulkanCommand::CreateCommandBuffers()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 SwapchainSize = m_Swapchain->GetSize();

		m_CommandBuffers = Device.allocateCommandBuffers(
			vk::CommandBufferAllocateInfo(
				m_CommandPool,
				vk::CommandBufferLevel::ePrimary,
				(uint32)SwapchainSize
			)
		);
	}

	Ref<VulkanCommand> VulkanCommand::Create()
	{
		Ref<VulkanCommand> s_VulkanCommand = CreateRef<VulkanCommand>();
		VulkanResourceCache::GetInstance()->Submit<VulkanCommand>(VulkanResourceType::CommandSystem, s_VulkanCommand);
		return s_VulkanCommand;
	}

}