#include "Morppch.h"
#include "VulkanCommandSystem.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanFramebuffer.h"

namespace Morpheus {

	// !--------------------------------------------------------------------------------------------------->

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation)
		: m_VulkanCore({ _lDevice, _Presentation })
	{
		MORP_CORE_WARN("[VULKAN] CommandBuffer Was Created!");
	}

	void VulkanCommandBuffer::BeginRecord()
	{
		auto func = [](VulkanCommandInformation Info) {
			VkCommandBufferBeginInfo BeginInfo {};
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			VkResult result = vkBeginCommandBuffer(Info.Buffers[Info.SmartIndex], &BeginInfo);
			MORP_CORE_ASSERT(result, "Failed to begin recording Command Buffer!");

			VkRenderPassBeginInfo RenderPassInfo {};
			{
				RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				RenderPassInfo.renderPass = CastRef<VulkanRenderpass>(Info.Renderpass)->GetRenderpass();
				RenderPassInfo.framebuffer = CastRef<VulkanFramebuffer>(Info.Framebuffer)->GetFramebuffers(Info.SmartIndex);
				RenderPassInfo.renderArea.offset = { 0, 0 };
				RenderPassInfo.renderArea.extent = Info.Extent;
				RenderPassInfo.clearValueCount = 1;
				RenderPassInfo.pClearValues = &Info.ClearColor;
			}

			vkCmdBeginRenderPass(Info.Buffers[Info.SmartIndex], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(Info.Buffers[Info.SmartIndex], VK_PIPELINE_BIND_POINT_GRAPHICS,
				CastRef<VulkanGraphicsPipeline>(Info.Pipeline)->GetPipeline());
		};

		m_Commands.push_back(func);
	}

	void VulkanCommandBuffer::EndRecord()
	{
		auto func = [](VulkanCommandInformation Info) {
			vkCmdEndRenderPass(Info.Buffers[Info.SmartIndex]);
			VkResult result = vkEndCommandBuffer(Info.Buffers[Info.SmartIndex]);
			MORP_CORE_ASSERT(result, "Failed to to record Command Buffer!");
		};
		m_Commands.push_back(func);
	}

	void VulkanCommandBuffer::cbDraw()
	{
		auto func = [](VulkanCommandInformation Info) {
			vkCmdDraw(Info.Buffers[Info.SmartIndex], 3, 1, 0, 0);
		};
		m_Commands.push_back(func);
	}

	void VulkanCommandBuffer::cbSetViewport()
	{
		m_Info.Extent = m_VulkanCore.Presentation->GetExtent();
	}

	void VulkanCommandBuffer::cbSetClearcolor(const Vector4& _Color)
	{
		VkClearValue ClearColor = { _Color.x, _Color.y, _Color.z, _Color.w };
		m_Info.ClearColor = ClearColor;
	}

	void VulkanCommandBuffer::cbSetRenderpass(const Ref<Renderpass>&  _Renderpass)
	{
		m_Info.Renderpass = _Renderpass;
	}

	void VulkanCommandBuffer::cbSetPipeline(const Ref<Pipeline>& _Pipeline)
	{
		m_Info.Pipeline = _Pipeline;
	}

	void VulkanCommandBuffer::cbSetFramebuffer(const Ref<Framebuffer>& _Framebuffer)
	{
		m_Info.Framebuffer = _Framebuffer;
	}

	void VulkanCommandBuffer::SetupBuffer(const VkCommandBuffer& _Buffer)
	{
		m_Info.Buffers.push_back(_Buffer);
		m_Info.SmartIndex = 0;
	}

	void VulkanCommandBuffer::FreeBuffers(VulkanCommandPool* _Pool)
	{
		vkFreeCommandBuffers(m_VulkanCore.lDevice->GetDevice(), _Pool->GetPool(),
		m_Info.Buffers.size(), m_Info.Buffers.data());
		m_Info.Buffers.clear();
	}

	const Vector<VkCommandBuffer>& VulkanCommandBuffer::CompileBuffer()
	{
		for (uint32 cmd = 0; cmd < m_Info.Buffers.size(); cmd++) {
			for (uint32 i = 0; i < m_Commands.size(); i++) {
				auto Func = m_Commands[i];
				Func(m_Info);
			}
			m_Info.SmartIndex++;
		}
		m_Commands.clear();
		return m_Info.Buffers;
	}

	// !--------------------------------------------------------------------------------------------------->

	VulkanCommandPool::VulkanCommandPool(VulkanLogicalDevice* _lDevice, VulkanPhysicalDevice* _pDevice)
		: m_VulkanCore({_lDevice, _pDevice})
	{
		CreateCommandPool();
		MORP_CORE_WARN("[VULKAN] CommandPool Was Created!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.CommandPool, nullptr);
	}

	void VulkanCommandPool::CreateCommandPool()
	{
		QueueFamilyIndices QueueFamilyIndices = m_VulkanCore.pDevice->m_VulkanObject.Indices;

		VkCommandPoolCreateInfo PoolInfo {};
		{
			PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			PoolInfo.queueFamilyIndex = QueueFamilyIndices.GraphicsFamily.value();
		}

		VkResult result = vkCreateCommandPool(m_VulkanCore.lDevice->GetDevice(), &PoolInfo, nullptr, &m_VulkanObject.CommandPool);
		MORP_CORE_ASSERT(result, "Failed to create CommandPool!");
	}

	// !--------------------------------------------------------------------------------------------------->

	VulkanCommandSystem::VulkanCommandSystem()
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanCommandPool = new VulkanCommandPool(Instance->GetLogicalDevice(), Instance->GetPhysicalDevice());
		MORP_CORE_WARN("[VULKAN] CommandSystem Was Created!");

	}

	VulkanCommandSystem::~VulkanCommandSystem()
	{
		auto Instance = VulkanInstance::GetInstance();
		vkDestroyCommandPool(Instance->GetLogicalDevice()->GetDevice(), m_VulkanCommandPool->GetPool(), nullptr);
	}

	void VulkanCommandSystem::AllocateBuffers(VulkanCommandBuffer* _Buffer)
	{
		auto Instance = VulkanInstance::GetInstance();
		uint32 Size = Instance->GetPresentation()->GetSize();
		m_CommandBuffers.resize(Size);

		VkCommandBufferAllocateInfo AllocInfo {};
		{
			AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			AllocInfo.commandPool = m_VulkanCommandPool->GetPool();
			AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			AllocInfo.commandBufferCount = Size;
		}

		VkResult result = vkAllocateCommandBuffers(Instance->GetLogicalDevice()->GetDevice(), &AllocInfo, m_CommandBuffers.data());
		MORP_CORE_ASSERT(result, "Failed to allocate Command Buffers!");

		for (uint32 i = 0; i < Size; i++)
			_Buffer->SetupBuffer(m_CommandBuffers[i]);
		m_Renderpass = _Buffer->m_Info.Renderpass;
	}

	void VulkanCommandSystem::ComputeBuffers(VulkanCommandBuffer* _Buffer)
	{
		m_CommandBuffers = _Buffer->CompileBuffer();
	}

	void VulkanCommandSystem::ResetBuffers(VulkanCommandBuffer* _Buffer)
	{
		_Buffer->FreeBuffers(m_VulkanCommandPool);
	}

}