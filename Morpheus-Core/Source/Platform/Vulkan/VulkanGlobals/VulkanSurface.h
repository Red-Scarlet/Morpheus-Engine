#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"	

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "VulkanGlobal.h"

namespace Morpheus {

	class VulkanSurface : public VulkanGlobal
	{
	public:
		VulkanSurface(const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex);
		virtual ~VulkanSurface();
		void SetupColorFormats();

		const vk::SurfaceKHR& GetSurface() { return m_Surface; }
		const vk::Format& GetColorFormat() { return m_ColorFormat; }
		const vk::ColorSpaceKHR& GetColorSpace() { return m_ColorSpace; }
		const vk::Format& GetDepthFormat() { return m_DepthFormat; }

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	private:
		Ref<VulkanInstance> m_Instance;
		vk::PhysicalDevice m_PhysicalDevice;
		uint32 m_QueueFamilyIndex = 0;

		vk::SurfaceKHR m_Surface;
		vk::Format m_ColorFormat;
		vk::ColorSpaceKHR m_ColorSpace;
		vk::Format m_DepthFormat;

	public:
		static Ref<VulkanSurface> Make(const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex);

	};

}