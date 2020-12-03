#include "Morppch.h"
#include "VulkanDescriptor.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"

namespace Morpheus {

	VulkanDescriptor::VulkanDescriptor(const uint32& _MaxDescriptorSize)
		: m_DescriptorMaxSize(_MaxDescriptorSize)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetDevice();

		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] Descriptor Was Created!");
	}

	VulkanDescriptor::~VulkanDescriptor()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] Descriptor Was Destoryed!");
	}

	const vk::DescriptorSet& VulkanDescriptor::GetDescriptorSet(const uint32& _Index)
	{
		if (_Index >= m_DescriptorSize)
			AllocateDescriptorSet();
		return m_DescriptorSets[_Index];
	}

	void VulkanDescriptor::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		vk::DescriptorSetLayoutBinding DescriptorSetLayoutBinding_Uniform = {};
		{
			DescriptorSetLayoutBinding_Uniform.binding = 0;
			DescriptorSetLayoutBinding_Uniform.descriptorType = vk::DescriptorType::eUniformBuffer;
			DescriptorSetLayoutBinding_Uniform.descriptorCount = 1;
			DescriptorSetLayoutBinding_Uniform.stageFlags = vk::ShaderStageFlagBits::eVertex;
			DescriptorSetLayoutBinding_Uniform.pImmutableSamplers = nullptr;
		}

		vk::DescriptorSetLayoutBinding DescriptorSetLayoutBinding_Sampler = {};
		{
			DescriptorSetLayoutBinding_Sampler.binding = 1;
			DescriptorSetLayoutBinding_Sampler.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			DescriptorSetLayoutBinding_Sampler.descriptorCount = 1;
			DescriptorSetLayoutBinding_Sampler.stageFlags = vk::ShaderStageFlagBits::eFragment;
			DescriptorSetLayoutBinding_Sampler.pImmutableSamplers = nullptr;
		}

		Vector<vk::DescriptorSetLayoutBinding> Bindings = { DescriptorSetLayoutBinding_Uniform, DescriptorSetLayoutBinding_Sampler };

		vk::DescriptorSetLayoutCreateInfo LayoutSetCreateInfo = {};
		{
			LayoutSetCreateInfo.flags = vk::DescriptorSetLayoutCreateFlags();
			LayoutSetCreateInfo.bindingCount = Bindings.size();
			LayoutSetCreateInfo.pBindings = Bindings.data();
		}

		m_DescriptorLayout = Device.createDescriptorSetLayout(LayoutSetCreateInfo);

		ExpandDescriptorArray();
	}

	void VulkanDescriptor::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.destroyDescriptorSetLayout(m_DescriptorLayout);
	}

	void VulkanDescriptor::ExpandDescriptorArray()
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		
		Vector<vk::DescriptorPoolSize> DescriptorPoolSizes;

		vk::DescriptorPoolSize PoolSize_Uniform = {};
		PoolSize_Uniform.type = vk::DescriptorType::eUniformBuffer;
		PoolSize_Uniform.descriptorCount = m_DescriptorMaxSize;
		DescriptorPoolSizes.push_back(PoolSize_Uniform);

		vk::DescriptorPoolSize PoolSize_Sampler = {};
		PoolSize_Sampler.type = vk::DescriptorType::eCombinedImageSampler;
		PoolSize_Sampler.descriptorCount = m_DescriptorMaxSize;
		DescriptorPoolSizes.push_back(PoolSize_Sampler);

		vk::DescriptorPoolCreateInfo PoolCreateInfo = {};
		PoolCreateInfo.flags = vk::DescriptorPoolCreateFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
		PoolCreateInfo.poolSizeCount = DescriptorPoolSizes.size();
		PoolCreateInfo.pPoolSizes = DescriptorPoolSizes.data();
		PoolCreateInfo.maxSets = m_DescriptorMaxSize;

		vk::DescriptorPool pool = Device.createDescriptorPool(PoolCreateInfo);
		MORP_CORE_ERROR("[VULKAN] DescriptorPool #" + std::to_string(m_DescriptorArray.size()) + " x " + std::to_string(m_DescriptorMaxSize) + " was Created!");
		m_DescriptorArray.push_back(pool);
	}

	void VulkanDescriptor::AllocateDescriptorSet()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		// Get Number of Descriptor in Current
		m_DescriptorSets.resize(m_DescriptorSets.size() + 1);
		m_DescriptorSize = m_DescriptorSets.size();

		if (m_DescriptorCount >= 16) {
			ExpandDescriptorArray();
			m_DescriptorCount = 0;
		}

		VkDescriptorSetAllocateInfo AllocateInfo = {};
		VkDescriptorSetLayout Layout = VkDescriptorSetLayout(m_DescriptorLayout);
		AllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		AllocateInfo.descriptorPool = m_DescriptorArray[m_DescriptorArray.size() - 1];
		AllocateInfo.descriptorSetCount = 1;
		AllocateInfo.pSetLayouts = &Layout;
		m_DescriptorLayout = vk::DescriptorSetLayout(Layout);

		VkDescriptorSet DescriptorSet = VkDescriptorSet(m_DescriptorSets[m_DescriptorSets.size() - 1]);
		VkResult result = vkAllocateDescriptorSets(Device, &AllocateInfo, &DescriptorSet);
		MORP_CORE_ASSERT(result, "[VULKAN] DescriptorSet cannot be Allocated!");

		m_DescriptorSets[m_DescriptorSets.size() - 1] = vk::DescriptorSet(DescriptorSet);
		VULKAN_CORE_ERROR("[VULKAN] DescriptorPool #" + std::to_string(m_DescriptorArray.size() - 1) + ", DescriptorSet #" + std::to_string(m_DescriptorSets.size() - 1) + " was Allocated!");
		m_DescriptorCount++;
	}

	void VulkanDescriptor::UpdateDescriptorWrites(const Vector<vk::WriteDescriptorSet>& _Writes)
	{
		vk::Device Device = m_Device->GetLogicalDevice();
		Device.updateDescriptorSets(_Writes, nullptr);
		VULKAN_CORE_ERROR("[VULKAN] Update to DescriptorSet was made!");
	}

	Ref<VulkanDescriptor> VulkanDescriptor::Make(const uint32& _MaxDescriptorSize)
	{
		return CreateRef<VulkanDescriptor>(_MaxDescriptorSize);
	}
}