#include "Morppch.h"
#include "VulkanRendererAPI.h"

#include "Platform/Vulkan/VulkanResources/VulkanPipeline.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	void VulkanRendererAPI::Init()
	{
		m_Queue = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanQueue>(VulkanGlobalTypes::VulkanQueue);
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanCommandSystem>(VulkanGlobalTypes::VulkanCommand);
	}

	void VulkanRendererAPI::Shutdown()
	{
		m_CommandSystem->BatchDeallocate(m_CommandBuffers);
	}

	void VulkanRendererAPI::Flush()
	{
		if (!m_Compiled)
			SetupCommands();

		m_Queue->Submit(m_CommandBuffers[m_Queue->GetCurrentFrame()]);
	}

	void VulkanRendererAPI::SetClearColor(const Vector4& _ClearColor)
	{
		if (m_ClearColor != _ClearColor) {
			m_ClearColor = _ClearColor;
			m_Compiled = false;
		}
	}

	void VulkanRendererAPI::SetCompile()
	{
		m_Compiled = false;
	}

	void VulkanRendererAPI::SetupCommands()
	{	
		MORP_CORE_WARN("[VULKAN] Compilation has Started!");
		m_CommandSystem->BatchDeallocate(m_CommandBuffers);
		m_CommandBuffers = m_CommandSystem->BatchAllocate(m_Queue->GetBufferCount());
		
		Ref<VulkanRenderpass> m_DefaultRenderpass = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanRenderpass>(VulkanResourceTypes::VulkanRenderpass);
		Ref<VulkanFramebuffer> m_DefaultFramebuffer = VulkanMemoryManager::GetInstance()->GetBindableCache()->Get<VulkanFramebuffer>(VulkanBindableTypes::VulkanFramebuffer);
		Ref<VulkanPipeline> m_DefaultPipeline = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanPipeline>(VulkanResourceTypes::VulkanPipeline);
		
		Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
		uint32 VertexArrayCount = BindableCache->GetNextBindableID(VulkanBindableTypes::VulkanVertexArray);
		
		auto Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		vk::Viewport Viewport = Swapchain->GetViewport();
		vk::Rect2D RenderArea = Swapchain->GetRenderArea();
		
		for (uint32 i = 0; i < m_Queue->GetBufferCount(); i++) {
			vk::CommandBuffer& cmd = m_CommandBuffers[i];
			VulkanCommandBuffer BufferExecutor(cmd);
		
			BufferExecutor.ResetBuffer();
			BufferExecutor.BeginBuffer();
			BufferExecutor.SetClearColor(m_ClearColor);
			BufferExecutor.BindFramebuffer(m_DefaultFramebuffer, i);
			BufferExecutor.BeginRenderpass(m_DefaultRenderpass);
			BufferExecutor.SetViewport(Viewport);
			BufferExecutor.SetScissor(RenderArea);
			BufferExecutor.BindPipeline(m_DefaultPipeline);
			for (uint32 j = 0; j < VertexArrayCount; j++) {
				Ref<VulkanVertexArray> VertexArray = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, j);
				BufferExecutor.SubmitVertexArray(VertexArray);
			}
			BufferExecutor.DrawIndexed();
			BufferExecutor.EndRenderpass();
			BufferExecutor.EndBuffer();
		
			BufferExecutor.Compile();
		}
		
		m_Compiled = true;
		MORP_CORE_WARN("[VULKAN] Compilation has Finished!");
	}


}