#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	enum class VulkanResourceTypes : uint8
	{
		VulkanVertexBuffer,
		VulkanIndexBuffer,
		VulkanUniformBuffer,
		VulkanTextureBuffer,

		// For Now
		VulkanRenderpass,
		VulkanFrameBuffer,
		VulkanPipeline,
		VulkanDescriptor,

		// Special Case
		VulkanCommandBuffer
	};

	typedef Pair<VulkanResourceTypes, Vector<Ref<void>>> VulkanResource;

	class VulkanResourceCache
	{
	public:
		VulkanResourceCache() = default;
		virtual ~VulkanResourceCache() = default;

	public:
		template<typename DataType>
		void Submit(const VulkanResourceTypes& _Type, const Ref<DataType>& _Data)
		{
			auto ConvertedValue = std::static_pointer_cast<void>(_Data);
			String str = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type) {
					m_Cache[i].second.push_back(ConvertedValue);
					MORP_CORE_SPECIAL("[VULKAN] ResourceType already exists, new submission was made by " + str);
					return;
				}

			Vector<Ref<void>> NewTypeArray;
			NewTypeArray.push_back(ConvertedValue);
			auto Element = std::make_pair(_Type, NewTypeArray);
			m_Cache.push_back(Element);
			MORP_CORE_SPECIAL("[VULKAN] ResourceType does not exists, new submission was made by " + str);
		}

		template<typename DataType>
		Ref<DataType> Get(const VulkanResourceTypes& _Type, const uint32& _ID = 0)
		{
			String str = typeid(DataType).name();
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type) {
					for (uint32 j = 0; j < m_Cache[i].second.size(); j++) {
						Ref<DataType> Reference = std::static_pointer_cast<DataType>(m_Cache[i].second[_ID]);
						MORP_CORE_SPECIAL("[VULKAN] Retrieval from VulkanResourceCache was made " + str);
						return Reference;
					}
				}

			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] " + str + " Does not exist in VulkanResourceCache!");
			return nullptr;
		}

		uint32 GetNextResourceID(const VulkanResourceTypes& _Type)
		{
			for (uint32 i = 0; i < m_Cache.size(); i++)
				if (m_Cache[i].first == _Type)
					return m_Cache[i].second.size();
			return 0;
		}

	private:
		Vector<VulkanResource> m_Cache;

	};

}