#include "Morppch.h"
#include "VulkanTextureBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanTextureBuffer::VulkanTextureBuffer()
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_Swapchain = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanSwapchain>(VulkanGlobalTypes::VulkanSwapchain);
		//SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanTextureBuffer));

		uint32 m_ID = 0;
		MORP_CORE_WARN("[VULKAN] TextureBuffer #" + std::to_string(m_ID) + " Was Created!");
	}

	VulkanTextureBuffer::~VulkanTextureBuffer()
	{
	}


	Ref<VulkanTextureBuffer> VulkanTextureBuffer::Make()
	{
		Ref<VulkanTextureBuffer> s_VulkanTextureBuffer = CreateRef<VulkanTextureBuffer>();
		//VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit(s_VulkanTextureBuffer);
		return s_VulkanTextureBuffer;
	}

}