#include "Morppch.h"
#include "VulkanDescriptorPool.h"

#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

#include "VulkanUniformBuffer.h"

namespace Morpheus {

	VulkanDescriptorPool::VulkanDescriptorPool(const uint32& _NumObjects)
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);

		MORP_CORE_WARN("[VULKAN] DescriptorPool Was Created!");

		// SHADER WILL TELL THE DESCRIPTOR LAYOUT HOW MANY BUFFER CAN BE ATTACHED!
		CreateDescriptorLayout();

		// SHADER WILL TELL THE DESCRIPTOR POOL HOW MANY OBJECT CAN BE CREATED!
		CreateDescriptorPool(_NumObjects);
		
		// SHADER WILL TELL THE DESCRIPTOR SETS HOW MANY OBJECT CAN BE CREATED!
		CreateDescriptorSet(_NumObjects);
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		MORP_CORE_WARN("[VULKAN] DescriptorPool Was Destoryed!");
	}

	void VulkanDescriptorPool::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
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

	void VulkanDescriptorPool::CreateDescriptorPool(const uint32& _Size)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		vk::DescriptorPoolSize PoolSize = {};
		{
			PoolSize.type = vk::DescriptorType::eUniformBuffer;
			PoolSize.descriptorCount = _Size;
		}
		Vector<vk::DescriptorPoolSize> DescriptorPoolSizes = { PoolSize, PoolSize };

		vk::DescriptorPoolCreateInfo PoolCreateInfo{};
		{
			PoolCreateInfo.flags = vk::DescriptorPoolCreateFlags();
			PoolCreateInfo.poolSizeCount = (uint32)DescriptorPoolSizes.size();
			PoolCreateInfo.pPoolSizes = DescriptorPoolSizes.data();
			PoolCreateInfo.maxSets = _Size;
		}
		m_DescriptorPool = Device.createDescriptorPool(PoolCreateInfo);
	}

	void VulkanDescriptorPool::CreateDescriptorSet(const uint32& _Size)
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		m_DescriptorSets.resize(_Size);

		for (uint32 i = 0; i < _Size; i++)
		{
			VkDescriptorSetAllocateInfo AllocateInfo {};
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

			MORP_CORE_ASSERT(result, "Failed to create Window Surface!");
			m_DescriptorSets[i] = vk::DescriptorSet(DescriptorSet);

		}
	}

	void VulkanDescriptorPool::UpdateDescriptorSet(const uint32& _ID)
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Ref<VulkanUniformBuffer> UBO = VulkanResourceCache::GetInstance()->Get<VulkanUniformBuffer>(VulkanResourceType::UniformBuffer, _ID);
		const vk::DescriptorBufferInfo& Buffer = UBO->GetDescriptorBuffer();

		Vector<vk::WriteDescriptorSet> descriptorWrites = 
		{ vk::WriteDescriptorSet(m_DescriptorSets[_ID], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &Buffer, nullptr ) };

		Device.updateDescriptorSets(descriptorWrites, nullptr);
	}

	Ref<VulkanDescriptorPool> VulkanDescriptorPool::VulkanCreate(const uint32& _NumObjects)
	{
		Ref<VulkanDescriptorPool> s_VulkanDescriptorPool = CreateRef<VulkanDescriptorPool>(_NumObjects);
		VulkanResourceCache::GetInstance()->Submit<VulkanDescriptorPool>(VulkanResourceType::DescriptorPool, s_VulkanDescriptorPool, s_VulkanDescriptorPool->GetID());
		return s_VulkanDescriptorPool;
	}

}
