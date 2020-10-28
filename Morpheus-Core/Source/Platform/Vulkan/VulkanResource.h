#pragma once

#include "Morpheus/Core/Common.h"

#include <typeinfo>
#include <map>

namespace Morpheus {

	enum class VulkanResourceType : uint32
	{
		Surface,
		Device,
		Swapchain,
		
		CommandSystem,
		Synchronization,

		DescriptorPool,
		VertexBuffer,
		IndexBuffer,
		UniformBuffer,

		Renderpass,
		FrameBuffer,
		Shader,
		Pipeline
	};

	enum class VulkanBindables : uint32
	{
		VertexArray,
		Shader
	};

	// [ParentType, ParentID], [ResourceType, ResourceID]
	typedef std::pair<VulkanBindables, std::pair<uint32, bool>> BindableIdentifiers;

	typedef std::pair<VulkanResourceType, uint32> ResourceIdentifier;
	typedef std::pair<std::pair<VulkanBindables, uint32>, std::pair<VulkanResourceType, uint32>> BindingPairs;
	typedef std::pair<VulkanResourceType, std::unordered_map<uint32, Ref<void>>> ResourceValue;

	static BindingPairs MakeBindingPairs(const VulkanBindables& BindingType, const uint32& BindingID, const VulkanResourceType& ResourceType, const uint32& ResourceID)
	{
		std::pair<VulkanBindables, uint32> Parent = std::make_pair(BindingType, BindingID);
		std::pair<VulkanResourceType, uint32> Resource = std::make_pair(ResourceType, ResourceID);
		return std::make_pair(Parent, Resource);
	}

	class VulkanResourceCache
	{
	protected:
		VulkanResourceCache();
		~VulkanResourceCache();

	public:
		static VulkanResourceCache* GetInstance();

		template<typename DataType>
		void Submit(const VulkanResourceType& _Type, const Ref<DataType>& _Data, const uint32& _ID = 0)
		{
			auto ConvertedValue = std::static_pointer_cast<void>(_Data);
			String str = typeid(DataType).name();

			// Check if Types already Exist
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type) {
					// if true then submit into real cache
					m_Cache[i].second[_ID] = ConvertedValue;
					MORP_CORE_SPECIAL("[VULKAN] #0 Submittion To Vulkan Resource Cache was made " + str);
					return;
				}
			// It is not within the array(cache)
			// Create new allocation as unmap

			std::unordered_map<uint32, Ref<void>> NewCache;
			NewCache[_ID] = ConvertedValue;
			auto Element = std::make_pair(_Type, NewCache);
			m_Cache.push_back(Element);
			MORP_CORE_SPECIAL("[VULKAN] #1 Submittion To Vulkan Resource Cache was made " + str);
		}

		// Retrieves An Vulkan Resource that has first been allocated, if given ID it will Retreive the nth resource
		template<typename DataType>
		Ref<DataType> Get(const VulkanResourceType& _Type, const uint32& _ID = 0)
		{
			String str = typeid(DataType).name();

			for (uint32 i = 0; i < m_Cache.size(); i++) {
				if (m_Cache[i].first == _Type) {
					auto Iter = m_Cache[i].second.find(_ID);
					if (Iter != m_Cache[i].second.end()) {
						Ref<DataType> Reference = std::static_pointer_cast<DataType>(Iter->second);
						MORP_CORE_SPECIAL("[VULKAN] Retrieval From Vulkan Resource Cache was made " + str);
						return Reference;
					} else 
						MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + str + " Does not exist in Vulkan Resource Cache!");
				}
			}
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + str + " Does not exist in Vulkan Resource Cache!");	
		}

		const uint32& GetNextResourceID(const VulkanResourceType& _Type)
		{
			for (uint32 i = 0; i < m_ResourceIdentifiers.size(); i++)
				if (m_ResourceIdentifiers[i].first == _Type) {
					m_ResourceIdentifiers[i].second += 1;
					return m_ResourceIdentifiers[i].second;
				}
			m_ResourceIdentifiers.push_back({ _Type, 0 });
			return m_ResourceIdentifiers[m_ResourceIdentifiers.size() - 1].second;
		}

		const uint32& GetResourceCount(const VulkanResourceType& _Type)
		{
			uint32 Count = 0;
			for (uint32 i = 0; i < m_ResourceIdentifiers.size(); i++)
				if (m_ResourceIdentifiers[i].first == _Type)
					Count++;
			return Count;
		}

		const uint32& GetNextBindingID(const VulkanBindables& _Type)
		{
			for (uint32 i = 0; i < m_BindableIdentifiers.size(); i++)
				if (m_BindableIdentifiers[i].first == _Type) {
					m_BindableIdentifiers[i].second.first += 1;
					return m_BindableIdentifiers[i].second.first;
				}
			m_BindableIdentifiers.push_back({ _Type, { 0, false } });
			return m_BindableIdentifiers[m_BindableIdentifiers.size() - 1].second.first;
		}

		void SetBindedPairs(const BindingPairs& _Binding)
		{
			m_BindedPairs.push_back(_Binding);
		}

		void SetBinding(const VulkanBindables& _Type, const uint32& _ID, const bool& _Value)
		{
			for (uint32 i = 0; i < m_BindableIdentifiers.size(); i++)
				if (m_BindableIdentifiers[i].first == _Type)
					if (m_BindableIdentifiers[i].second.first == _ID) {
						m_BindableIdentifiers[i].second.second = _Value;
						return;
					}
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Unable to bind this resource!");
		}

		bool CheckBinding(const VulkanBindables& _Type, const uint32& _ID)
		{
			for (uint32 i = 0; i < m_BindableIdentifiers.size(); i++)
				if (m_BindableIdentifiers[i].first == _Type)
					if (m_BindableIdentifiers[i].second.first == _ID)
						if (m_BindableIdentifiers[i].second.second)
							return true;
			return false;
		}

		const uint32& GetBindingCount(const VulkanBindables& _Filter)
		{
			uint32 BindingCount = 0;
			for (uint32 i = 0; i < m_BindableIdentifiers.size(); i++)
				if (m_BindableIdentifiers[i].first == _Filter)
					BindingCount++;
			return BindingCount;
		}

		/*
		
					uint32 BindingCount;
			for (uint32 i = 0; i < m_BindableIdentifiers.size(); i++)	// 1 { VA | 0 }
			{
				for (uint32 j = 0; j < m_BindedPairs.size(); j++)		// 3 { VA | 0 || VBO 0 } { VA | 0 || IBO 0 } { VA | 0 || UBO 0 }
				{
					if (m_BindedPairs[j].first.first == m_BindableIdentifiers[i].first) {
						if (m_BindedPairs[j].first.second == m_BindableIdentifiers[i].second.first) {

							// Resource has been binded.


						}

					}
				}
			}
		*/

	private:
		void Shutdown();
		void Init();

	private:
		static VulkanResourceCache* s_Instance;
		Vector<ResourceIdentifier> m_ResourceIdentifiers;
		Vector<BindableIdentifiers> m_BindableIdentifiers;
		Vector<BindingPairs> m_BindedPairs;
		Vector<ResourceValue> m_Cache;


	};

}