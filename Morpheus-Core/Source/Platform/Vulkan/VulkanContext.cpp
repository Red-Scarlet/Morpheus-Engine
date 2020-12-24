#include "Morppch.h"
#include "VulkanContext.h"

#include "Morpheus/Core/Application.h"

namespace Morpheus { namespace Vulkan {

	bool VulkanContext::s_Running = true;

	void VulkanContext::Init()
	{
		MORP_PROFILE_FUNCTION();
		m_MainThreadID = std::this_thread::get_id();

		m_Instance = VulkanInstance::GetInstance();
		m_Instance->GetSurface();
		m_Device = m_Instance->GetDevice(0);
		m_Device->CreateSwapchain();
		m_UnitCache = VulkanCache<VulkanUnit>::Get(VULKAN_CACHE_UNIT_TYPE);
	}

	void VulkanContext::Shutdown()
	{
		MORP_PROFILE_FUNCTION();

		m_UnitCache->Clear();
		m_Instance->Shutdown();
	}

	void VulkanContext::SetUnit(const uint32& _ID, const ThreadID& _TID)
	{
		MORP_PROFILE_FUNCTION();
		Ref<VulkanUnit> unit = m_UnitCache->Get(_ID);
		unit->SetThreadID(_TID);
	}

	void VulkanContext::InitUnit()
	{
		MORP_PROFILE_FUNCTION();

		Ref<VulkanUnit> unit = VulkanUnit::Create(m_Device);
		unit->SetThreadID(m_MainThreadID);
	}

	void VulkanContext::Flush()
	{
		MORP_PROFILE_FUNCTION();

		// Maybe Construct the values.!
	}

	void VulkanContext::TestSomething()
	{
	}

	void VulkanContext::FlushUnit()
	{
		MORP_PROFILE_FUNCTION();

		// Get Functions
		uint32 Count = m_UnitCache->GetCount();
		for (uint32 i = 0; i < Count; i++) {
			Ref<VulkanUnit> unit = m_UnitCache->Get(i);
			if (unit->GetThreadID() == std::this_thread::get_id())
				if (unit->GetWorkID() != 0)
					unit->Process();
		}

	}

}}