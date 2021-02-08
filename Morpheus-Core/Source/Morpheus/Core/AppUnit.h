#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Base.h"

#include "Morpheus/ResourceManager/ResourceUnit.h"

namespace Morpheus {

	class AppUnit : public Base
	{
	public:
		AppUnit(const uint32& _ThreadID);
		~AppUnit();

		virtual void Init() override;
		virtual void Run() override;
		virtual void Stop() override;

	private:
		Ref<ResourceUnit> m_ResourceUnit;
		uint32 m_ThreadID = 0;
		bool m_Running = true;
		
	public:
		static Ref<AppUnit> Create(const uint32& _ThreadID);
	};

}