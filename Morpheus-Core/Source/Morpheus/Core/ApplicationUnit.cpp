#include "Morppch.h"
#include "ApplicationUnit.h"

#include "Morpheus/Core/Application.h"
#include <sstream>
namespace Morpheus {

	bool ApplicationUnit::s_Running = true;

	ApplicationUnit::ApplicationUnit(const uint32& _ID)
		: m_ID(_ID)
	{
	}

	ApplicationUnit::~ApplicationUnit()
	{
	}

	void ApplicationUnit::SetGraphicsContext(const Ref<GraphicsContext>& _Graphics)
	{ m_Graphics = _Graphics; }

	void ApplicationUnit::Init()
	{
		MORP_PROFILE_FUNCTION();
		m_Graphics->InitUnit();
	}

	void ApplicationUnit::Run()
	{
		MORP_PROFILE_FUNCTION();

		m_Graphics->SetUnit(m_ID, std::this_thread::get_id());
		Application& app = Application::Get();

		while (s_Running)
		{
			m_Graphics->FlushUnit();

			//s_Running = app.GetRunning();
		}
	}

	void ApplicationUnit::Stop()
	{
		MORP_PROFILE_FUNCTION();

		s_Running = false;
	}

	Ref<ApplicationUnit> ApplicationUnit::Create(const uint32& _ID)
	{
		return CreateRef<ApplicationUnit>(_ID);
	}

}