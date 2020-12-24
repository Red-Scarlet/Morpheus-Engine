#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void InitUnit() = 0;
		virtual void Shutdown() = 0;

		virtual void Flush() = 0;
		virtual void FlushUnit() = 0;

		virtual void SetUnit(const uint32& _ID, const ThreadID& _TID) = 0;
		virtual void TestSomething() = 0;

	public:
		static Scope<GraphicsContext> Create();
	};

}