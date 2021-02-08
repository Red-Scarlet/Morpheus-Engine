#include "Morppch.h"
#include "VulkanDescriptor.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"

namespace Morpheus { namespace Vulkan {

	VulkanDescriptor::VulkanDescriptor(const PipelineDescriptorLayout& _Layout, const uint32& _DescriptorMax)
		: m_Layout(_Layout), m_DescriptorMax(_DescriptorMax), m_DescriptorCount(0)
	{
		m_Device = VulkanInstance::GetInstance()->GetDevice(0);
		Expand();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Descriptor Was Created!");
	}

	VulkanDescriptor::~VulkanDescriptor()
	{
		VULKAN_CORE_WARN("[VULKAN] Descriptor Was Destoryed!");
	}

	const VkDescriptorSet& VulkanDescriptor::GetDescriptorSet(const uint32& _Index)
	{
		uint32 Index = CheckDescriptorAvailability();
		return std::get<VkDescriptorSet>(m_DescriptorSets.at(Index));
	}

	void VulkanDescriptor::UpdateDescriptorWrites(const Vector<VkWriteDescriptorSet>& _Writes)
	{
		MORP_PROFILE_FUNCTION();

		vkUpdateDescriptorSets(m_Device->GetLogical(), _Writes.size(), _Writes.data(), 0, nullptr);
		VULKAN_CORE_ERROR("[VULKAN] Update to DescriptorSet was made!");
	}

	void VulkanDescriptor::VulkanCreate()
	{
		Vector<VkDescriptorSetLayoutBinding> Layouts;
		for (PipelineDescriptorElement element : m_Layout) {
			VkDescriptorSetLayoutBinding Layout = {};
			Layout.binding = element.Binding; 
			Layout.descriptorType = GetVulkanDescriptorTypeFromEngine(element.Type);
			Layout.descriptorCount = 1;
			Layout.stageFlags = GetVulkanDescriptorStageFromEngine(element.Stage);
			Layout.pImmutableSamplers = nullptr;
			Layouts.push_back(Layout);
		}

		VkDescriptorSetLayoutCreateInfo LayoutSetCreateInfo = {};
		LayoutSetCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		LayoutSetCreateInfo.flags = VkDescriptorSetLayoutCreateFlags();
		LayoutSetCreateInfo.bindingCount = Layouts.size();
		LayoutSetCreateInfo.pBindings = Layouts.data();

		vkCreateDescriptorSetLayout(m_Device->GetLogical(), &LayoutSetCreateInfo, nullptr, &m_DescriptorLayout);
	}

	void VulkanDescriptor::VulkanDestory()
	{
		vkDestroyDescriptorSetLayout(m_Device->GetLogical(), m_DescriptorLayout, nullptr);

		for (uint32 i = 0; i < m_DescriptorSets.size(); i++)
			Deallocate(i);
		for (VkDescriptorPool pool : m_DescriptorPool)
			vkDestroyDescriptorPool(m_Device->GetLogical(), pool, nullptr);
		m_DescriptorPool.clear();
		m_DescriptorSets.clear();
		m_DescriptorCount = 0;
		m_DescriptorMax = 0;
	}

