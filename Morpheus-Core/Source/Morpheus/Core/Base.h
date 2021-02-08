#pragma once

namespace Morpheus {

	class Base
	{
	public:
		~Base() = default;
		virtual void Init() = 0;
		virtual void Run() = 0;
		virtual void Stop() = 0;
	};

}