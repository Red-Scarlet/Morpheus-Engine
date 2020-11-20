#pragma once

#include "Morpheus/Core/Common.h"
#include "EntityCSTypes.h"

namespace Morpheus {

	class EntityManager
	{
	public:
		EntityManager()
		{
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
				m_AvailableEntities.push(entity);
		}

		Entity CreateEntity()
		{
			MORP_CORE_ASSERT(m_LivingEntityCount < MAX_ENTITIES, "Too many entities in existence.");
			Entity id = m_AvailableEntities.front();
			m_AvailableEntities.pop();
			++m_LivingEntityCount;
			return id;
		}

		void DestroyEntity(Entity entity)
		{
			MORP_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			m_Signatures[entity].reset();

			m_AvailableEntities.push(entity);
			--m_LivingEntityCount;
		}

		void SetSignature(Entity entity, Signature signature)
		{
			MORP_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			m_Signatures[entity] = signature;
		}

		Signature GetSignature(Entity entity)
		{
			MORP_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			return m_Signatures[entity];
		}

	private:
		Queue<Entity> m_AvailableEntities = {};
		Array<Signature, MAX_ENTITIES> m_Signatures = {};
		uint32 m_LivingEntityCount = {};

	};

}