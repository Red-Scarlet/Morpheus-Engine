#include "Morppch.h"
#include "VulkanCommandBuffer.h"

namespace Morpheus {

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanInstance* Instance, VulkanDevice* Device, VulkanFramebuffer* Framebuffer, 
		VulkanCommandPool* CommandPool, VulkanSwapchain* Swapchain, VulkanRenderpass* Renderpass, VulkanGraphicsPipeline* GraphicsPipeline)
		: m_VulkanInstance(Instance), m_VulkanDevice(Device), m_VulkanFramebuffer(Framebuffer), 
		m_VulkanCommandPool(CommandPool), m_VulkanSwapchain(Swapchain), m_VulkanRenderpass(Renderpass), m_VulkanGraphicsPipeline(GraphicsPipeline)
	{
		CreateCommandBuffer();
		MORP_CORE_WARN("[VULKAN] Vulkan CommandBuffer Was Created!");
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
	}

	void VulkanCommandBuffer::CreateCommandBuffer()
	{
		UINT32 FramebufferSize = m_VulkanFramebuffer->GetFramebuffers().size();
		m_CommandBuffers.resize(FramebufferSize);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_VulkanCommandPool->GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (UINT32)m_CommandBuffers.size();

		VkResult result = vkAllocateCommandBuffers(m_VulkanDevice->GetLogicalDevice(), &allocInfo, m_CommandBuffers.data());
		MORP_CORE_ASSERT(result, "Failed to allocate Command Buffers!");

		for (UINT32 i = 0; i < m_CommandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			VkResult result_2 = vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo);
			MORP_CORE_ASSERT(result_2, "Failed to begin recording Command Buffer!");

			VkRenderPassBeginInfo renderPassInfo {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_VulkanRenderpass->GetRenderpass();
			renderPassInfo.framebuffer = m_VulkanFramebuffer->GetFramebuffers()[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_VulkanSwapchain->GetSwapExtent();

			VkClearValue clearColor = { 0.02f, 0.02f, 0.02f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

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

