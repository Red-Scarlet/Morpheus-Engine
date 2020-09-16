#include "Morppch.h"
#include "VulkanCommandBuffer.h"

namespace Morpheus {

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation, VulkanFramebuffer* _Framebuffer, VulkanCommandPool* _CommandPool,
		VulkanRenderpass* _VulkanRenderpass, VulkanGraphicsPipeline* _GraphicsPipeline)
		: m_VulkanDevice(_lDevice), m_VulkanPresentation(_Presentation), m_VulkanFramebuffer(_Framebuffer), m_VulkanCommandPool(_CommandPool),
		m_VulkanRenderpass(_VulkanRenderpass), m_VulkanGraphicsPipeline(_GraphicsPipeline)
	{
		CreateCommandBuffer();
		MORP_CORE_WARN("[VULKAN] CommandBuffer Was Created!");
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}

	void VulkanCommandBuffer::CreateCommandBuffer()
	{
		uint32 FramebufferSize = m_VulkanFramebuffer->GetSize();
		m_CommandBuffers.resize(FramebufferSize);

		VkCommandBufferAllocateInfo allocInfo {};
		{
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_VulkanCommandPool->GetCommandPool();
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (UINT32)m_CommandBuffers.size();
		}

		VkResult result = vkAllocateCommandBuffers(m_VulkanDevice->GetDevice(), &allocInfo, m_CommandBuffers.data());
		MORP_CORE_ASSERT(result, "Failed to allocate Command Buffers!");

		for (UINT32 i = 0; i < m_CommandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			VkResult result_2 = vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo);
			MORP_CORE_ASSERT(result_2, "Failed to begin recording Command Buffer!");

			VkClearValue clearColor = { 0.02f, 0.02f, 0.02f, 1.0f };
			VkRenderPassBeginInfo renderPassInfo {};
			{
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = m_VulkanRenderpass->GetRenderpass();
				renderPassInfo.framebuffer = m_VulkanFramebuffer->GetFramebuffers(i);
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = m_VulkanPresentation->GetExtent();
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;
			}

			//DRAW COMMANDS
			vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanGraphicsPipeline->GetPipeline());
			vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(m_CommandBuffers[i]);

			VkResult result_3 = vkEndCommandBuffer(m_CommandBuffers[i]);
			MORP_CORE_ASSERT(result_3, "Failed to to record Command Buffer!");
		}

	}

}

