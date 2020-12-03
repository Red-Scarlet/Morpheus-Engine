#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

namespace Morpheus {

	class VulkanDescriptor
	{
	public:
		VulkanDescriptor(const uint32& _MaxDescriptorSize);
		virtual ~VulkanDescriptor();

		const vk::DescriptorSetLayout& GetDescriptorSetLayout() const { return m_DescriptorLayout; }
		const uint32& GetDescriptorCount() const { return m_DescriptorSets.size(); }
		const vk::DescriptorSet& GetDescriptorSet(const uint32& _Index);

		void UpdateDescriptorWrites(const Vector<vk::WriteDescriptorSet>& _Writes);

	private:
		void VulkanCreate();
		void VulkanDestory();

		void ExpandDescriptorArray();
		void AllocateDescriptorSet();

	private:
		Ref<VulkanDevice> m_Device;

		vk::DescriptorSetLayout m_DescriptorLayout;

		Vector<vk::DescriptorPool> m_DescriptorArray;
		Vector<vk::DescriptorSet> m_DescriptorSets;

		uint32 m_DescriptorMaxSize = 0;
		uint32 m_DescriptorCount = 0;
		uint32 m_DescriptorSize = 0;

	public:
		static Ref<VulkanDescriptor> Make(const uint32& _MaxDescriptorSize = 16);

	};

}