#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	enum class VulkanResourceTypes : uint8
	{
		VulkanNone = 0,
		VulkanVertexBuffer,
		VulkanIndexBuffer,
		VulkanUniformBuffer,
		VulkanTextureBuffer,
			
		VulkanRenderpass,
		VulkanPipeline,
		VulkanDescriptor
	};

	class VulkanResource
	{
	public:
		VulkanResource(const VulkanResourceTypes& _Type) : m_Type(_Type) {}
		virtual ~VulkanResource() = default;

	private:
		virtual void VulkanCreate() = 0;
		virtual void VulkanDestory() = 0;

	public:
		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }
		const VulkanResourceTypes& GetType() { return m_Type; }
		void SetType(const VulkanResourceTypes& _Type) { m_Type = _Type; }

	private:
		uint32 m_ID = 0;
		VulkanResourceTypes m_Type = VulkanResourceTypes::VulkanNone;

	};

	class VulkanResourceCache
	{
	public:
		VulkanResourceCache() = default;
		virtual ~VulkanResourceCache() = default;

	public:
		bool Submit(const Ref<VulkanResource>& _Resource)
		{
			String Info = typeid(_Resource).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetID() == _Resource->GetID())
					if (m_Cache[i]->GetType() == _Resource->GetType()) {
						MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + Info + " Already exists in Vulkan Resource Cache!");
						return false;
					}
			m_Cache.push_back(_Resource);
			MORP_CORE_SPECIAL("[VULKAN] " + Info + " was added to Vulkan Resource Cache!");
			return true;
		}

		template<typename DataType>
		Ref<DataType> Get(const VulkanResourceTypes& _Type, const uint32& _ID = 0)
		{
			String Info = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetID() == _ID)
					if (m_Cache[i]->GetType() == _Type) {
						Ref<DataType> Reference = std::static_pointer_cast<DataType>(m_Cache[i]);
						//MORP_CORE_SPECIAL("[VULKAN] " + Info + " was fetched from Vulkan Resource Cache!");
						return Reference;
					}
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + Info + " Does not exist in Vulkan Resource Cache!");
			return nullptr;
		}

		uint32 GetNextResourceID(const VulkanResourceTypes& _Type)
		{
			uint32 Count = 0;
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					Count++;
			return Count;
		}

	private:
		Vector<Ref<VulkanResource>> m_Cache;

	};

}