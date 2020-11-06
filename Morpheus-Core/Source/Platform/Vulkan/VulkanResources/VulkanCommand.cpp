#include "Morppch.h"
#include "VulkanCommand.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"

#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanResources/VulkanPipeline.h"

#include "Platform/Vulkan/VulkanUnknown/VulkanRenderpass.h"
#include "Platform/Vulkan/VulkanUnknown/VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanCommand::VulkanCommand()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanCommandBuffer));

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
	bool VulkanCommand::SetupCommands(const Vector4& ClearColor)
	{
		if (m_RecompilationRequired) {
			FreeBuffers();
			CreateCommandBuffers();

			Ref<VulkanResourceCache> ResourceCache = VulkanMemoryManager::GetInstance()->GetResourceCache();
			MORP_CORE_WARN("[VULKAN] Command System compilation has Started!");

			// TODO: Change into Get<VulkanShader>(VulkanBindablesTypes::Shader);
			Ref<VulkanDescriptorPool> _VulkanDescriptorPool = ResourceCache->Get<VulkanDescriptorPool>(VulkanResourceTypes::VulkanDescriptor);
			Ref<VulkanPipeline> _VulkanPipeline = ResourceCache->Get<VulkanPipeline>(VulkanResourceTypes::VulkanPipeline);

			Ref<VulkanRenderpass> _VulkanRenderpass = ResourceCache->Get<VulkanRenderpass>(VulkanResourceTypes::VulkanRenderpass);
			Ref<VulkanFramebuffer> _VulkanFramebuffer = ResourceCache->Get<VulkanFramebuffer>(VulkanResourceTypes::VulkanFrameBuffer);

			auto SwapchainBuffers = _VulkanFramebuffer->GetSwapchainBuffers();
			vk::Rect2D RenderArea = m_Swapchain->GetRenderArea();
			vk::Viewport Viewport = m_Swapchain->GetViewport();

			vk::RenderPass Renderpass = _VulkanRenderpass->GetRenderpass();
			vk::Pipeline Pipeline = _VulkanPipeline->GetPipeline();
			vk::PipelineLayout PipelineLayout = _VulkanPipeline->GetLayout();

			Vector<vk::ClearValue> ClearValues =
			{
				vk::ClearColorValue(std::array<float32, 4>{ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w}),
				vk::ClearDepthStencilValue(1.0f, 0)
			};


			Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
			uint32 VertexArrayCount = BindableCache->GetNextBindableID(VulkanBindableTypes::VulkanVertexArray);

			struct VertexArrayData {
				vk::Buffer VertexBuffer;
				vk::Buffer IndexBuffer;
				uint32 IndexCount;

				vk::DescriptorSet DescriptorSet;
			};

			Vector<VertexArrayData> VertexArrays;
			for (uint32 i = 0; i < VertexArrayCount; i++) {
				Ref<VulkanVertexArray> VertexArray = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, i);
				Ref<VulkanVertexBuffer> VertexBuffer = ResourceCache->Get<VulkanVertexBuffer>(VulkanResourceTypes::VulkanVertexBuffer, VertexArray->GetVertexBuffer()->GetID());
				Ref<VulkanIndexBuffer> IndexBuffer = ResourceCache->Get<VulkanIndexBuffer>(VulkanResourceTypes::VulkanIndexBuffer, VertexArray->GetIndexBuffer()->GetID());
				if(VertexArray->CheckBound())
					VertexArrays.push_back({ VertexBuffer->GetBuffer(), IndexBuffer->GetBuffer(), IndexBuffer->GetCount(), _VulkanDescriptorPool->GetDescriptorSet(i) });
			}

			for (uint32 i = 0; i < m_CommandBuffers.size(); i++) {
				vk::CommandBuffer& cmd = m_CommandBuffers[i];

				cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
				cmd.begin(vk::CommandBufferBeginInfo());

				vk::RenderPassBeginInfo RenderpassBeginInfo{};
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
				for (uint32 j = 0; j < VertexArrays.size(); j++) {
					cmd.bindVertexBuffers(0, 1, &VertexArrays[j].VertexBuffer, &offsets);
					cmd.bindIndexBuffer(VertexArrays[j].IndexBuffer, 0, vk::IndexType::eUint32);
					cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, PipelineLayout, 0, VertexArrays[j].DescriptorSet, nullptr);
					cmd.drawIndexed(VertexArrays[j].IndexCount, 1, 0, 0, 1);
				}
				cmd.endRenderPass();
				cmd.end();
			}
			m_RecompilationRequired = false;
			MORP_CORE_WARN("[VULKAN] Command System Compiled!");
			return true;
		}
		return false;
	}

	void VulkanCommand::FreeBuffers()
	{
		MORP_CORE_WARN("[VULKAN] FreeBuffer Was Called!");

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
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanCommand>(VulkanResourceTypes::VulkanCommandBuffer, s_VulkanCommand);
		return s_VulkanCommand;
	}

}