#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Morpheus/Renderer/RendererPipeline/RenderPipeline.h"

namespace Morpheus { namespace Vulkan {

	class VulkanDescriptor
	{
	public:
		VulkanDescriptor(const PipelineDescriptorLayout& _Layout, const uint32& _DescriptorMax);
		virtual ~VulkanDescriptor();

		const VkDescriptorSetLayout& GetDescriptorLayout() { return m_DescriptorLayout; }
		const VkDescriptorSet& GetDescriptorSet(const uint32& _Index);
		void UpdateDescriptorWrites(const Vector<VkWriteDescriptorSet>& _Writes);

	private:
		void VulkanCreate();
		void VulkanDestory();

		void Expand();
		void Allocate();
		void Deallocate(const uint32& _Index);
		uint32 CheckDescriptorAvailability();

	private:
		Ref<VulkanDevice> m_Device;
		Vector<VkDescriptorPool> m_DescriptorPool;
		Vector<Tuple<VkDescriptorSet, bool>> m_DescriptorSets;
		VkDescriptorSetLayout m_DescriptorLayout;
		uint32 m_DescriptorMax, m_DescriptorCount;
		PipelineDescriptorLayout m_Layout;

	public:
		static Ref<VulkanDescriptor> Create(const PipelineDescriptorLayout& _Layout, const uint32& _DescriptorMax = 32);
		static void Destroy(const Ref<VulkanDescriptor>& _Descriptor);

	};

}}