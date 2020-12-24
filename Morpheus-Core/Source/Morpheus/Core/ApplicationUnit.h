#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/ApplicationBase.h"

#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"

namespace Morpheus {

	class ApplicationUnit : public ApplicationBase
	{
	public:
		ApplicationUnit(const uint32& _ID);
		~ApplicationUnit();

		void SetGraphicsContext(const Ref<GraphicsContext>& _Graphics);
		virtual void Init() override;
		virtual void Run() override;
		virtual void Stop() override;

	private:
		Ref<GraphicsContext> m_Graphics;
		static bool s_Running;
		uint32 m_ID;

	public:
		static Ref<ApplicationUnit> Create(const uint32& _ID);
	};

}