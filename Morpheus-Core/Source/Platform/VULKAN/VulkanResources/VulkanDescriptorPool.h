#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

namespace Morpheus {

	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool(const uint32& _NumObjects);
		virtual ~VulkanDescriptorPool();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const uint32& GetCount() { return m_CurrentSize; }
		void SetDesciptorCount(const uint32& _Count) { m_CurrentSize = _Count; }

		const vk::DescriptorSetLayout& GetLayoutSet() { return m_DescriptorSetLayout; }
		const vk::DescriptorSet& GetDescriptorSet(const uint32& _Index) { return m_DescriptorSets[_Index]; }

		void CreateDescriptorLayout();
		void CreateDescriptorPool();

		void CreateDescriptorSet();	
		void DeleteDescriptorSet();

		//UniformBuffer
		void UpdateDescriptorSet(const uint32& _ID);

	private:
		Ref<VulkanDevice> m_Device;

		vk::DescriptorPool m_DescriptorPool;
		vk::DescriptorSetLayout m_DescriptorSetLayout;

		Vector<vk::DescriptorSet> m_DescriptorSets;
		uint32 m_CurrentSize = 0;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanDescriptorPool> VulkanCreate(const uint32& _NumObjects = 0);
	};

}