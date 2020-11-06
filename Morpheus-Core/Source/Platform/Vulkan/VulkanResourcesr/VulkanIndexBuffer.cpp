#include "Morppch.h"
#include "VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanIndexBuffer::VulkanIndexBuffer(uint32* _Indices, const uint32& _Size)
		: m_IndexData(_Indices), m_IndexSize(_Size)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Command = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanCommand>(VulkanResourceTypes::VulkanCommandBuffer);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanIndexBuffer));

		CreateIndexBuffer();
		String str = "[VULKAN] IndexBuffer #" + std::to_string(m_ID) + " Was Created!";
		MORP_CORE_WARN(str);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		MORP_CORE_WARN("[VULKAN] IndexBuffer Was Destoryed!");
	}

	void VulkanIndexBuffer::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeMemory(m_Indices.Memory);
		Device.destroyBuffer(m_Indices.Buffer);
	}

	void VulkanIndexBuffer::Submit(const vk::Buffer& StagingBuffer)
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

		// Index buffer
		Vector<vk::BufferCopy> copyRegions =
		{
			vk::BufferCopy(0, 0,  m_BufferSize)
		};

		copyCmd.copyBuffer(StagingBuffer, m_Indices.Buffer, copyRegions);

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

	uint32 VulkanIndexBuffer::GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties)
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

	void VulkanIndexBuffer::CreateIndexBuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();

		m_Indices.Count = m_IndexSize;
		m_BufferSize = m_Indices.Count * sizeof(uint32);

		struct {
			vk::DeviceMemory Memory;
			vk::Buffer Buffer;
		} StagingBuffer;

		StagingBuffer.Buffer = Device.createBuffer(
			vk::BufferCreateInfo(
				vk::BufferCreateFlags(),
				m_BufferSize,
				vk::BufferUsageFlagBits::eTransferSrc,
				vk::SharingMode::eExclusive,
				1,
				&QueueFamilyIndex
			)
		);

		auto memReqs = Device.getBufferMemoryRequirements(StagingBuffer.Buffer);

		StagingBuffer.Memory = Device.allocateMemory(
			vk::MemoryAllocateInfo(
				memReqs.size,
				GetMemoryTypeIndex(PhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
			)
		);

		void* data;
		data = Device.mapMemory(StagingBuffer.Memory, 0, m_BufferSize, vk::MemoryMapFlags());
		memcpy(data, m_IndexData, m_BufferSize);
		Device.unmapMemory(StagingBuffer.Memory);
		Device.bindBufferMemory(StagingBuffer.Buffer, StagingBuffer.Memory, 0);

		// Create destination buffer with device only visibility
		m_Indices.Buffer = Device.createBuffer(
			vk::BufferCreateInfo(
				vk::BufferCreateFlags(),
				m_BufferSize,
				vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
				vk::SharingMode::eExclusive,
				0,
				nullptr
			)
		);

		memReqs = Device.getBufferMemoryRequirements(m_Indices.Buffer);
		m_Indices.Memory = Device.allocateMemory(
			vk::MemoryAllocateInfo(
				memReqs.size,
				GetMemoryTypeIndex(PhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal
				)
			)
		);

		Device.bindBufferMemory(m_Indices.Buffer, m_Indices.Memory, 0);

		Submit(StagingBuffer.Buffer);
		Device.destroyBuffer(StagingBuffer.Buffer);
		Device.freeMemory(StagingBuffer.Memory);
	}

	Ref<VulkanIndexBuffer> VulkanIndexBuffer::VulkanCreate(uint32* _Indices, const uint32& _Size)
	{
		Ref<VulkanIndexBuffer> s_VulkanIndexBuffer = CreateRef<VulkanIndexBuffer>(_Indices, _Size);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanIndexBuffer>(VulkanResourceTypes::VulkanIndexBuffer, s_VulkanIndexBuffer); //s_VulkanIndexBuffer->GetID()
		return s_VulkanIndexBuffer;
	}


}