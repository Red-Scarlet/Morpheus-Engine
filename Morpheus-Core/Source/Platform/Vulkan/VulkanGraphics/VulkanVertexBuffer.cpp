#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	static const uint32& FindMemoryType(const uint32& _TypeFilter, const VkMemoryPropertyFlags& _Properties) 
	{
		auto Instance = VulkanInstance::GetInstance();

		VkPhysicalDeviceMemoryProperties MemProperties;
		vkGetPhysicalDeviceMemoryProperties(Instance->GetPhysicalDevice()->GetDevice(), &MemProperties);

		for (uint32 i = 0; i < MemProperties.memoryTypeCount; i++) {
			if ((_TypeFilter & (1 << i)) && (MemProperties.memoryTypes[i].propertyFlags & _Properties) == _Properties) {
				return i;
			}
		}

		MORP_CORE_ASSERT(true, "[VULKAN] Failed to find suitable memory type!");
	}

	VulkanVertexBuffer::VulkanVertexBuffer(const Vector<Vertex>& _Vertices)
		
	{
		m_VulkanObject.Vertices = _Vertices;
		CreateVertexBuffer();
		MORP_CORE_WARN("[VULKAN] VertexBuffer Was Created!");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		auto Instance = VulkanInstance::GetInstance();
		vkDestroyBuffer(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Buffer, nullptr);
		vkFreeMemory(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Memory, nullptr);
	}

	void VulkanVertexBuffer::CreateVertexBuffer()
	{
		auto Instance = VulkanInstance::GetInstance();

		VkBufferCreateInfo BufferInfo {};
		{
			BufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferInfo.size = sizeof(m_VulkanObject.Vertices[0]) * m_VulkanObject.Vertices.size();
			BufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			BufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		VkResult result = vkCreateBuffer(Instance->GetLogicalDevice()->GetDevice(), &BufferInfo, nullptr, &m_VulkanObject.Buffer);
		MORP_CORE_ASSERT(result, "Failed to create VertexBuffer!");

		VkMemoryRequirements MemRequirements;
		vkGetBufferMemoryRequirements(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Buffer, &MemRequirements);
		
		VkMemoryAllocateInfo AllocInfo {};
		{
			AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			AllocInfo.allocationSize = MemRequirements.size;
			AllocInfo.memoryTypeIndex = FindMemoryType(MemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}

		VkResult result_2 = vkAllocateMemory(Instance->GetLogicalDevice()->GetDevice(), &AllocInfo, nullptr, &m_VulkanObject.Memory);
		MORP_CORE_ASSERT(result, "Failed to Allocate memory to the VertexBuffer!");
		
		vkBindBufferMemory(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Buffer, m_VulkanObject.Memory, 0);

		void* data;
		vkMapMemory(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Memory, 0, BufferInfo.size, 0, &data);
		memcpy(data, m_VulkanObject.Vertices.data(), (size_t)BufferInfo.size);
		vkUnmapMemory(Instance->GetLogicalDevice()->GetDevice(), m_VulkanObject.Memory);

	}	

}