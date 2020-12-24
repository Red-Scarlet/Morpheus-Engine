#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

namespace Morpheus { namespace Vulkan {
	
	// Device = 0, Unit = 1, Shader = 2, FrameBuffer = 3, VertexArray = 4
	typedef uint32 VulkanCacheType;
	template <typename T, typename Key = VulkanCacheType>
	class VulkanMultiton
	{
	public:
		~VulkanMultiton()
		{ s_Instances = nullptr; }

		static const Ref<T>& Get(const Key& _Key)
		{
			MORP_PROFILE_FUNCTION();

			if (const auto it = s_Instances.find(_Key); it != s_Instances.end())
				return it->second;
			return s_Instances[_Key] = CreateRef<T>();
		}

	private:
		static std::map<Key, Ref<T>> s_Instances;
	};
	
	template<typename T>
	class VulkanCacheMember
	{
	public:
		VulkanCacheMember() = default;

		void Clear()
		{ m_Cache.clear(); }

		const uint32& GetCount() const 
		{ return m_Count; }
		
		void Add(const uint32& _ID, const Ref<T>& _Type)
		{
			MORP_PROFILE_FUNCTION();
			m_Cache[_ID] = _Type; 
			m_Count++;
		}

		Ref<T> Get(const uint32& _ID)
		{
			MORP_PROFILE_FUNCTION();
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find value #" + ToString(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{
			MORP_PROFILE_FUNCTION();
			return m_Cache.find(_ID) != m_Cache.end();
		}

		void SetBoundID(const uint32& _ID) 
		{ m_Bound = _ID; VULKAN_CORE_DEBUG_BOUND(ToString(m_Bound)); }

		const Ref<T>& GetBound() 
		{
			MORP_PROFILE_FUNCTION();
			if (Exists(m_Bound))
				return Get(m_Bound);
			else return nullptr; 
		}

	private:
		UnorderedMap<uint32, Ref<T>> m_Cache;
		uint32 m_Bound = uint32_max;
		uint32 m_Count = 0;
	};

	template<typename T>
	using VulkanCache = VulkanMultiton<VulkanCacheMember<T>, VulkanCacheType>;

}}

template<typename T, typename Key>
std::map<Key, Morpheus::Ref<T>> Morpheus::Vulkan::VulkanMultiton<T, Key>::s_Instances;