#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

namespace Morpheus {

	class VulkanDescriptor
	{
	public:
		VulkanDescriptor(const uint32& _MaxDescriptorSize);
		virtual ~VulkanDescriptor();

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return m_DescriptorLayout; }
		const VkDescriptorSet& GetDescriptorSet(const uint32& _Index);
		void UpdateDescriptorWrites(const Vector<VkWriteDescriptorSet>& _Writes);

	private:
		void VulkanCreate();
		void VulkanDestory();
		void Expand();
		void Allocate();

	private:
		Ref<VulkanLogicalDevice> m_Device;
		VkDescriptorSetLayout m_DescriptorLayout;
		Vector<VkDescriptorPool> m_DescriptorArray;
		Vector<VkDescriptorSet> m_DescriptorSets;

		uint32 m_DescriptorMaxSize = 0;
		uint32 m_DescriptorCount = 0;
		uint32 m_DescriptorSize = 0;

	public:
		static Ref<VulkanDescriptor> Make(const uint32& _MaxDescriptorSize = 16);

	};

}