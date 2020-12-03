#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommand.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandList.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanExecutionStack.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanVertexBuffer::VulkanVertexBuffer(QuadVertex* _Data, const uint32& _Size)
		: m_Data(_Data), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetCommandSystem();

		VulkanCreate();
		m_ID = VulkanMemoryManager::GetInstance()->GetVertexBufferCache().Count();
		VULKAN_CORE_WARN("[VULKAN] VertexBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const uint32& _Size)
		: m_Data(nullptr), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetCommandSystem();

		VULKAN_CORE_WARN("[VULKAN] VertexBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] VertexBuffer Was Destoryed!");
	}

	void VulkanVertexBuffer::VulkanCreate()
	{
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();

		uint32 BufferSize = m_Size * sizeof(QuadVertex);
		VulkanBuffer VulkanStagingBuffer = {};

		vk::BufferCreateInfo StagingCreateInfo = {};
		{
			StagingCreateInfo.flags = vk::BufferCreateFlags();
			StagingCreateInfo.size = BufferSize;
			StagingCreateInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
			StagingCreateInfo.sharingMode = vk::SharingMode::eExclusive;
			StagingCreateInfo.queueFamilyIndexCount = 1;
			StagingCreateInfo.pQueueFamilyIndices = &QueueFamilyIndex;
			VulkanStagingBuffer.Buffer = Device.createBuffer(StagingCreateInfo);
		}

		vk::MemoryRequirements StagingMemoryRequirements = Device.getBufferMemoryRequirements(VulkanStagingBuffer.Buffer);
		uint32 StagingMemoryIndex = m_Device->GetMemoryTypeIndex(StagingMemoryRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		VulkanStagingBuffer.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(StagingMemoryRequirements.size, StagingMemoryIndex));

		void* Data = Device.mapMemory(VulkanStagingBuffer.Memory, 0, StagingMemoryRequirements.size, vk::MemoryMapFlags());
		memcpy(Data, m_Data, BufferSize);
		Device.unmapMemory(VulkanStagingBuffer.Memory);
		Device.bindBufferMemory(VulkanStagingBuffer.Buffer, VulkanStagingBuffer.Memory, 0);

		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.size = BufferSize;
			CreateInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
			CreateInfo.sharingMode = vk::SharingMode::eExclusive;
			CreateInfo.queueFamilyIndexCount = 1;
			CreateInfo.pQueueFamilyIndices = &QueueFamilyIndex;
			m_VulkanBuffer.Buffer = Device.createBuffer(CreateInfo);
		}

		vk::MemoryRequirements VertexMemoryRequirements = Device.getBufferMemoryRequirements(m_VulkanBuffer.Buffer);
		uint32 VertexMemoryIndex = m_Device->GetMemoryTypeIndex(VertexMemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_VulkanBuffer.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(VertexMemoryRequirements.size, VertexMemoryIndex));
		Device.bindBufferMemory(m_VulkanBuffer.Buffer, m_VulkanBuffer.Memory, 0);

		Submit(VulkanStagingBuffer);
		
		Device.destroyBuffer(VulkanStagingBuffer.Buffer);
		Device.freeMemory(VulkanStagingBuffer.Memory);

		m_Completed = true;
	}

	void VulkanVertexBuffer::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeMemory(m_VulkanBuffer.Memory);
		Device.destroyBuffer(m_VulkanBuffer.Buffer);
	}

	void VulkanVertexBuffer::SetData(const Memory32& _Data, const uint32& _Size)
	{
		m_Completed = false;
		VulkanCreate();
	}

	void VulkanVertexBuffer::Submit(const VulkanBuffer& _Staging)
	{
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();
		uint32 BufferSize = m_Size * sizeof(QuadVertex);

		Ref<VulkanCommandBuffer> CommandBuffer = VulkanCommandBuffer::Make(m_CommandSystem->Allocate());
		Vector<VkBufferCopy> CopyRegions = { vk::BufferCopy(0, 0, BufferSize) };
		
		Ref<VulkanExecutionStack> Executor = VulkanExecutionStack::Make();
		Executor->AppendCommand(CommandBeginBuffer::Create(CommandBuffer));

		Ref<CommandCopyBuffer> CopyBuffer = CommandCopyBuffer::Create(CommandBuffer);
		CopyBuffer->PopulateData(_Staging.Buffer, m_VulkanBuffer.Buffer, CopyRegions);
		Executor->AppendCommand(CopyBuffer);

		Executor->AppendCommand(CommandEndBuffer::Create(CommandBuffer));
		Executor->Compile();
		
		Vector<vk::SubmitInfo> SubmitInfos = { vk::SubmitInfo(0, nullptr, nullptr, 1, &vk::CommandBuffer(CommandBuffer->GetBuffer()), 0, nullptr) };
		vk::Fence fence = Device.createFence(vk::FenceCreateInfo());
		Queue.submit(SubmitInfos, fence);
		Device.waitForFences(1, &fence, VK_TRUE, UINT_MAX);
		Device.destroyFence(fence);
		m_CommandSystem->Deallocate(CommandBuffer->GetBuffer());
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::Make(QuadVertex* _Data, const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_Data, _Size);
		VulkanMemoryManager::GetInstance()->GetVertexBufferCache().Add(s_VulkanVertexBuffer->GetID(), s_VulkanVertexBuffer);

		return s_VulkanVertexBuffer;
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::Make(const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_Size);
		VulkanMemoryManager::GetInstance()->GetVertexBufferCache().Add(s_VulkanVertexBuffer->GetID(), s_VulkanVertexBuffer);
		return s_VulkanVertexBuffer;
	}


}