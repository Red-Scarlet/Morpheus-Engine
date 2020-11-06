#include "Morppch.h"
#include "VulkanImGuiN.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

#include <imgui.h>
#include <examples/imgui_impl_vulkan.h>

#include <GLFW/glfw3.h>
#include "Morpheus/Core/Application.h"

namespace Morpheus {

	VulkanImGui::VulkanImGui()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Instance = m_Device->GetVulkanInstance();

		m_Surface = m_Device->GetSurface();
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		//m_RenderQueue = VulkanRenderQueue::VulkanCreate();
		m_RenderQueue = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanRenderQueue>(VulkanGlobalTypes::VulkanRenderQueue);

		//m_CommandBuffers
		
		//auto buffer = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanCommand>(VulkanResourceTypes::VulkanCommandBuffer);
		//m_CommandBuffers.resize(2);
		//m_CommandBuffers[0] = (buffer->GetCommandBuffer(0));
		//m_CommandBuffers[1] = (buffer->GetCommandBuffer(1));
		//m_CommandPool = buffer->GetPool();
		//m_Framebuffer = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanFramebuffer>(VulkanResourceTypes::VulkanFrameBuffer);
		//m_Renderpass = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanRenderpass>(VulkanResourceTypes::VulkanRenderpass);

	}

	void VulkanImGui::Init()
	{
		RenderpassLayout RPLayout = {
				{ RenderpassTypes::ATTACHMENT_COLOR, RenderpassAttachment::ATTACHMENT_LOAD, RenderpassAttachment::ATTACHMENT_STORE }
				//{ RenderpassTypes::ATTACHMENT_DEPTH, RenderpassAttachment::ATTACHMENT_LOAD, RenderpassAttachment::ATTACHMENT_DONTCARE }
		};
		m_Renderpass = VulkanRenderpass::VulkanCreate(RPLayout);
		m_Framebuffer = VulkanFramebuffer::VulkanCreate(m_Renderpass, false);

		CreateDescriptorPool();
		CreateCommandBuffer();


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
		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		Result = vkBeginCommandBuffer(command_buffer, &begin_info);

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
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 Size = m_Swapchain->GetSize();
		uint32 i = m_RenderQueue->GetCurrentFrame();

		VkCommandBuffer& cmd = m_CommandBuffers[i];
		Vector<VkClearValue> ClearValues = 
		{ VkClearValue({ 0.25f, 0.25f, 0.25f, 1.00f }) };
		VkResult Result;
		//VkResult Result = vkResetCommandPool(Device, m_CommandPool, 0);
		//MORP_CORE_ASSERT(Result, "Failed to reset command pool!");
		VkCommandBufferBeginInfo BeginInfo = {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		Result = vkBeginCommandBuffer(cmd, &BeginInfo);
		MORP_CORE_ASSERT(Result, "Failed to begin command buffer!");

		auto Framebuffers = m_Framebuffer->GetSwapchainBuffers();
		VkRenderPassBeginInfo RenderpassInfo = {};
		RenderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderpassInfo.renderPass = m_Renderpass->GetRenderpass();
		RenderpassInfo.framebuffer = Framebuffers[i].Framebuffer;
		RenderpassInfo.renderArea.extent.width = 1280;			//Extent
		RenderpassInfo.renderArea.extent.height = 720;			//Extent
		RenderpassInfo.clearValueCount = ClearValues.size();
		RenderpassInfo.pClearValues = ClearValues.data();
		vkCmdBeginRenderPass(cmd, &RenderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

		vkCmdEndRenderPass(cmd);
		Result = vkEndCommandBuffer(cmd);
		MORP_CORE_ASSERT(Result, "Failed to end command buffer!");

		m_RenderQueue->Submit(cmd);
		//MORP_CORE_INFO("[QUEUE] Submission made from IMGUIAPI #" + std::to_string(i) + "!");

	}

	void VulkanImGui::Flush()
	{


		//m_RenderQueue->Flush(false);
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

	void VulkanImGui::SetupVulkanStuff()
	{
	}

	void VulkanImGui::CreateCommandBuffer()
	{
		uint32 QueueFamilyIndices = m_Device->GetQueueFamilyIndex();
		uint32 SwapchainSize = m_Swapchain->GetSize();

		VkCommandPoolCreateInfo PoolInfo {};
		{
			PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			PoolInfo.queueFamilyIndex = QueueFamilyIndices;
		}

		VkResult Result1 = vkCreateCommandPool(m_Device->GetLogicalDevice(), &PoolInfo, nullptr, &m_CommandPool);
		MORP_CORE_ASSERT(Result1, "Failed to create CommandPool!");

		VkCommandBufferAllocateInfo AllocInfo{};
		{
			AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			AllocInfo.commandPool = m_CommandPool;
			AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			AllocInfo.commandBufferCount = SwapchainSize;
		}
		m_CommandBuffers.resize(SwapchainSize);

		VkResult Result2 = vkAllocateCommandBuffers(m_Device->GetLogicalDevice(),
			&AllocInfo, m_CommandBuffers.data());

		MORP_CORE_ASSERT(Result2, "Failed to allocate Command Buffers!");
	}

}