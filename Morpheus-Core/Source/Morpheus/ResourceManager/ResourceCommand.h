#pragma once

#include "Morpheus/Core/Common.h"

#include "ResourceTypes.h"
#include "ResourceManager.h"
#include "ResourceComponentManager.h"
#include "ResourceSystemManager.h"
#include "ResourceCoordinator.h"

namespace Morpheus {

	class ResourceCommand
	{
	public:
		static void Init()
		{
			s_Coordinator = CreateRef<ResourceCoordinator>();
		}

		static void Shutdown()
		{
			s_Coordinator.reset();
		}

		static Resource CreateResource()
		{
			MORP_PROFILE_FUNCTION();
			return s_Coordinator->CreateResource();
		}

		static void DestroyResource(const Resource& _Resource)
		{
			MORP_PROFILE_FUNCTION();
			s_Coordinator->DestroyResource(_Resource);
		}

		template<typename T>
		static void RegisterComponent()
		{
			MORP_PROFILE_FUNCTION();
			s_Coordinator->RegisterComponent<T>();
		}

		template<typename T>
		static void AddComponent(const Resource& _Resource, T _Component)
		{
			MORP_PROFILE_FUNCTION();
			s_Coordinator->AddComponent<T>(_Resource, _Component);
		}

		template<typename T>
		static void RemoveComponent(const Resource& _Resource)
		{
			MORP_PROFILE_FUNCTION();
			s_Coordinator->RemoveComponent<T>(_Resource);
		}

		template<typename T>
		static T& GetComponent(const Resource& _Resource)
		{
			MORP_PROFILE_FUNCTION();
			return s_Coordinator->GetComponent<T>(_Resource);
		}

		template<typename T>
		static bool ContainsComponent(const Resource& _Resource)
		{
			MORP_PROFILE_FUNCTION();
			return s_Coordinator->ContainsComponent<T>(_Resource);
		}

		template<typename T>
		static ResourceComponent GetComponentType()
		{
			MORP_PROFILE_FUNCTION();
			return s_Coordinator->GetComponentType<T>();
		}

		template<typename T>
		static Ref<T> RegisterSystem()
		{
			MORP_PROFILE_FUNCTION();
			return s_Coordinator->RegisterSystem<T>();
		}

		template<typename T>
		static Ref<T> RegisterSystem(const Ref<T>& _System)
		{
			MORP_PROFILE_FUNCTION();
			return s_Coordinator->RegisterSystem<T>(_System);
		}

		template<typename T>
		static void SetSystemSignature(const ResourceSignature& _Signature)
		{
			MORP_PROFILE_FUNCTION();
			s_Coordinator->SetSystemSignature<T>(_Signature);
		}

	private:
		static Ref<ResourceCoordinator> s_Coordinator;
	};

}