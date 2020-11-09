#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	enum class VulkanGlobalTypes : uint8
	{
		VulkanNone = 0,
		VulkanInstance,					//DONE
		VulkanSurface,
		VulkanDevice,

		VulkanSwapchain,
		VulkanQueue, 
		VulkanSynchronization,
		VulkanCommand
	};

	class VulkanGlobal
	{
	public:
		VulkanGlobal(const VulkanGlobalTypes& _Type) : m_Type(_Type) {}
		virtual ~VulkanGlobal() = default;

	private:
		virtual void VulkanCreate() = 0;
		virtual void VulkanDestory() = 0;

	public:
		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }
		const VulkanGlobalTypes& GetType() { return m_Type; }
		void SetType(const VulkanGlobalTypes& _Type) { m_Type = _Type; }

	private:
		uint32 m_ID = 0;
		VulkanGlobalTypes m_Type = VulkanGlobalTypes::VulkanNone;

	};

	class VulkanGlobalCache
	{
	public:
		VulkanGlobalCache() = default;
		virtual ~VulkanGlobalCache() = default;

	public:
		bool Submit(const Ref<VulkanGlobal>& _Global)
		{
			String Info = typeid(_Global).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetID() == _Global->GetID())					
					if (m_Cache[i]->GetType() == _Global->GetType()) {
						MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + Info + " Already exists in Vulkan Global Cache!");
						return false;
					}
			m_Cache.push_back(_Global);
			MORP_CORE_SPECIAL("[VULKAN] " + Info + " was added to Vulkan Global Cache!");
			return true;
		}

		template<typename DataType>
		Ref<DataType> Get(const VulkanGlobalTypes& _Type, const uint32& _ID = 0)
		{
			String Info = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetID() == _ID)
					if (m_Cache[i]->GetType() == _Type) {
						Ref<DataType> Reference = std::static_pointer_cast<DataType>(m_Cache[i]);
						//MORP_CORE_SPECIAL("[VULKAN] " + Info + " was fetched from Vulkan Global Cache!");
						return Reference;
					}
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + Info + " Does not exist in Vulkan Global Cache!");
			return nullptr;
		}

		uint32 GetNextGlobalID(const VulkanGlobalTypes& _Type)
		{
			uint32 Count = 0;
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					Count++;
			return Count;

			//for (uint32 i = 0; i < m_Cache.size(); i++)
			//	if (m_Cache[i]->GetType() == _Type)
			//		return m_Cache.size();
			//return 0;
		}

	private:
		Vector<Ref<VulkanGlobal>> m_Cache;
	};

}