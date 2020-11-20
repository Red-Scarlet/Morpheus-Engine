#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanCore/VulkanValidation.h"

namespace Morpheus {

	typedef const float8* Extention;

	class VulkanInstance
	{
	public:
		VulkanInstance();
		virtual ~VulkanInstance();
		const vk::Instance& GetInstance() { return m_Instance; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Vector<Extention> GetExtensions(const Vector<vk::ExtensionProperties>& _Installed, const Vector<Extention>& _Wanted);
		Vector<Extention> GetLayers(const Vector<vk::LayerProperties>& _Installed, const Vector<Extention>& _Wanted);

	private:
		vk::Instance m_Instance;
		VulkanValidation* m_Validation;

	public:
		static Ref<VulkanInstance> Make();
	};

}