#include "Morppch.h"
#include "VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanUniformBuffer::VulkanUniformBuffer(const BufferLayout& _Layout)
		: VulkanResource(VulkanResourceTypes::VulkanUniformBuffer), m_BufferLayout(_Layout)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanUniformBuffer));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] UniformBuffer #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] UniformBuffer Was Destoryed!");
	}

	void VulkanUniformBuffer::VulkanCreate()
	{
		vk::PhysicalDevice PhysicalDevice = m_Device->GetPhysicalDevice();
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::Viewport Viewport = m_Swapchain->GetViewport();
		uint32 Size = m_BufferLayout.GetStride();
		auto Layout = m_BufferLayout.GetCommons();

		VulkanUniformMessage DataMessage;
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
			AllocInfo.memoryTypeIndex = m_Device->GetMemoryTypeIndex(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		}

		m_Uniform.Memory = Device.allocateMemory(AllocInfo);
		Device.bindBufferMemory(m_Uniform.Buffer, m_Uniform.Memory, 0);

		m_Uniform.Descriptor.buffer = m_Uniform.Buffer;
		m_Uniform.Descriptor.offset = 0;
		m_Uniform.Descriptor.range = Size;

		// Memory Copying
		void* pData = nullptr;
		pData = Device.mapMemory(m_Uniform.Memory, 0, Size);
		std::memcpy(pData, DataMessage.Body.data(), Size);
		Device.unmapMemory(m_Uniform.Memory);
	}

	void VulkanUniformBuffer::VulkanDestory()
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

		VulkanUniformMessage DataMessage;
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

		void* pData = Device.mapMemory(m_Uniform.Memory, 0, Size);
		std::memcpy(pData, DataMessage.Body.data(), Size);
		Device.unmapMemory(m_Uniform.Memory);
	}

	Ref<VulkanUniformBuffer> VulkanUniformBuffer::Make(const BufferLayout& _Layout)
	{
		Ref<VulkanUniformBuffer> s_VulkanUniformBuffer = CreateRef<VulkanUniformBuffer>(_Layout);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit(s_VulkanUniformBuffer);
		return s_VulkanUniformBuffer;
	}

}

