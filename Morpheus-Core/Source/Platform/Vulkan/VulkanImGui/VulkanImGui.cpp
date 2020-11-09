#include "Morppch.h"
#include "VulkanImGui.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

#include <imgui.h>
#include <examples/imgui_impl_vulkan.h>

#include <GLFW/glfw3.h>
#include "Morpheus/Core/Application.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"

namespace Morpheus {

	VulkanImGui::VulkanImGui()
	{
		m_Instance = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanInstance>(VulkanGlobalTypes::VulkanInstance);
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);

		m_Surface = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSurface>(VulkanGlobalTypes::VulkanSurface);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanCommandSystem>(VulkanGlobalTypes::VulkanCommand);
		m_Queue = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanQueue>(VulkanGlobalTypes::VulkanQueue);
	}

	void VulkanImGui::Init()
	{
		RenderpassLayout RPLayout = {
				{ RenderpassTypes::ATTACHMENT_COLOR, RenderpassAttachment::ATTACHMENT_LOAD, RenderpassAttachment::ATTACHMENT_STORE }
		};
		
		m_Renderpass = VulkanRenderpass::Make(RPLayout);
		m_Framebuffer = VulkanFramebuffer::Make(m_Renderpass, false);

		m_CommandBuffers.resize(m_Queue->GetBufferCount());
		VulkanCommands Buffers = m_CommandSystem->BatchAllocate(m_Queue->GetBufferCount());
		for (uint32 i = 0; i < Buffers.size(); i++)
			m_CommandBuffers[i] = VkCommandBuffer(Buffers[i]);
	
		CreateDescriptorPool();

		ImGui_ImplVulkan_InitInfo Info = {};
		{
			Info.Instance = m_Instance->GetInstance();
			Info.PhysicalDevice = m_Device->GetPhysicalDevice();
			Info.Device = m_Device->GetLogicalDevice();
			Info.QueueFamily = m_Device->GetQueueFamilyIndex();
			Info.Queue = m_Device->GetQueue();
			Info.PipelineCache = nullptr;
			Info.DescriptorPool = m_DescriptorPool;
			Info.Allocator = nullptr;
			Info.CheckVkResultFn = nullptr;
		}
		ImGui_ImplVulkan_Init(&Info, m_Renderpass->GetRenderpass());

		InitCommands();
	}

	void VulkanImGui::InitCommands()
	{
		uint32 QueueFamilyIndices = m_Device->GetQueueFamilyIndex();
		uint32 SwapchainSize = m_Swapchain->GetSize();
		
		// Use any command queue
		VkCommandPool command_pool = nullptr;
		VkCommandBuffer command_buffer = nullptr;
		
		VkCommandPoolCreateInfo PoolInfo{};
		{
			PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			PoolInfo.queueFamilyIndex = QueueFamilyIndices;
		}
		
		VkResult Result = vkCreateCommandPool(m_Device->GetLogicalDevice(), &PoolInfo, nullptr, &command_pool);
		MORP_CORE_ASSERT(Result, "Failed to create CommandPool!");
		
		VkCommandBufferAllocateInfo AllocInfo{};
		{
			AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			AllocInfo.commandPool = command_pool;
			AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			AllocInfo.commandBufferCount = 1;
		}
		
		Result = vkAllocateCommandBuffers(m_Device->GetLogicalDevice(),
			&AllocInfo, &command_buffer);
		
		MORP_CORE_ASSERT(Result, "Failed to allocate Command Buffers!");

		Result = vkResetCommandPool(m_Device->GetLogicalDevice(), command_pool, 0);
		VkCommandBufferBeginInfo BeginInfo = {};
		{
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			Result = vkBeginCommandBuffer(command_buffer, &BeginInfo);
		}	
		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
		
		VkSubmitInfo end_info = {};
		end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers = &command_buffer;
		Result = vkEndCommandBuffer(command_buffer);
		Result = vkQueueSubmit(m_Device->GetQueue(), 1, &end_info, VK_NULL_HANDLE);
		
		Result = vkDeviceWaitIdle(m_Device->GetLogicalDevice());
		ImGui_ImplVulkan_InvalidateFontUploadObjects();
	}

	void VulkanImGui::SetupCommands()
	{
		uint32 CurrentFrame = m_Queue->GetCurrentFrame();
		VkCommandBuffer cmd = m_CommandBuffers[CurrentFrame];
		{
			Vector<VkClearValue> ClearValues =
			{ VkClearValue({ 0.25f, 0.25f, 0.25f, 1.00f }) };

			vkResetCommandBuffer(cmd, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

			VkCommandBufferBeginInfo BeginInfo = {};
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			vkBeginCommandBuffer(cmd, &BeginInfo);	

			VkRenderPassBeginInfo RenderpassInfo = {};
			RenderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			RenderpassInfo.renderPass = m_Renderpass->GetRenderpass();
			RenderpassInfo.framebuffer = m_Framebuffer->GetFramebuffer(CurrentFrame);
			RenderpassInfo.renderArea.extent.width = 1280;			//Extent
			RenderpassInfo.renderArea.extent.height = 720;			//Extent
			RenderpassInfo.clearValueCount = ClearValues.size();
			RenderpassInfo.pClearValues = ClearValues.data();
			
			vkCmdBeginRenderPass(cmd, &RenderpassInfo, VK_SUBPASS_CONTENTS_INLINE);
			ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
			vkCmdEndRenderPass(cmd);

			vkEndCommandBuffer(cmd);
		}
		m_Queue->Submit(cmd);
	}


	void VulkanImGui::CreateDescriptorPool()
	{
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		CreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		CreateInfo.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		CreateInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		CreateInfo.pPoolSizes = pool_sizes;

		VkResult Result = vkCreateDescriptorPool(m_Device->GetLogicalDevice(), &CreateInfo, nullptr, &m_DescriptorPool);
		MORP_CORE_ASSERT(Result, "Failed to allocate Descriptor Pool!");
	}

}