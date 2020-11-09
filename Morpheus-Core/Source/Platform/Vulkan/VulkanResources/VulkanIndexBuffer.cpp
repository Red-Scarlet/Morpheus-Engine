#include "Morppch.h"
#include "VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanIndexBuffer::VulkanIndexBuffer(uint32* _Indices, const uint32& _Size)
		: VulkanResource(VulkanResourceTypes::VulkanIndexBuffer), m_Data(_Indices), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanCommandSystem>(VulkanGlobalTypes::VulkanCommand);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanIndexBuffer));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] IndexBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] IndexBuffer Was Destoryed!");
	}

	void VulkanIndexBuffer::VulkanCreate()
	{
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();

		m_Count = m_Size;
		uint32 BufferSize = m_Size * sizeof(uint32);

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

		void* Data = Device.mapMemory(VulkanStagingBuffer.Memory, 0, BufferSize, vk::MemoryMapFlags());
		memcpy(Data, m_Data, BufferSize);
		Device.unmapMemory(VulkanStagingBuffer.Memory);
		Device.bindBufferMemory(VulkanStagingBuffer.Buffer, VulkanStagingBuffer.Memory, 0);

		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.size = BufferSize;
			CreateInfo.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
			CreateInfo.sharingMode = vk::SharingMode::eExclusive;
			CreateInfo.queueFamilyIndexCount = 0;
			CreateInfo.pQueueFamilyIndices = nullptr;
			m_VulkanBuffer.Buffer = Device.createBuffer(CreateInfo);
		}

		vk::MemoryRequirements IndexMemoryRequirements = Device.getBufferMemoryRequirements(m_VulkanBuffer.Buffer);
		uint32 IndexMemoryIndex = m_Device->GetMemoryTypeIndex(IndexMemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_VulkanBuffer.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(IndexMemoryRequirements.size, IndexMemoryIndex));
		Device.bindBufferMemory(m_VulkanBuffer.Buffer, m_VulkanBuffer.Memory, 0);

		Submit(VulkanStagingBuffer);

		Device.destroyBuffer(VulkanStagingBuffer.Buffer);
		Device.freeMemory(VulkanStagingBuffer.Memory);
	}

	void VulkanIndexBuffer::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeMemory(m_VulkanBuffer.Memory);
		Device.destroyBuffer(m_VulkanBuffer.Buffer);
	}

	void VulkanIndexBuffer::Submit(const VulkanBuffer& _Staging)
	{
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();

		uint32 BufferSize = m_Size * sizeof(uint32);
		
		vk::CommandBuffer CommandBuffer = m_CommandSystem->Allocate();
		Vector<vk::BufferCopy> CopyRegions = { vk::BufferCopy(0, 0, BufferSize) };
		
		VulkanCommandBuffer CommandExecutor(CommandBuffer);
		CommandExecutor.BeginBuffer();
		CommandExecutor.Copy(_Staging.Buffer, m_VulkanBuffer.Buffer, CopyRegions);
		CommandExecutor.EndBuffer();
		CommandExecutor.Compile(false);
		
		Vector<vk::SubmitInfo> SubmitInfos = { vk::SubmitInfo(0, nullptr, nullptr, 1, &CommandBuffer, 0, nullptr) };
		vk::Fence fence = Device.createFence(vk::FenceCreateInfo());
		Queue.submit(SubmitInfos, fence);
		Device.waitForFences(1, &fence, VK_TRUE, UINT_MAX);
		Device.destroyFence(fence);
		m_CommandSystem->Deallocate(CommandBuffer);
	}

	Ref<VulkanIndexBuffer> VulkanIndexBuffer::Make(uint32* _Indices, const uint32& _Size)
	{
		Ref<VulkanIndexBuffer> s_VulkanIndexBuffer = CreateRef<VulkanIndexBuffer>(_Indices, _Size);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit(s_VulkanIndexBuffer);
		return s_VulkanIndexBuffer;
	}


}