#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"

#include "VulkanGlobal.h"

namespace Morpheus {

	class VulkanDevice : public VulkanGlobal
	{
	public:
		VulkanDevice();
		virtual ~VulkanDevice();
		void Wait();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		uint32 GetMemoryTypeIndex(uint32 _TypeBits, const vk::MemoryPropertyFlags& _Properties);
		const vk::PhysicalDevice& GetPhysicalDevice() { return m_PhysicalDevice; }
		const vk::Device& GetLogicalDevice() { return m_LogicalDevice; }
		const uint32& GetQueueFamilyIndex() { return m_QueueFamilyIndex; }
		const vk::Queue& GetQueue() { return m_Queue; }

	private:
		const uint32& GetQueueIndex(vk::PhysicalDevice& _PhysicalDevice, vk::QueueFlagBits _Flags);
		Vector<Extention> GetExtensions(const Vector<vk::ExtensionProperties>& _Installed, const Vector<Extention>& _Wanted);

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanSurface> m_Surface;

		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_LogicalDevice;
		uint32 m_QueueFamilyIndex;
		vk::Queue m_Queue;

	public:
		static Ref<VulkanDevice> Make();

	};

}