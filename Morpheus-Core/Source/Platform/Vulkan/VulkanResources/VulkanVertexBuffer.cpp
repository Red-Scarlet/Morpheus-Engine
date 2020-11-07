#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanVertexBuffer::VulkanVertexBuffer(const Memory32& _Data, const uint32& _Size)
		: m_Data(_Data), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Command = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanCommand>(VulkanResourceTypes::VulkanCommandBuffer);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanVertexBuffer));

		CreateVertexBuffer();
		String str = "[VULKAN] VertexBuffer #" + std::to_string(m_ID) + " Was Created!";
		MORP_CORE_WARN(str);
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const uint32& _Size)
		: m_Data(nullptr), m_Size(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Command = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanCommand>(VulkanResourceTypes::VulkanCommandBuffer);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanVertexBuffer));

		String str = "[VULKAN] VertexBuffer #" + std::to_string(m_ID) + " Was Created!";
		MORP_CORE_WARN(str);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeMemory(m_VulkanBuffer.Memory);
		Device.destroyBuffer(m_VulkanBuffer.Buffer);
		MORP_CORE_WARN("[VULKAN] VertexBuffer Was Destoryed!");
	}

	void VulkanVertexBuffer::SetData(const Memory32& _Data, const uint32& _Size)
	{
		m_Data = _Data;
		m_Size = _Size;
		CreateVertexBuffer();	// maybe add function to just zero bits and fill them in later
	}

	uint32 VulkanVertexBuffer::GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties)
	{
		auto GpuMemoryProps = _PhysicalDevice.getMemoryProperties();
		for (uint32_t i = 0; i < GpuMemoryProps.memoryTypeCount; i++) {
			if ((_TypeBits & 1) == 1)
				if ((GpuMemoryProps.memoryTypes[i].propertyFlags & _Properties) == _Properties)
					return i;
			_TypeBits >>= 1;
		}
		return 0;
	}

	void VulkanVertexBuffer::Submit(const vk::Buffer& StagingBuffer)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Queue Queue = m_Device->GetQueue();
		vk::CommandPool CommandPool = m_Command->GetPool();

		auto GetCommandBuffer = [&](bool begin)
		{
			vk::CommandBuffer cmdBuffer = Device.allocateCommandBuffers(
				vk::CommandBufferAllocateInfo(
					CommandPool,
					vk::CommandBufferLevel::ePrimary,
					1)
			)[0];

			// If requested, also start the new command buffer
			if (begin)
				cmdBuffer.begin(
					vk::CommandBufferBeginInfo()
				);
			return cmdBuffer;
		};

		// Buffer copies have to be submitted to a queue, so we need a command buffer for them
		// Note: Some devices offer a dedicated transfer queue (with only the transfer bit set) that may be faster when doing lots of copies
		vk::CommandBuffer copyCmd = GetCommandBuffer(true);

		Vector<vk::BufferCopy> copyRegions =
		{
			vk::BufferCopy(0, 0, m_Size)
		};

		// Vertex buffer
		copyCmd.copyBuffer(StagingBuffer, m_VulkanBuffer.Buffer, copyRegions);

		// Flushing the command buffer will also submit it to the queue and uses a fence to ensure that all commands have been executed before returning
		auto flushCommandBuffer = [&](vk::CommandBuffer commandBuffer)
		{
			commandBuffer.end();

			std::vector<vk::SubmitInfo> submitInfos = {
				vk::SubmitInfo(0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr)
			};

			// Create fence to ensure that the command buffer has finished executing
			vk::Fence fence = Device.createFence(vk::FenceCreateInfo());

			// Submit to the queue
			Queue.submit(submitInfos, fence);
			// Wait for the fence to signal that command buffer has finished executing
			Device.waitForFences(1, &fence, VK_TRUE, UINT_MAX);
			Device.destroyFence(fence);
			Device.freeCommandBuffers(CommandPool, 1, &commandBuffer);
		};

		flushCommandBuffer(copyCmd);
	}

	void VulkanVertexBuffer::CreateVertexBuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();

		VulkanBuffer VulkanStagingBuffer = CreateStagingBuffer();

		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.size = m_Size;
			CreateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
			CreateInfo.sharingMode = vk::SharingMode::eExclusive;
			CreateInfo.queueFamilyIndexCount = 1;
			CreateInfo.pQueueFamilyIndices = &QueueFamilyIndex;
		}
		m_VulkanBuffer.Buffer = Device.createBuffer(CreateInfo);
		vk::MemoryRequirements MemoryRequirements = Device.getBufferMemoryRequirements(m_VulkanBuffer.Buffer);

		m_VulkanBuffer.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(MemoryRequirements.size, GetMemoryTypeIndex(PhysicalDevice, MemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)));
		Device.bindBufferMemory(m_VulkanBuffer.Buffer, m_VulkanBuffer.Memory, 0);

		// After That is done!

		Submit(VulkanStagingBuffer.Buffer);
		Device.destroyBuffer(VulkanStagingBuffer.Buffer);
		Device.freeMemory(VulkanStagingBuffer.Memory);
	}

	VulkanBuffer VulkanVertexBuffer::CreateStagingBuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();
		vk::Device Device = m_Device->GetLogicalDevice();

		VulkanBuffer VulkanStagingBuffer;
		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.size = m_Size;
			CreateInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
			CreateInfo.sharingMode = vk::SharingMode::eExclusive;
			CreateInfo.queueFamilyIndexCount = 1;
			CreateInfo.pQueueFamilyIndices = &QueueFamilyIndex;
		}
		VulkanStagingBuffer.Buffer = Device.createBuffer(CreateInfo);
		vk::MemoryRequirements MemoryRequirements = Device.getBufferMemoryRequirements(VulkanStagingBuffer.Buffer);
		VulkanStagingBuffer.Memory = Device.allocateMemory(vk::MemoryAllocateInfo(MemoryRequirements.size, GetMemoryTypeIndex(PhysicalDevice, MemoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)));

		void* Data;
		Data = Device.mapMemory(VulkanStagingBuffer.Memory, 0, MemoryRequirements.size, vk::MemoryMapFlags());
		std::memcpy(Data, &m_Data, m_Size);
		Device.unmapMemory(VulkanStagingBuffer.Memory);
		Device.bindBufferMemory(VulkanStagingBuffer.Buffer, VulkanStagingBuffer.Memory, 0);

		return VulkanStagingBuffer;
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::VulkanCreate(const Memory32& _Data, const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_Data, _Size);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanVertexBuffer>(VulkanResourceTypes::VulkanVertexBuffer, s_VulkanVertexBuffer); // s_VulkanVertexBuffer->GetID()
		return s_VulkanVertexBuffer;
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::VulkanCreate(const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_Size);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanVertexBuffer>(VulkanResourceTypes::VulkanVertexBuffer, s_VulkanVertexBuffer); // s_VulkanVertexBuffer->GetID()
		return s_VulkanVertexBuffer;
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::VulkanDestory()
	{
		return Ref<VulkanVertexBuffer>();
	}

}