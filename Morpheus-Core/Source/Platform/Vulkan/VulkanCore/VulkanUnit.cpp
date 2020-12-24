#include "Morppch.h"
#include "VulkanUnit.h"

namespace Morpheus { namespace Vulkan {

	VulkanUnit::VulkanUnit(const Ref<VulkanDevice>& _Device)
		: m_Device(_Device)
	{
		CreateUnit();
		VULKAN_CORE_WARN("[VULKAN] Unit Was Created!");
	}

	VulkanUnit::~VulkanUnit()
	{
		DestroyUnit();
		VULKAN_CORE_WARN("[VULKAN] Unit Was Destroyed!");
	}

	void VulkanUnit::Process()
	{
		MORP_PROFILE_FUNCTION();

		for (Ref<VulkanResource> Resource : m_Resources)
			Resource->Accept(std::dynamic_pointer_cast<IResourceDispatcher, ResourceDispatcher>(m_ResourceDispatcher));

		m_Resources.clear();
		m_WorkID = 0;
	}

	void VulkanUnit::MakeResource(const Ref<VulkanResource>& _Resource)
	{
		MORP_PROFILE_FUNCTION();

		m_Resources.push_back(_Resource);
		m_WorkID++;
	}

	void VulkanUnit::MakeResources(const Vector<Ref<VulkanResource>>& _Resources)
	{
		MORP_PROFILE_FUNCTION();

		m_Resources += _Resources;
		m_WorkID++;
	}

	void VulkanUnit::SetThreadID(const ThreadID& _ID)
	{		
		m_ThreadID = _ID;
	}

	const ThreadID& VulkanUnit::GetThreadID()
	{
		return m_ThreadID;
	}

	void VulkanUnit::CreateUnit()
	{
		MORP_PROFILE_FUNCTION();

		m_CommandPool = VulkanCommandPool::Create(m_Device, 32);
		m_ResourceDispatcher = ResourceDispatcher::Create(m_Device, m_CommandPool);
	}

	void VulkanUnit::DestroyUnit()
	{
		MORP_PROFILE_FUNCTION();

		m_CommandPool.reset();
		m_ResourceDispatcher.reset();
	}

	Ref<VulkanUnit> VulkanUnit::Create(const Ref<VulkanDevice>& _Device)
	{
		Ref<UnitCache> s_Cache = VulkanCache<VulkanUnit>::Get(VULKAN_CACHE_UNIT_TYPE);
		Ref<VulkanUnit> s_Unit = CreateRef<VulkanUnit>(_Device);
		s_Cache->Add(s_Cache->GetCount(), s_Unit);
		return s_Unit;
	}

}}