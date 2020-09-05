#pragma once

#include "Common.h"
#include <list>

namespace Morpheus {

	class FunctionSystem
	{
	public:
		FunctionSystem() = default;
		~FunctionSystem();

		void PushFunction(Function<void()> _Function);

		std::vector<Function<void()>>::iterator begin() { return m_Functions.begin(); }
		std::vector<Function<void()>>::iterator end() { return m_Functions.end(); }

		void Reset();

	private:
		std::vector<Function<void()>> m_Functions;
	};

}