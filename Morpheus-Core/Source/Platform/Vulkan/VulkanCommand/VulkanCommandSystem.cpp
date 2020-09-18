#include "Morppch.h"
#include "VulkanCommandSystem.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanVertexBuffer.h"

namespace Morpheus {

	// !--------------------------------------------------------------------------------------------------->

	void VulkanCommandBuffer::BeginRecord()
	{
		VkCommandBufferBeginInfo BeginInfo {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VkResult result = vkBeginCommandBuffer(m_VulkanObject.Buffers[m_VulkanObject.Index], &BeginInfo);
		MORP_CORE_ASSERT(result, "Failed to begin recording Command Buffer!");
	}

	void VulkanCommandBuffer::EndRecord()
	{
		VkResult result = vkEndCommandBuffer(m_VulkanObject.Buffers[m_VulkanObject.Index]);
		MORP_CORE_ASSERT(result, "Failed to to record Command Buffer!");
	}

	void VulkanCommandBuffer::cbSetViewport()
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanObject.Extent = Instance->GetPresentation()->GetExtent();
	}

	void VulkanCommandBuffer::cbSetIndex(const uint32& _Index)
	{
		m_VulkanObject.Index = _Index;
	}

	void VulkanCommandBuffer::cbSetClearcolor(const Vector4& _Color)
	{
		VkClearValue ClearColor = { _Color.x, _Color.y, _Color.z, _Color.w };
		m_VulkanObject.ClearColor = ClearColor;
	}

	void VulkanCommandBuffer::cbBindPipeline(const Ref<Pipeline>& _Pipeline)
	{
		vkCmdBindPipeline(m_VulkanObject.Buffers[m_VulkanObject.Index], VK_PIPELINE_BIND_POINT_GRAPHICS,
			CastRef<VulkanGraphicsPipeline>(_Pipeline)->GetPipeline());
	}

	void VulkanCommandBuffer::cbBeginRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		VkRenderPassBeginInfo BeginInfo;
		{
			BeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			BeginInfo.pNext = nullptr;
			BeginInfo.renderPass = CastRef<VulkanRenderpass>(_Renderpass)->GetRenderpass();
			BeginInfo.framebuffer = CastRef<VulkanRenderpass>(_Renderpass)->GetFramebuffer(m_VulkanObject.Index);
			BeginInfo.renderArea.offset = { 0, 0 };
			BeginInfo.renderArea.extent = m_VulkanObject.Extent;
			BeginInfo.clearValueCount = 1;
			BeginInfo.pClearValues = &m_VulkanObject.ClearColor;
		}

		vkCmdBeginRenderPass(m_VulkanObject.Buffers[m_VulkanObject.Index],
			&BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanCommandBuffer::cbEndRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		vkCmdEndRenderPass(m_VulkanObject.Buffers[m_VulkanObject.Index]);
	}

	void VulkanCommandBuffer::cbDraw(const Ref<VertexBuffer>& _VertexBuffer)
	{
		Ref<VulkanVertexBuffer> VertexBuffer = CastRef<VulkanVertexBuffer>(_VertexBuffer);
		VkBuffer Buffers[] = { VertexBuffer->GetBuffer() };
		VkDeviceSize Offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_VulkanObject.Buffers[m_VulkanObject.Index], 0, 1, Buffers, Offsets);
		vkCmdDraw(m_VulkanObject.Buffers[m_VulkanObject.Index], VertexBuffer->GetSize(), 1, 0, 0);
	}

	void VulkanCommandBuffer::PushBuffer(const VkCommandBuffer& _Buffer)
	{
		m_VulkanObject.Buffers.push_back(_Buffer);
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
		m_VulkanObject.Pool = new VulkanCommandPool(Instance->GetLogicalDevice(), Instance->GetPhysicalDevice());
		MORP_CORE_WARN("[VULKAN] CommandSystem Was Created!");

	}

	VulkanCommandSystem::~VulkanCommandSystem()
	{
		delete m_VulkanObject.Pool;
	}

	void VulkanCommandSystem::AllocateBuffers()
	{
		auto Instance = VulkanInstance::GetInstance();
		uint32 BufferSize = Instance->GetPresentation()->GetSize();

		Vector<VkCommandBuffer> Buffers(BufferSize);
		VkCommandBufferAllocateInfo AllocInfo {};
		{
			AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			AllocInfo.commandPool = m_VulkanObject.Pool->GetPool();
			AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			AllocInfo.commandBufferCount = BufferSize;
		}

		VkResult result = vkAllocateCommandBuffers(Instance->GetLogicalDevice()->GetDevice(), 
			&AllocInfo, Buffers.data());
		MORP_CORE_ASSERT(result, "Failed to allocate Command Buffers!");

		m_VulkanObject.Buffer = new VulkanCommandBuffer();

		for(uint32 i = 0; i < BufferSize; ++i)
			m_VulkanObject.Buffer->PushBuffer(Buffers[i]);
	}

	void VulkanCommandSystem::DeallocateBuffers()
	{
		auto Instance = VulkanInstance::GetInstance();
		for (uint32 i = 0; i < m_VulkanObject.Buffer->GetSize(); i++)
			vkFreeCommandBuffers(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Pool->GetPool(), 1,
				&m_VulkanObject.Buffer->GetBuffer(i));
	}

	void VulkanCommandSystem::CompileBuffers()
	{
		for (uint32 i = 0; i < m_VulkanObject.Buffer->GetSize(); i++)
			for (uint32 j = 0; j < m_VulkanObject.Commands.size(); j++) {
				m_VulkanObject.Buffer->cbSetIndex(i);
				auto Func = m_VulkanObject.Commands[j];
				Func(m_VulkanObject.Buffer, m_Description);
			}
		m_VulkanObject.Commands.clear();
	}

	const VkCommandPool& VulkanCommandSystem::GetCommandPool()
	{
		return m_VulkanObject.Pool->GetPool();
	}

	const VkCommandBuffer& VulkanCommandSystem::GetCommandBuffer(const uint32& _Index)
	{
		return m_VulkanObject.Buffer->GetBuffer(_Index);
	}

	void VulkanCommandSystem::BeginRecord()
	{
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->BeginRecord();
		});
	}

	void VulkanCommandSystem::EndRecord()
	{
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->EndRecord();
		});
	}

	void VulkanCommandSystem::SetViewport()
	{
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->cbSetViewport();
		});
	}

	void VulkanCommandSystem::SetClearcolor(const Vector4& _Color)
	{
		m_Description.Color = _Color;
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->cbSetClearcolor(CommandDesc.Color);
		});
	}

	void VulkanCommandSystem::BindPipeline(const Ref<Pipeline>& _Pipeline)
	{
		m_Description.Pipeline = _Pipeline;
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->cbBindPipeline(CommandDesc.Pipeline);
		});
	}

	void VulkanCommandSystem::BeginRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		m_Description.Renderpass = _Renderpass;
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->cbBeginRenderpass(CommandDesc.Renderpass);
		});
	}

	void VulkanCommandSystem::EndRenderpass(const Ref<Renderpass>& _Renderpass)
	{
		m_Description.Renderpass = _Renderpass;
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->cbEndRenderpass(CommandDesc.Renderpass);
		});
	}

	void VulkanCommandSystem::DrawGeomerty(const Ref<VertexBuffer>& _VertexBuffer)
	{
		m_Description.VertexBuffer = _VertexBuffer;
		m_VulkanObject.Commands.push_back([](VulkanCommandBuffer* BufferDesc, CommandDescription CommandDesc) {
			BufferDesc->cbDraw(CommandDesc.VertexBuffer);
		});
	}

}