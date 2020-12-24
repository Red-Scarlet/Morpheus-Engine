#include "Morppch.h"
#include "VulkanDescriptor.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus {

	VulkanDescriptor::VulkanDescriptor(const uint32& _MaxDescriptorSize)
		: m_DescriptorMaxSize(_MaxDescriptorSize)
	{
		MORP_PROFILE_FUNCTION();

		m_Device = VulkanInstance::GetInstance()->GetLogicalDevice();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Descriptor Was Created!");
	}

	VulkanDescriptor::~VulkanDescriptor()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] Descriptor Was Destoryed!");
	}

	const VkDescriptorSet& VulkanDescriptor::GetDescriptorSet(const uint32& _Index)
	{
		if (_Index >= m_DescriptorSize)
			Allocate();
		return m_DescriptorSets[_Index];
	}

	void VulkanDescriptor::VulkanCreate()
	{
		VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding_Uniform = {};
		DescriptorSetLayoutBinding_Uniform.binding = 0;
		DescriptorSetLayoutBinding_Uniform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		DescriptorSetLayoutBinding_Uniform.descriptorCount = 1;
		DescriptorSetLayoutBinding_Uniform.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		DescriptorSetLayoutBinding_Uniform.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding_Sampler = {};
		DescriptorSetLayoutBinding_Sampler.binding = 1;
		DescriptorSetLayoutBinding_Sampler.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		DescriptorSetLayoutBinding_Sampler.descriptorCount = 1;
		DescriptorSetLayoutBinding_Sampler.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		DescriptorSetLayoutBinding_Sampler.pImmutableSamplers = nullptr;

		Vector<VkDescriptorSetLayoutBinding> Bindings = { DescriptorSetLayoutBinding_Uniform, DescriptorSetLayoutBinding_Sampler };
		VkDescriptorSetLayoutCreateInfo LayoutSetCreateInfo = {};
		LayoutSetCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		LayoutSetCreateInfo.flags = VkDescriptorSetLayoutCreateFlags();
		LayoutSetCreateInfo.bindingCount = Bindings.size();
		LayoutSetCreateInfo.pBindings = Bindings.data();

		vkCreateDescriptorSetLayout(m_Device->GetDevice(), &LayoutSetCreateInfo, nullptr, &m_DescriptorLayout);
		Expand();
	}

	void VulkanDescriptor::VulkanDestory()
	{
		vkDestroyDescriptorSetLayout(m_Device->GetDevice(), m_DescriptorLayout, nullptr);
	}

	void VulkanDescriptor::Expand()
	{
		MORP_PROFILE_FUNCTION();

		Vector<VkDescriptorPoolSize> DescriptorPoolSizes;

		VkDescriptorPoolSize PoolSize_Uniform = {};
		PoolSize_Uniform.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		PoolSize_Uniform.descriptorCount = m_DescriptorMaxSize;
		DescriptorPoolSizes.push_back(PoolSize_Uniform);

		VkDescriptorPoolSize PoolSize_Sampler = {};
		PoolSize_Sampler.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		PoolSize_Sampler.descriptorCount = m_DescriptorMaxSize;
		DescriptorPoolSizes.push_back(PoolSize_Sampler);

		VkDescriptorPoolCreateInfo PoolCreateInfo = {};
		PoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		PoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		PoolCreateInfo.poolSizeCount = DescriptorPoolSizes.size();
		PoolCreateInfo.pPoolSizes = DescriptorPoolSizes.data();
		PoolCreateInfo.maxSets = m_DescriptorMaxSize;


		VkDescriptorPool Pool = {};
		vkCreateDescriptorPool(m_Device->GetDevice(), &PoolCreateInfo, nullptr, &Pool);
		VULKAN_CORE_ERROR("[VULKAN] DescriptorPool #" + ToString(m_DescriptorArray.size()) + " x " + ToString(m_DescriptorMaxSize) + " was Created!");
		m_DescriptorArray.push_back(Pool);
	}

	void VulkanDescriptor::Allocate()
	{
		MORP_PROFILE_FUNCTION();

		// Get Number of Descriptor in Current
		m_DescriptorSets.resize(m_DescriptorSets.size() + 1);
		m_DescriptorSize = m_DescriptorSets.size();

		if (m_DescriptorCount >= m_DescriptorMaxSize) {
			Expand();
			m_DescriptorCount = 0;
		}

		VkDescriptorSetAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		AllocateInfo.descriptorPool = m_DescriptorArray[m_DescriptorArray.size() - 1];
		AllocateInfo.descriptorSetCount = 1;
		AllocateInfo.pSetLayouts = &m_DescriptorLayout;

		VkDescriptorSet DescriptorSet = m_DescriptorSets[m_DescriptorSets.size() - 1];
		VkResult result = vkAllocateDescriptorSets(m_Device->GetDevice(), &AllocateInfo, &DescriptorSet);
		MORP_CORE_ASSERT(result, "[VULKAN] DescriptorSet cannot be Allocated!");

		m_DescriptorSets[m_DescriptorSets.size() - 1] = DescriptorSet;
		VULKAN_CORE_ERROR("[VULKAN] DescriptorPool #" + ToString(m_DescriptorArray.size() - 1) + ", DescriptorSet #" + ToString(m_DescriptorSets.size() - 1) + " was Allocated!");
		m_DescriptorCount++;
	}

	void VulkanDescriptor::UpdateDescriptorWrites(const Vector<VkWriteDescriptorSet>& _Writes)
	{
		MORP_PROFILE_FUNCTION();

		vkUpdateDescriptorSets(m_Device->GetDevice(), _Writes.size(), _Writes.data(), 0, nullptr);
		VULKAN_CORE_ERROR("[VULKAN] Update to DescriptorSet was made!");
	}

	Ref<VulkanDescriptor> VulkanDescriptor::Make(const uint32& _MaxDescriptorSize)
	{
		return CreateRef<VulkanDescriptor>(_MaxDescriptorSize);
	}
}