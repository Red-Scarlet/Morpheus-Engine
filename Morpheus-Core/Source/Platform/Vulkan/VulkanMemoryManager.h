#pragma once
#include "Morpheus/Core/Common.h"

#include "VulkanGlobals/VulkanGlobal.h"
#include "VulkanResources/VulkanResource.h"
#include "VulkanBindables/VulkanBindable.h"

namespace Morpheus {

	class VulkanMemoryManager
	{
	protected:
		VulkanMemoryManager();
		~VulkanMemoryManager();

	public:
		static VulkanMemoryManager* GetInstance();

		const Ref<VulkanGlobalCache>& GetGlobalCache()
		{ return m_Globals; }

		const Ref<VulkanResourceCache>& GetResourceCache()
		{ return m_Resources; }

		const Ref<VulkanBindableCache>& GetBindableCache()
		{ return m_Bindables; }

	private:
		void Shutdown();
		void Init();

	private:
		static VulkanMemoryManager* s_Instance;

		Ref<VulkanGlobalCache> m_Globals;
		Ref<VulkanResourceCache> m_Resources;
		Ref<VulkanBindableCache> m_Bindables;

	};

}