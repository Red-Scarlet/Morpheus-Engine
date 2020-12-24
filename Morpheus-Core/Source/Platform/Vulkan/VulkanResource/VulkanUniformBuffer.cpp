#include "Morppch.h"
#include "VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	VulkanUniformBuffer::VulkanUniformBuffer(const UniformAttributeLayout& _Layout, const Ref<UniformBufferCache>& _Cache)
		: m_Layout(_Layout), m_Cache(_Cache)
	{
		MORP_PROFILE_FUNCTION();

		m_Device = VulkanInstance::GetInstance()->GetLogicalDevice();
		m_ID = m_Cache->GetCount();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] UniformBuffer #" + ToString(GetID()) + " Was Created!");
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] UniformBuffer Was Destoryed!");
	}

	void VulkanUniformBuffer::SetInt(const String& _Name, const uint32& _Value)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanUniformBuffer::SetFloat(const String& _Name, const float32& _Value)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanUniformBuffer::SetFloat3(const String& _Name, const Vector3& _Value)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanUniformBuffer::SetFloat4(const String& _Name, const Vector4& _Value)
	{
		m_UniformCache[_Name] = _Value;
	}

	void VulkanUniformBuffer::SetMat4(const String& _Name, const Matrix4& _Matrix)
	{
		m_UniformCache[_Name] = _Matrix;
	}

	void VulkanUniformBuffer::UpdateBufferData(VulkanUniformMessage& _Message)
	{
		uint32 Size = m_Layout.GetStride();

		void* pData = nullptr;
		vkMapMemory(m_Device->GetDevice(), m_Memory, 0, Size, 0, &pData);
		memcpy(pData, _Message.GetData(), Size);
		vkUnmapMemory(m_Device->GetDevice(), m_Memory);
	}

	VkWriteDescriptorSet VulkanUniformBuffer::UpdateDescriptorSet(const VkDescriptorSet& _DescriptorSet)
	{
		m_Compiled = true;

		VkWriteDescriptorSet DescriptorWrite = {};
		DescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		DescriptorWrite.dstSet = _DescriptorSet;
		DescriptorWrite.dstBinding = 0;
		DescriptorWrite.dstArrayElement = 0;
		DescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		DescriptorWrite.descriptorCount = 1;
		DescriptorWrite.pBufferInfo = &m_BufferInfo;

		return DescriptorWrite;
	}

	void VulkanUniformBuffer::UpdateUniformBuffer()
	{
		// HOTTEST CODE @ 3-12-20
		if (m_UniformCache.empty())
			return;

		uint32 Size = m_Layout.GetStride();
		VulkanUniformMessage DataMessage = {};
		Vector<UniformAttributeElement> Elements = m_Layout.GetElements();
		for (const auto& Element : Elements) {
			auto it = m_UniformCache.find(Element.Name);
			if (it != m_UniformCache.end()) {
				switch (Element.Type) {
					case UniformAttributeType::Float:		DataMessage << AnyCast<float32>(it->second);
						break;
					case UniformAttributeType::Float2:		DataMessage << AnyCast<Vector2>(it->second);
						break;																
					case UniformAttributeType::Float3:		DataMessage << AnyCast<Vector2>(it->second);
						break;																
					case UniformAttributeType::Float4:		DataMessage << AnyCast<Vector3>(it->second);
						break;																
					case UniformAttributeType::Mat3:		DataMessage << AnyCast<Matrix4>(it->second);
						break;																
					case UniformAttributeType::Mat4:		DataMessage << AnyCast<Matrix4>(it->second);
						break;
					case UniformAttributeType::Int:			DataMessage << AnyCast<int32>(it->second);
						break;
					case UniformAttributeType::Int2:		DataMessage << AnyCast<Vector2>(it->second);
						break;																
					case UniformAttributeType::Int3:		DataMessage << AnyCast<Vector3>(it->second);
						break;																
					case UniformAttributeType::Int4:		DataMessage << AnyCast<Vector4>(it->second);
						break;
					case UniformAttributeType::Bool:		DataMessage << AnyCast<bool>(it->second);
						break;
				}
			}
		}
		
		UpdateBufferData(DataMessage);
	}

	void VulkanUniformBuffer::VulkanCreate()
	{
		uint32 Size = m_Layout.GetStride();

		VkBufferCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		CreateInfo.flags = VkBufferCreateFlags();
		CreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		CreateInfo.size = Size;
		vkCreateBuffer(m_Device->GetDevice(), &CreateInfo, nullptr, &m_Buffer);
		
		VkMemoryRequirements MemoryRequirements = {};
		vkGetBufferMemoryRequirements(m_Device->GetDevice(), m_Buffer, &MemoryRequirements);
		uint32 MemoryIndex = m_Device->FindMemoryType(MemoryRequirements.memoryTypeBits, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		VkMemoryAllocateInfo AllocInfo = {};
		AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		AllocInfo.allocationSize = MemoryRequirements.size;
		AllocInfo.memoryTypeIndex = MemoryIndex;
		vkAllocateMemory(m_Device->GetDevice(), &AllocInfo, nullptr, &m_Memory);
		vkBindBufferMemory(m_Device->GetDevice(), m_Buffer, m_Memory, 0);

		VulkanUniformMessage DataMessage;
		Vector<UniformAttributeElement> Layout = m_Layout.GetElements();
		for (uint32 i = 0; i < Layout.size(); i++)
			switch (Layout[i].Type)
			{
				case UniformAttributeType::Float:		DataMessage << float32(1.00f);							break;
				case UniformAttributeType::Float2:		DataMessage << Vector2(1.00f, 1.00f);					break;
				case UniformAttributeType::Float3:		DataMessage << Vector3(1.00f, 1.00f, 1.00f);			break;
				case UniformAttributeType::Float4:		DataMessage << Vector4(1.00f, 1.00f, 1.00f, 1.00f);		break;
				case UniformAttributeType::Mat3:		DataMessage << Matrix4(1.00f);							break;
				case UniformAttributeType::Mat4:		DataMessage << Matrix4(1.00f);							break;
				case UniformAttributeType::Int:			DataMessage << Vector2(1.00f, 1.00f);					break;
				case UniformAttributeType::Int2:		DataMessage << Vector2(1.00f, 1.00f);					break;
				case UniformAttributeType::Int3:		DataMessage << Vector3(1.00f, 1.00f, 1.00f);			break;
				case UniformAttributeType::Int4:		DataMessage << Vector4(1.00f, 1.00f, 1.00f, 1.00f);		break;
				case UniformAttributeType::Bool:		DataMessage << Boolean(true);							break;
			}
		
		m_BufferInfo.buffer = m_Buffer;
		m_BufferInfo.offset = 0;
		m_BufferInfo.range = m_Layout.GetStride();

		UpdateBufferData(DataMessage);
	}

	void VulkanUniformBuffer::VulkanDestory()
	{
		vkDestroyBuffer(m_Device->GetDevice(), m_Buffer, nullptr);
		vkFreeMemory(m_Device->GetDevice(), m_Memory, nullptr);
	}

	Ref<VulkanUniformBuffer> VulkanUniformBuffer::Make(const UniformAttributeLayout& _Layout)
	{
		Ref<UniformBufferCache> s_Cache = VulkanCache<VulkanUniformBuffer>::Get(VULKAN_UNIFORM_BUFFER_CACHE_TYPE);
		Ref<VulkanUniformBuffer> s_VulkanUniformBuffer = CreateRef<VulkanUniformBuffer>(_Layout, s_Cache);
		s_Cache->Add(s_VulkanUniformBuffer->GetID(), s_VulkanUniformBuffer);
		return s_VulkanUniformBuffer;
	}

}