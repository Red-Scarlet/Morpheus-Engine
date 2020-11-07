#include "Morppch.h"
#include "VulkanTextureBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanTextureBuffer::VulkanTextureBuffer()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanTextureBuffer));

		CreateTextureBuffer();
		String str = "[VULKAN] TextureBuffer #" + std::to_string(m_ID) + " Was Created!";
		MORP_CORE_WARN(str);
	}

	VulkanTextureBuffer::~VulkanTextureBuffer()
	{
	}

	void VulkanTextureBuffer::Destory()
	{
	}

	uint32 VulkanTextureBuffer::GetMemoryTypeIndex(vk::PhysicalDevice& _PhysicalDevice, uint32 _TypeBits, vk::MemoryPropertyFlags _Properties)
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

	void VulkanTextureBuffer::CreateTextureBuffer()
	{
	}

	Ref<VulkanTextureBuffer> VulkanTextureBuffer::VulkanCreate()
	{
		Ref<VulkanTextureBuffer> s_VulkanTextureBuffer = CreateRef<VulkanTextureBuffer>();
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit<VulkanTextureBuffer>(VulkanResourceTypes::VulkanTextureBuffer, s_VulkanTextureBuffer);
		return s_VulkanTextureBuffer;
	}

}