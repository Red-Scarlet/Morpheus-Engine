#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	enum class VulkanBindableTypes : uint8
	{
		VulkanNone = 0,
		VulkanVertexArray,
		VulkanFramebuffer,
		VulkanShader
	};

	struct VulkanBindableIdentifier
	{
	public:
		uint32_t ID = 0;
		VulkanBindableTypes Type = VulkanBindableTypes::VulkanNone;
		bool Bounded = false;
		bool Appending = false;
	};

	class VulkanBindable
	{
	public:
		VulkanBindable(const VulkanBindableTypes& _Type) 
		{ m_Identifier.Type = _Type; }
		virtual ~VulkanBindable() = default;

	private:
		virtual void VulkanCreate() = 0;
		virtual void VulkanDestory() = 0;

	public:
		const uint32& GetID() { return m_Identifier.ID; }
		void SetID(const uint32& _ID) { m_Identifier.ID = _ID; }
		const VulkanBindableTypes& GetType() { return m_Identifier.Type; }
		void SetType(const VulkanBindableTypes& _Type) { m_Identifier.Type = _Type; }
		const bool& GetBound() { return m_Identifier.Bounded; }
		void SetBound(const bool& _Value) { m_Identifier.Bounded = _Value; }

		void SetAppending() { m_Identifier.Appending = true; }

		void AppendBinding(const VulkanBindableIdentifier& _Identifier)
		{ m_Bindings.push_back(_Identifier); }

		const bool& CheckAppening() 
		{ return m_Identifier.Appending; }

		void DisableAppening()
		{ m_Identifier.Appending = false; }

		const VulkanBindableIdentifier& GetIdentifier()
		{ return m_Identifier; }

		const VulkanBindableIdentifier& GetBinding(const VulkanBindableTypes& _Type, const uint32& _Index)
		{ 
			for (uint32 i = 0; i < m_Bindings.size(); i++)
				if(m_Bindings[i].Type == _Type)
					if(m_Bindings[i].ID == _Index)
						return m_Bindings[i];
			return m_Bindings[_Index];
		}

		const uint32& GetBindingsCount(const VulkanBindableTypes& _Type)
		{
			uint32 Count = 0;
			for (uint32 i = 0; i < m_Bindings.size(); i++)
				if (m_Bindings[i].Type == _Type)
					Count++;
			return Count;
		}

	private:
		VulkanBindableIdentifier m_Identifier;
		Vector<VulkanBindableIdentifier> m_Bindings;

	};

	class VulkanBindableCache
	{
	public:
		VulkanBindableCache() = default;
		virtual ~VulkanBindableCache() = default;

	public:
		bool Submit(const Ref<VulkanBindable>& _Data)
		{
			String Info = typeid(_Data).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetID() == _Data->GetID())
					if (m_Cache[i]->GetType() == _Data->GetType()) {
						String Message = "[VULKAN] " + Info + " Already exists in Vulkan Bindable Cache!";
						MORP_CORE_ASSERT(MORP_ERROR, Message);
						return false;
					}
			m_Cache.push_back(_Data);
			MORP_CORE_SPECIAL("[VULKAN] " + Info + " was added to Vulkan Bindable Cache!");
			return true;
		}

		template<typename DataType>
		Ref<DataType> Get(const VulkanBindableTypes& _Type, const uint32& _ID = 0)
		{
			String Info = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetID() == _ID)
					if (m_Cache[i]->GetType() == _Type) {
						Ref<DataType> Reference = std::static_pointer_cast<DataType>(m_Cache[i]);
						return Reference;
					}
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + Info + " Does not exist in Vulkan Bindable Cache!");
			return nullptr;
		}


		uint32 GetNextBindableID(const VulkanBindableTypes& _Type)
		{
			uint32 Count = 0;
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					Count++;
			return Count;
		}

		void DisableBindings(const VulkanBindableTypes& _Type)
		{
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					m_Cache[i]->SetBound(false);
		}

		uint32 GetAppendingCount(const VulkanBindableTypes& _Type)
		{
			uint32 Count = 0;
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					if (m_Cache[i]->CheckAppening())
						Count++;
			return Count;
		}

		bool CheckAvaliable(const VulkanBindableTypes& _Type)
		{
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					return true;
			return false;
		}

		bool CheckBoundAvaliable(const VulkanBindableTypes& _Type)
		{
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					if(m_Cache[i]->GetBound())
						return true;
			return false;
		}
		
		uint32 CheckBound(const VulkanBindableTypes& _Type)
		{
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i]->GetType() == _Type)
					if (m_Cache[i]->GetBound())
						return (m_Cache[i]->GetID());
			return uint32_max;
		}


		/*

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

		*/

	private:
		Vector<Ref<VulkanBindable>> m_Cache;

		//VulkanBindableIdentifier m_PresentBindable;
		//Vector<VulkanBindableIdentifier> m_AppendingBindables;
	};

}