#include "Morppch.h"
#include "FunctionSystem.h"

namespace Morpheus
{
	FunctionSystem::~FunctionSystem()
	{
	}

	void FunctionSystem::PushFunction(Function<void()> _Function)
	{
		m_Functions.push_back(_Function);
	}

	void FunctionSystem::Reset()
	{
		m_Functions.clear();
	}

}