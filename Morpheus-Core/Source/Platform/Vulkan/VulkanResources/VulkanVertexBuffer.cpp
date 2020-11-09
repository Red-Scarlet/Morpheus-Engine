#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanVertexBuffer::VulkanVertexBuffer(QuadVertex* _Data, const uint32& _Size)
		: VulkanResource(VulkanResourceTypes::VulkanVertexBuffer), m_Data(_Data), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanCommandSystem>(VulkanGlobalTypes::VulkanCommand);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanVertexBuffer));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] VertexBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const uint32& _Size)
		: VulkanResource(VulkanResourceTypes::VulkanVertexBuffer), m_Data(nullptr), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_CommandSystem = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanCommandSystem>(VulkanGlobalTypes::VulkanCommand);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanVertexBuffer));

		MORP_CORE_WARN("[VULKAN] VertexBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] VertexBuffer Was Destoryed!");
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

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::Make(QuadVertex* _Data, const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_Data, _Size);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit(s_VulkanVertexBuffer);
		return s_VulkanVertexBuffer;
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::Make(const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_Size);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit(s_VulkanVertexBuffer);
		return s_VulkanVertexBuffer;
	}


}