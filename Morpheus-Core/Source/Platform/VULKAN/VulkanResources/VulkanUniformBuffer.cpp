#include "Morppch.h"
#include "VulkanUniformBuffer.h"

#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

#include "Morpheus/Core/EngineMessageSystem.h"

namespace Morpheus {

	VulkanUniformBuffer::VulkanUniformBuffer(const BufferLayout& _Layout)
		: m_BufferLayout(_Layout)
	{
		SetID(VulkanResourceCache::GetInstance()->GetNextResourceID(VulkanResourceType::UniformBuffer));

		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_Swapchain = VulkanResourceCache::GetInstance()->Get<VulkanSwapchain>(VulkanResourceType::Swapchain);
		//m_DescriptorPool = VulkanResourceCache::GetInstance()->Get<VulkanDescriptorPool>(VulkanResourceType::DescriptorPool);

		CreateUniformBuffer();
		String str = "[VULKAN] UniformBuffer #" + std::to_string(m_ID) + " Was Created!";
		MORP_CORE_WARN(str);
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		MORP_CORE_WARN("[VULKAN] UniformBuffer Was Destoryed!");
	}

	void VulkanUniformBuffer::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.freeMemory(m_Uniform.Memory);
		Device.destroyBuffer(m_Uniform.Buffer);
	}

	void VulkanUniformBuffer::SetLayout(const BufferLayout& _Layout)
	{
		m_BufferLayout = _Layout;
		vk::Device Device = m_Device->GetLogicalDevice();

		uint32 Size = m_BufferLayout.GetStride();
		auto Layout = m_BufferLayout.GetCommons();

		EngineMessageSystem DataMessage;
		for (uint32 i = 0; i < Layout.size(); i++)
		{
			switch (Layout[i].first)
			{
			case UniformDataType::Float:	DataMessage << GetUniformData<float32>(Layout[i].second.Data);
				break;
			case UniformDataType::Float2:	DataMessage << GetUniformData<Vector2>(Layout[i].second.Data);
				break;
			case UniformDataType::Float3:	DataMessage << GetUniformData<Vector2>(Layout[i].second.Data);
				break;
			case UniformDataType::Float4:	DataMessage << GetUniformData<Vector3>(Layout[i].second.Data);
				break;
			case UniformDataType::Mat3:		DataMessage << GetUniformData<Matrix4>(Layout[i].second.Data);
				break;
			case UniformDataType::Mat4:		DataMessage << GetUniformData<Matrix4>(Layout[i].second.Data);
				break;
			case UniformDataType::Int:		DataMessage << GetUniformData<int32>(Layout[i].second.Data);
				break;
			case UniformDataType::Int2:		DataMessage << GetUniformData<Vector2>(Layout[i].second.Data);
				break;
			case UniformDataType::Int3:		DataMessage << GetUniformData<Vector3>(Layout[i].second.Data);
				break;
			case UniformDataType::Int4:		DataMessage << GetUniformData<Vector4>(Layout[i].second.Data);
				break;
			case UniformDataType::Bool:		DataMessage << GetUniformData<bool>(Layout[i].second.Data);

			}
		}

		void* pData = nullptr;
		pData = Device.mapMemory(m_Uniform.Memory, 0, Size);
		memcpy(pData, DataMessage.GetData().data(), Size);
		Device.unmapMemory(m_Uniform.Memory);
	}

	uint32 VulkanUniformBuffer::GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties)
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

	void VulkanUniformBuffer::CreateUniformBuffer()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Viewport Viewport = m_Swapchain->GetViewport();
		uint32 Size = m_BufferLayout.GetStride();
		auto Layout = m_BufferLayout.GetCommons();

		EngineMessageSystem DataMessage;
		for (uint32 i = 0; i < Layout.size(); i++) 
		{
			switch (Layout[i].first)
			{
				case UniformDataType::Float:	DataMessage << GetUniformData<float32>(Layout[i].second.Data);
					break;
				case UniformDataType::Float2:	DataMessage << GetUniformData<Vector2>(Layout[i].second.Data);
					break;
				case UniformDataType::Float3:	DataMessage << GetUniformData<Vector2>(Layout[i].second.Data);
					break;
				case UniformDataType::Float4:	DataMessage << GetUniformData<Vector3>(Layout[i].second.Data);
					break;
				case UniformDataType::Mat3:		DataMessage << GetUniformData<Matrix4>(Layout[i].second.Data);
					break;
				case UniformDataType::Mat4:		DataMessage << GetUniformData<Matrix4>(Layout[i].second.Data);
					break;
				case UniformDataType::Int:		DataMessage << GetUniformData<int32>(Layout[i].second.Data);
					break;
				case UniformDataType::Int2:		DataMessage << GetUniformData<Vector2>(Layout[i].second.Data);
					break;
				case UniformDataType::Int3:		DataMessage << GetUniformData<Vector3>(Layout[i].second.Data);
					break;
				case UniformDataType::Int4:		DataMessage << GetUniformData<Vector4>(Layout[i].second.Data);
					break;
				case UniformDataType::Bool:		DataMessage << GetUniformData<bool>(Layout[i].second.Data);

			}
		}

		vk::BufferCreateInfo CreateInfo = {};
		{
			CreateInfo.flags = vk::BufferCreateFlags();
			CreateInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
			CreateInfo.size = Size;
		}
		m_Uniform.Buffer = Device.createBuffer(CreateInfo);

		auto memReqs = Device.getBufferMemoryRequirements(m_Uniform.Buffer);
		vk::MemoryAllocateInfo AllocInfo = {};
		{
			AllocInfo.pNext = nullptr;
			AllocInfo.allocationSize = memReqs.size;
			AllocInfo.memoryTypeIndex = GetMemoryTypeIndex(PhysicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		}

		m_Uniform.Memory = Device.allocateMemory(AllocInfo);
		Device.bindBufferMemory(m_Uniform.Buffer, m_Uniform.Memory, 0);

		m_Uniform.Descriptor.buffer = m_Uniform.Buffer;
		m_Uniform.Descriptor.offset = 0;
		m_Uniform.Descriptor.range = Size;

		// Memory Copying
		void* pData = nullptr;
		pData = Device.mapMemory(m_Uniform.Memory, 0, Size);
		memcpy(pData, DataMessage.GetData().data(), Size);
		Device.unmapMemory(m_Uniform.Memory);
	}

	Ref<VulkanUniformBuffer> VulkanUniformBuffer::VulkanCreate(const BufferLayout& _Layout)
	{
		Ref<VulkanUniformBuffer> s_VulkanUniformBuffer = CreateRef<VulkanUniformBuffer>(_Layout);
		VulkanResourceCache::GetInstance()->Submit<VulkanUniformBuffer>(VulkanResourceType::UniformBuffer, s_VulkanUniformBuffer, s_VulkanUniformBuffer->GetID());
		return s_VulkanUniformBuffer;
	}

}

