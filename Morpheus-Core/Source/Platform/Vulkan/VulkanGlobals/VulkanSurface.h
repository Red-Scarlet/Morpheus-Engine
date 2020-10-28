#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"	

namespace Morpheus {

	struct SurfaceStruct
	{
		vk::SurfaceKHR Surface;
		vk::Format ColorFormat;
		vk::ColorSpaceKHR ColorSpace;
		vk::Format DepthFormat;
	};

	class VulkanSurface
	{
	public:
		VulkanSurface(const vk::Instance& _Instance, const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex);
		~VulkanSurface();
		// Destory Function Here!

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		void SetupColorFormats();

		const SurfaceStruct& GetStruct() { return m_Struct; }

	private:
		bool CreateSurface();

	private:
		SurfaceStruct m_Struct;

		vk::Instance m_Instance;
		vk::PhysicalDevice m_PhysicalDevice;
		uint32 m_QueueFamilyIndex = 0;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanSurface> Create(const vk::Instance& _Instance, const vk::PhysicalDevice& _Physical, const uint32& _QueueFamilyIndex)
		{ return CreateRef<VulkanSurface>(_Instance, _Physical, _QueueFamilyIndex); }
	};

}