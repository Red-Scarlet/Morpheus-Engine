#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	VulkanVertexBuffer::VulkanVertexBuffer(const AnyData& _Data)
	{
		m_Device = VulkanInstance::GetInstance()->GetDevice(0);
		m_Data = AnyCast<RendererBufferData>(_Data);

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] VertexBuffer Was Created!");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VULKAN_CORE_WARN("[VULKAN] VertexBuffer Was Destoryed!");
	}

	void VulkanVertexBuffer::VulkanCreate()
	{
		MORP_PROFILE_FUNCTION();

		// Begining of Staging

		VkBuffer StagingBuffer = {};
		VkDeviceMemory StagingMemory = {};

		VkBufferCreateInfo StagingCreateInfo = {};
		StagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		StagingCreateInfo.size = m_Data.Size;
		StagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		StagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkResult result = vkCreateBuffer(m_Device->GetLogical(), &StagingCreateInfo, nullptr, &StagingBuffer);
		VULKAN_CORE_ASSERT(result, "[VULKAN] VertexBuffer Staging CreateBuffer Failure!");

		VkMemoryRequirements StagingMemoryRequirements = {};
		vkGetBufferMemoryRequirements(m_Device->GetLogical(), StagingBuffer, &StagingMemoryRequirements);
		uint32 StagingMemoryIndex = m_Device->FindMemoryType(StagingMemoryRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkMemoryAllocateInfo StagingAllocInfo = {};
		StagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		StagingAllocInfo.allocationSize = StagingMemoryRequirements.size;
		StagingAllocInfo.memoryTypeIndex = StagingMemoryIndex;
		result = vkAllocateMemory(m_Device->GetLogical(), &StagingAllocInfo, nullptr, &StagingMemory);
		VULKAN_CORE_ASSERT(result, "[VULKAN] VertexBuffer Staging AllocateMemory Failure!");
	
		result = vkBindBufferMemory(m_Device->GetLogical(), StagingBuffer, StagingMemory, 0);
		VULKAN_CORE_ASSERT(result, "[VULKAN] VertexBuffer Staging BindBufferMemory Failure!");

		void* pData = nullptr;
		vkMapMemory(m_Device->GetLogical(), StagingMemory, 0, m_Data.Size, 0, &pData);
		memcpy(pData, m_Data.Data, m_Data.Size);
		vkUnmapMemory(m_Device->GetLogical(), StagingMemory);

		VkBufferCreateInfo BufferCreateInfo = {};
		BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		BufferCreateInfo.size = m_Data.Size;
		BufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		result = vkCreateBuffer(m_Device->GetLogical(), &BufferCreateInfo, nullptr, &m_Buffer);
		VULKAN_CORE_ASSERT(result, "[VULKAN] VertexBuffer CreateBuffer Failure!");

		VkMemoryRequirements BufferMemoryRequirements = {};
		vkGetBufferMemoryRequirements(m_Device->GetLogical(), m_Buffer, &BufferMemoryRequirements);
		uint32 BufferMemoryIndex = m_Device->FindMemoryType(BufferMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkMemoryAllocateInfo BufferAllocInfo = {};
		BufferAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		BufferAllocInfo.allocationSize = BufferMemoryRequirements.size;
		BufferAllocInfo.memoryTypeIndex = BufferMemoryIndex;
		result = vkAllocateMemory(m_Device->GetLogical(), &BufferAllocInfo, nullptr, &m_Memory);
		VULKAN_CORE_ASSERT(result, "[VULKAN] VertexBuffer AllocateMemory Failure!");
		vkBindBufferMemory(m_Device->GetLogical(), m_Buffer, m_Memory, 0);

		Submit(StagingBuffer);
		
		vkDestroyBuffer(m_Device->GetLogical(), StagingBuffer, nullptr);
		vkFreeMemory(m_Device->GetLogical(), StagingMemory, nullptr);
	}

	void VulkanVertexBuffer::VulkanDestory()
	{
		vkDestroyBuffer(m_Device->GetLogical(), m_Buffer, nullptr);
		vkFreeMemory(m_Device->GetLogical(), m_Memory, nullptr);
	}

	void VulkanVertexBuffer::Submit(const VkBuffer& _Buffer)
	{
		MORP_PROFILE_FUNCTION();
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::Create(const AnyData& _Data)
	{
		return CreateRef<VulkanVertexBuffer>(_Data);
	}

	void VulkanVertexBuffer::Destroy(const Ref<VulkanVertexBuffer>& _VertexBuffer)
	{
		_VertexBuffer->VulkanDestory();
	}

}}