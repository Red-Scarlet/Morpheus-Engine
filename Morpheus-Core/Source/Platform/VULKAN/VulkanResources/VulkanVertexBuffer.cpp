#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

namespace Morpheus {

	VulkanVertexBuffer::VulkanVertexBuffer(VertexData* _VertexData, const uint32& _Size)
		: m_VertexData(_VertexData), m_VertexSize(_Size)
	{
		SetID(VulkanResourceCache::GetInstance()->GetNextResourceID(VulkanResourceType::VertexBuffer));

		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_Command = VulkanResourceCache::GetInstance()->Get<VulkanCommand>(VulkanResourceType::CommandSystem);

		CreateVertexBuffer();
		String str = "[VULKAN] VertexBuffer #" + std::to_string(m_ID) + " Was Created!";
		MORP_CORE_WARN(str);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		MORP_CORE_WARN("[VULKAN] VertexBuffer Was Destoryed!");
	}

	void VulkanVertexBuffer::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeMemory(m_Vertices.Memory);
		Device.destroyBuffer(m_Vertices.Buffer);
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
			vk::BufferCopy(0, 0, m_BufferSize)
		};

		// Vertex buffer
		copyCmd.copyBuffer(StagingBuffer, m_Vertices.Buffer, copyRegions);

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
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 QueueFamilyIndex = m_Device->GetQueueFamilyIndex();


		m_BufferSize = m_VertexSize *sizeof(VertexData);

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
		data = Device.mapMemory(StagingBuffer.Memory, 0, memReqs.size, vk::MemoryMapFlags());
		memcpy(data, m_VertexData, m_BufferSize);
		Device.unmapMemory(StagingBuffer.Memory);
		Device.bindBufferMemory(StagingBuffer.Buffer, StagingBuffer.Memory, 0);

		// Create a device local buffer to which the (host local) vertex data will be copied and which will be used for rendering
		m_Vertices.Buffer = Device.createBuffer(
			vk::BufferCreateInfo(
				vk::BufferCreateFlags(),
				m_BufferSize,
				vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
				vk::SharingMode::eExclusive,
				1,
				&QueueFamilyIndex
			)
		);

		memReqs = Device.getBufferMemoryRequirements(m_Vertices.Buffer);

		m_Vertices.Memory = Device.allocateMemory(
			vk::MemoryAllocateInfo(
				memReqs.size,
				GetMemoryTypeIndex(PhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
			)
		);

		Device.bindBufferMemory(m_Vertices.Buffer, m_Vertices.Memory, 0);

		Submit(StagingBuffer.Buffer);
		Device.destroyBuffer(StagingBuffer.Buffer);
		Device.freeMemory(StagingBuffer.Memory);
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::VulkanCreate(VertexData* _VertexData, const uint32& _Size)
	{
		Ref<VulkanVertexBuffer> s_VulkanVertexBuffer = CreateRef<VulkanVertexBuffer>(_VertexData, _Size);
		VulkanResourceCache::GetInstance()->Submit<VulkanVertexBuffer>(VulkanResourceType::VertexBuffer, s_VulkanVertexBuffer, s_VulkanVertexBuffer->GetID());
		return s_VulkanVertexBuffer;
	}

}