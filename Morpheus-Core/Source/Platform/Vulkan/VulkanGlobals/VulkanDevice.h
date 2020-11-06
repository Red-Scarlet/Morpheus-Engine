#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"

namespace Morpheus {

	class VulkanDevice
	{
	public:
		VulkanDevice(const Ref<VulkanInstance>& _Instance);
		~VulkanDevice();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		void Wait();

		const vk::Queue& GetQueue() { return m_Queue; }
		const vk::Device& GetLogicalDevice() { return m_LogicalDevice; }
		const vk::PhysicalDevice& GetPhysicalDevice() { return m_PhysicalDevice; }
		const Ref<VulkanSurface>& GetSurface() { return m_Surface; }
		const uint32& GetQueueFamilyIndex() { return m_QueueFamilyIndex; }

		const Ref<VulkanInstance>& GetVulkanInstance() { return m_Instance; }

	private:
		const uint32& GetQueueIndex(vk::PhysicalDevice& _PhysicalDevice, vk::QueueFlagBits _Flags);
		Vector<Extention> GetExtensions(const Vector<vk::ExtensionProperties>& _Installed, const Vector<Extention>& _Wanted);
		void CreateDevice();

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanSurface> m_Surface;

		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_LogicalDevice;
		uint32 m_QueueFamilyIndex;
		vk::Queue m_Queue;
		uint32 m_ID = 0;


	public:
		static Ref<VulkanDevice> VulkanCreate(const Ref<VulkanInstance>& _Instance);

	};

}