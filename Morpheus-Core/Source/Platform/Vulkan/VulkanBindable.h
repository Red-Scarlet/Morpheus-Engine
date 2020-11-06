#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	enum class VulkanBindableTypes : uint8
	{
		VulkanNone = 0,
		VulkanVertexArray,
		VulkanShader
	};

	typedef Pair<VulkanBindableTypes, Vector<Ref<void>>> VulkanBindable;

	class VulkanBindableIdentifier
	{
	public:
		uint32_t ID = 0;
		VulkanBindableTypes Type = VulkanBindableTypes::VulkanNone;
		bool Bounded = false;
	};

	class VulkanBindableCache
	{
	public:
		VulkanBindableCache() = default;
		virtual ~VulkanBindableCache() = default;

	public:
		template<typename DataType>
		void Submit(const VulkanBindableTypes& _Type, const Ref<DataType>& _Data)
		{
			auto ConvertedValue = std::static_pointer_cast<void>(_Data);
			String str = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type) {
					m_Cache[i].second.push_back(ConvertedValue);
					MORP_CORE_SPECIAL("[VULKAN] BindableType already exists, new submission was made by " + str);
					return;
				}

			Vector<Ref<void>> NewTypeArray;
			NewTypeArray.push_back(ConvertedValue);
			auto Element = std::make_pair(_Type, NewTypeArray);
			m_Cache.push_back(Element);
			MORP_CORE_SPECIAL("[VULKAN] BindableType does not exists, new submission was made by " + str);
		}

		template<typename DataType>
		Ref<DataType> Get(const VulkanBindableTypes& _Type, const uint32& _ID = 0)
		{
			String str = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type) {
					for (uint32 j = 0; j < m_Cache[i].second.size(); j++) {
						Ref<DataType> Reference = std::static_pointer_cast<DataType>(m_Cache[i].second[_ID]);
						MORP_CORE_SPECIAL("[VULKAN] Retrieval from VulkanBindableCache was made " + str);
						return Reference;
					}
				}

			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + str + " Does not exist in VulkanBindableCache!");
			return nullptr;
		}

		uint32 GetNextBindableID(const VulkanBindableTypes& _Type)
		{
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type)
					return m_Cache[i].second.size();
			return 0;
		}

		bool CheckBinding(const VulkanBindableTypes& _Type, const uint32& _ID)
		{
			if (m_PresentBindable.Type == _Type && m_PresentBindable.ID == _ID)
				return true;
			return false;
		}
		
		bool CheckBindingAny()
		{
			if (m_PresentBindable.Type != VulkanBindableTypes::VulkanNone)
				return true;
			return false;
		}

		void SetPresent(const VulkanBindableIdentifier& _Identifier)
		{
			m_PresentBindable = _Identifier;
		}

		uint32 GetPresentID()
		{
			if(CheckBindingAny())
				return m_PresentBindable.ID;
			return 0;
		}

		uint32 AppendingCount()
		{
			return m_AppendingBindables.size();
		}
			
		void ClearAppending()
		{
			m_AppendingBindables.clear();
		}

		void AddAppending(const VulkanBindableIdentifier& _Identifier)
		{
			m_AppendingBindables.push_back(_Identifier);
		}

		const VulkanBindableIdentifier& GetAppending(const uint32& _Index)
		{
			return m_AppendingBindables.at(_Index);
		}

	private:
		Vector<VulkanBindable> m_Cache;
		VulkanBindableIdentifier m_PresentBindable;
		Vector<VulkanBindableIdentifier> m_AppendingBindables;
	};

}