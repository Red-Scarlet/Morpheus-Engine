#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

#include "VulkanResource.h"

namespace Morpheus {

	class VulkanDescriptorPool : public VulkanResource
	{
	public:
		VulkanDescriptorPool(const uint32& _NumObjects);
		virtual ~VulkanDescriptorPool();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const uint32& GetCount() { return m_CurrentSize; }
		void SetDesciptorCount(const uint32& _Count) { m_CurrentSize = _Count; }
		const vk::DescriptorSetLayout& GetLayoutSet() { return m_DescriptorSetLayout; }
		const vk::DescriptorSet& GetDescriptorSet(const uint32& _Index) { return m_DescriptorSets[_Index]; }

	public:
		void CreateDescriptorLayout();
		void CreateDescriptorPool();
		void CreateDescriptorSet();	
		void DeleteDescriptorSet();
		void UpdateDescriptorSet(const uint32& _ID);
		void FreeDescriptor();

	private:
		Ref<VulkanDevice> m_Device;

		vk::DescriptorPool m_DescriptorPool;
		vk::DescriptorSetLayout m_DescriptorSetLayout;
		Vector<vk::DescriptorSet> m_DescriptorSets;
		uint32 m_CurrentSize = 0;

	public:
		static Ref<VulkanDescriptorPool> Make(const uint32& _NumObjects = 0);
	};

}