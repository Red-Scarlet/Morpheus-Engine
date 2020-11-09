#include "Morppch.h"
#include "VulkanDescriptorPool.h"
#include "VulkanUniformBuffer.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"
#include "Morpheus/Renderer/RendererCore/RenderCommand.h"

namespace Morpheus {

	VulkanDescriptorPool::VulkanDescriptorPool(const uint32& _NumObjects)
		: VulkanResource(VulkanResourceTypes::VulkanDescriptor), m_CurrentSize(_NumObjects)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		SetID(VulkanMemoryManager::GetInstance()->GetResourceCache()->GetNextResourceID(VulkanResourceTypes::VulkanDescriptor));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] DescriptorPool #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		VulkanDestory();
		MORP_CORE_WARN("[VULKAN] DescriptorPool Was Destoryed!");
	}

	void VulkanDescriptorPool::VulkanCreate()
	{
		CreateDescriptorLayout();
		CreateDescriptorPool();
		CreateDescriptorSet();
	}

	void VulkanDescriptorPool::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Device.freeDescriptorSets(m_DescriptorPool, m_DescriptorSets);
		Device.destroyDescriptorPool(m_DescriptorPool);
		Device.destroyDescriptorSetLayout(m_DescriptorSetLayout);
	}

	void VulkanDescriptorPool::CreateDescriptorLayout()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		vk::DescriptorSetLayoutBinding DescriptorSetLayoutBinding = {};
		{
			DescriptorSetLayoutBinding.binding = 0;
			DescriptorSetLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
			DescriptorSetLayoutBinding.descriptorCount = 1;
			DescriptorSetLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
			DescriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		}

		vk::DescriptorSetLayoutCreateInfo LayoutSetCreateInfo = {};
		{
			LayoutSetCreateInfo.flags = vk::DescriptorSetLayoutCreateFlags();
			LayoutSetCreateInfo.bindingCount = 1;
			LayoutSetCreateInfo.pBindings = &DescriptorSetLayoutBinding;
		}
		m_DescriptorSetLayout = Device.createDescriptorSetLayout(LayoutSetCreateInfo);
	}

	void VulkanDescriptorPool::CreateDescriptorPool()
	{
		if(m_CurrentSize >= 1) {
			vk::Device Device = m_Device->GetLogicalDevice();
			vk::DescriptorPoolSize PoolSize = {};
			{
				PoolSize.type = vk::DescriptorType::eUniformBuffer;
				PoolSize.descriptorCount = m_CurrentSize;
			}
			Vector<vk::DescriptorPoolSize> DescriptorPoolSizes = { PoolSize, PoolSize };

			vk::DescriptorPoolCreateInfo PoolCreateInfo{};
			{
				//PoolCreateInfo.flags = ;
				PoolCreateInfo.flags = vk::DescriptorPoolCreateFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
				PoolCreateInfo.poolSizeCount = (uint32)DescriptorPoolSizes.size();
				PoolCreateInfo.pPoolSizes = DescriptorPoolSizes.data();
				PoolCreateInfo.maxSets = m_CurrentSize;
			}
			m_DescriptorPool = Device.createDescriptorPool(PoolCreateInfo);
			MORP_CORE_ERROR("[VULKAN] DescriptorPool of " + std::to_string(m_CurrentSize) + " was Created!");
		}
	}

	void VulkanDescriptorPool::CreateDescriptorSet()
	{
		if (m_CurrentSize >= 1) {

			vk::Device Device = m_Device->GetLogicalDevice();

			m_DescriptorSets.resize(m_CurrentSize);

			for (uint32 i = 0; i < m_CurrentSize; i++)
			{
				VkDescriptorSetAllocateInfo AllocateInfo{};
				{
					VkDescriptorSetLayout Layout = VkDescriptorSetLayout(m_DescriptorSetLayout);

					AllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
					AllocateInfo.descriptorPool = m_DescriptorPool;
					AllocateInfo.descriptorSetCount = 1;
					AllocateInfo.pSetLayouts = &Layout;

					m_DescriptorSetLayout = vk::DescriptorSetLayout(Layout);
				}

				VkDescriptorSet DescriptorSet = VkDescriptorSet(m_DescriptorSets[i]);
				VkResult result = vkAllocateDescriptorSets(Device, &AllocateInfo, &DescriptorSet);

				MORP_CORE_ASSERT(result, "Descriptor FAIL!");
				m_DescriptorSets[i] = vk::DescriptorSet(DescriptorSet);
				MORP_CORE_ERROR("[VULKAN] DescriptorSet of #" + std::to_string(i) + " and Size of " + std::to_string(m_CurrentSize) + " was Allocated!");

			}


			// Set Compilation State!
			RenderCommand::SetCompile();
		}
	}

	void VulkanDescriptorPool::DeleteDescriptorSet()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Vector<VkDescriptorSet> DiscriptorSets;
		for (uint32 i = 0; i < m_DescriptorSets.size(); i++)
		{
			VkDescriptorSet DescriptorSet = VkDescriptorSet(m_DescriptorSets[i]);
			DiscriptorSets.push_back(DescriptorSet);
		}

		VkResult result = vkFreeDescriptorSets(Device, m_DescriptorPool, 1, DiscriptorSets.data());
		MORP_CORE_ASSERT(result, "Failed to create Descriptor set!");
	}

	void VulkanDescriptorPool::UpdateDescriptorSet(const uint32& _ID)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Ref<VulkanUniformBuffer> UBO = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanUniformBuffer>(VulkanResourceTypes::VulkanUniformBuffer, _ID);

		if(!UBO->GetCompiled()) {
			const vk::DescriptorBufferInfo& Buffer = UBO->GetDescriptorBuffer();
			Vector<vk::WriteDescriptorSet> descriptorWrites = 
			{ vk::WriteDescriptorSet(m_DescriptorSets[_ID], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &Buffer, nullptr ) };
			Device.updateDescriptorSets(descriptorWrites, nullptr);
			UBO->SetCompiled(true);
			MORP_CORE_ERROR("[VULKAN] Update to DescriptorWrite of UBO #" + std::to_string(_ID) + " was made!");
		}
	}

	void VulkanDescriptorPool::FreeDescriptor()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		DeleteDescriptorSet();

		//Device.freeDescriptorSets(m_DescriptorPool, m_DescriptorSets);
		Device.destroyDescriptorPool(m_DescriptorPool);
	}

	Ref<VulkanDescriptorPool> VulkanDescriptorPool::Make(const uint32& _NumObjects)
	{
		Ref<VulkanDescriptorPool> s_VulkanDescriptorPool = CreateRef<VulkanDescriptorPool>(_NumObjects);
		VulkanMemoryManager::GetInstance()->GetResourceCache()->Submit(s_VulkanDescriptorPool);
		return s_VulkanDescriptorPool;
	}

}