	void VulkanDescriptor::Expand()
	{
		MORP_PROFILE_FUNCTION();

		Vector<VkDescriptorPoolSize> DescriptorPoolSizes;
		{
			VkDescriptorPoolSize PoolSize_Uniform = {};
			PoolSize_Uniform.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			PoolSize_Uniform.descriptorCount = m_DescriptorMax;
			DescriptorPoolSizes.push_back(PoolSize_Uniform);

			VkDescriptorPoolSize PoolSize_Sampler = {};
			PoolSize_Sampler.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			PoolSize_Sampler.descriptorCount = m_DescriptorMax;
			DescriptorPoolSizes.push_back(PoolSize_Sampler);
		}

		m_DescriptorPool.resize(m_DescriptorPool.size() + 1);
		VkDescriptorPoolCreateInfo PoolCreateInfo = {};
		PoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		PoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		PoolCreateInfo.poolSizeCount = DescriptorPoolSizes.size();
		PoolCreateInfo.pPoolSizes = DescriptorPoolSizes.data();
		PoolCreateInfo.maxSets = m_DescriptorMax;

		VkResult result = vkCreateDescriptorPool(m_Device->GetLogical(), &PoolCreateInfo, nullptr, &m_DescriptorPool.at(m_DescriptorPool.size() - 1));
		VULKAN_CORE_ASSERT(result, "[VULKAN] Failed to Create a DescriptorPool!");
		VULKAN_CORE_INFO("[VULKAN] DescriptorPool #" + ToString(m_DescriptorPool.size() - 1) + " with maximum size of " +
			ToString(m_DescriptorMax) + " was Created!");
	}

	void VulkanDescriptor::Allocate()
	{
		MORP_PROFILE_FUNCTION();

		m_DescriptorSets.resize(m_DescriptorSets.size() + 1);
		if (m_DescriptorCount >= m_DescriptorMax)
		{ Expand(); m_DescriptorMax += m_DescriptorMax; }
		uint32 PoolIndex = m_DescriptorCount / m_DescriptorMax;

		VkDescriptorSetAllocateInfo AllocateInfo = {};
		AllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		AllocateInfo.descriptorPool = m_DescriptorPool[m_DescriptorPool.size() - 1];
		AllocateInfo.descriptorSetCount = 1;
		AllocateInfo.pSetLayouts = &m_DescriptorLayout;

		VkDescriptorSet& DescriptorSet = std::get<VkDescriptorSet>(m_DescriptorSets.at(m_DescriptorCount));
		VkResult result = vkAllocateDescriptorSets(m_Device->GetLogical(), &AllocateInfo, &DescriptorSet);
		MORP_CORE_ASSERT(result, "[VULKAN] DescriptorSet cannot be Allocated!");

		std::get<bool>(m_DescriptorSets.at(m_DescriptorCount)) = false;
		VULKAN_CORE_INFO("[VULKAN] DescriptorSet #" + ToString(m_DescriptorCount) +
			" was Allocated from Pool #" + ToString(m_DescriptorPool.size() - 1));
		m_DescriptorCount++;
	}

	void VulkanDescriptor::Deallocate(const uint32& _Index)
	{
		uint32 PoolIndex = _Index / m_DescriptorMax;
		Tuple<VkDescriptorSet, bool>& DescriptorSet = m_DescriptorSets.at(_Index);
		if (std::get<bool>(DescriptorSet) == true) {
			vkFreeDescriptorSets(m_Device->GetLogical(), m_DescriptorPool.at(PoolIndex), 1, &std::get<VkDescriptorSet>(DescriptorSet));
			std::remove(std::begin(m_DescriptorSets), std::end(m_DescriptorSets), DescriptorSet);
			m_DescriptorCount--;
		}
	}

	uint32 VulkanDescriptor::CheckDescriptorAvailability()
	{
		MORP_PROFILE_FUNCTION();

		uint32 Index = 0;
		for (auto& Descriptor : m_DescriptorSets)
			if (std::get<bool>(Descriptor) == false) {
				std::get<bool>(Descriptor) = true;
				return Index;
			}
			else Index++;
		Allocate();
		return m_DescriptorSets.size() - 1;
	}


	Ref<VulkanDescriptor> VulkanDescriptor::Create(const PipelineDescriptorLayout& _Layout, const uint32& _DescriptorMax)
	{
		return CreateRef<VulkanDescriptor>(_Layout, _DescriptorMax);
	}

	void VulkanDescriptor::Destroy(const Ref<VulkanDescriptor>& _Descriptor)
	{
		_Descriptor->VulkanDestory();
	}


}}