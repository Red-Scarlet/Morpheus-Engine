#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

namespace Morpheus {

	typedef const float8* Extention;

	class VulkanInstance
	{
	public:
		VulkanInstance();
		virtual ~VulkanInstance();

		void Destroy();

		const vk::Instance& GetInstance() { return m_Instance; }

	private:
		Vector<Extention> GetExtensions(const Vector<vk::ExtensionProperties>& _Installed, const Vector<Extention>& _Wanted);
		Vector<Extention> GetLayers(const Vector<vk::LayerProperties>& _Installed, const Vector<Extention>& _Wanted);
		void CreateInstance();

	private:
		vk::Instance m_Instance;

	public:
		static Ref<VulkanInstance> Create()
		{ return CreateRef<VulkanInstance>(); }
	};

}