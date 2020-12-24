#pragma once

namespace Morpheus {

	class ApplicationBase
	{
	public:
		~ApplicationBase() = default;

		virtual void Init() = 0;
		virtual void Run() = 0;
		virtual void Stop() = 0;
	};

}