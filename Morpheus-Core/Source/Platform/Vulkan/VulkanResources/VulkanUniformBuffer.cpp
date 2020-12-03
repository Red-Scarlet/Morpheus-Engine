#include "Morppch.h"
#include "VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanUniformBuffer::VulkanUniformBuffer(const ShaderAttributeLayout& _Layout, const vk::DescriptorSet& _Set)
		: m_Layout(_Layout), m_DescriptorSet(_Set)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();
		m_ID = VulkanMemoryManager::GetInstance()->GetUniformBufferCache().Count();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] UniformBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] UniformBuffer Was Destoryed!");
	}

	void VulkanUniformBuffer::UpdateBufferData(const VulkanUniformMessage& _Message)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 Size = m_Layout.GetStride();

		void* pData = Device.mapMemory(m_Buffer.Memory, 0, Size);
		std::memcpy(pData, _Message.Data.data(), Size);
		Device.unmapMemory(m_Buffer.Memory);
	}

	vk::WriteDescriptorSet VulkanUniformBuffer::UpdateDescriptorSet()
	{
		const vk::DescriptorBufferInfo& WriteBufferInfo = m_BufferInfo;
		vk::WriteDescriptorSet DescriptorWrite = vk::WriteDescriptorSet(m_DescriptorSet, 0, 0, 1, 
			vk::DescriptorType::eUniformBuffer, nullptr, &WriteBufferInfo, nullptr);
		return DescriptorWrite;
	}

	void VulkanUniformBuffer::VulkanCreate()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		uint32 Size = m_Layout.GetStride();

		vk::DescriptorBufferInfo UniformDescriptor;
		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
			CreateInfo.size = Size;
		}

		m_Buffer.Buffer = Device.createBuffer(CreateInfo);

		auto memReqs = Device.getBufferMemoryRequirements(m_Buffer.Buffer);
		vk::MemoryAllocateInfo AllocInfo = {};
		{
			AllocInfo.pNext = nullptr;
			AllocInfo.allocationSize = memReqs.size;
			AllocInfo.memoryTypeIndex = m_Device->GetMemoryTypeIndex(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		}

		m_BufferInfo.buffer = m_Buffer.Buffer;
		m_BufferInfo.offset = 0;
		m_BufferInfo.range = m_Layout.GetStride();

		m_Buffer.Memory = Device.allocateMemory(AllocInfo);
		Device.bindBufferMemory(m_Buffer.Buffer, m_Buffer.Memory, 0);

		VulkanUniformMessage DataMessage;
		auto Layout = m_Layout.GetElements();
		for (uint32 i = 0; i < Layout.size(); i++) {
			switch (Layout[i].Type)
			{
			case ShaderAttributeType::Float:	DataMessage << float32(1.00f);
				break;
			case ShaderAttributeType::Float2:	DataMessage << Vector2(1.00f, 1.00f);
				break;
			case ShaderAttributeType::Float3:	DataMessage << Vector3(1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Float4:	DataMessage << Vector4(1.00f, 1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Mat3:		DataMessage << Matrix4(1.00f);
				break;
			case ShaderAttributeType::Mat4:		DataMessage << Matrix4(1.00f);
				break;
			case ShaderAttributeType::Int:		DataMessage << Vector2(1.00f, 1.00f);
				break;
			case ShaderAttributeType::Int2:		DataMessage << Vector2(1.00f, 1.00f);
				break;
			case ShaderAttributeType::Int3:		DataMessage << Vector3(1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Int4:		DataMessage << Vector4(1.00f, 1.00f, 1.00f, 1.00f);
				break;
			case ShaderAttributeType::Bool:		DataMessage << Boolean(true);
				break;
			}
		}

		void* pData = Device.mapMemory(m_Buffer.Memory, 0, Size);
		std::memcpy(pData, DataMessage.Data.data(), Size);
		Device.unmapMemory(m_Buffer.Memory);
	}

	void VulkanUniformBuffer::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Device.freeMemory(m_Buffer.Memory);
		Device.destroyBuffer(m_Buffer.Buffer);
	}

	Ref<VulkanUniformBuffer> VulkanUniformBuffer::Make(const ShaderAttributeLayout& _Layout, const vk::DescriptorSet& _Set)
	{
		Ref<VulkanUniformBuffer> s_VulkanUniformBuffer = CreateRef<VulkanUniformBuffer>(_Layout, _Set);
		VulkanMemoryManager::GetInstance()->GetUniformBufferCache().Add(s_VulkanUniformBuffer->GetID(), s_VulkanUniformBuffer);
		return s_VulkanUniformBuffer;
	}

}