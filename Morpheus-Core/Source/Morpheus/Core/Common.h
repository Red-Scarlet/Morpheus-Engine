#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstddef>
#include <functional>

namespace Morpheus {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{ return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{ return std::make_shared<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Function = std::function<T>;

	template<typename T>
	using List = std::list<T>;

	template<typename T>
	using Vector = std::vector<T>;

	using String = std::string;

	typedef std::uint64_t UINT64;
	typedef std::uint32_t UINT32;
	typedef std::uint16_t UINT16;
	typedef std::uint8_t UINT8;

	typedef std::int64_t INT64;
	typedef std::int32_t INT32;
	typedef std::int16_t INT16;
	typedef std::int8_t INT8;

	typedef double FLOAT64;
	typedef float FLOAT32;
	typedef short FLOAT16;
	typedef char FLOAT8;

	typedef std::nullptr_t NULLPTR;
	typedef void* VOIDPTR;
	
	using FVector8 = Vector<const FLOAT8*>;
}

#ifdef MORP_PLATFORM_WINDOWS
#endif

#define MORP_CORE_ASSERTS

#ifdef MORP_CORE_ASSERTS
#define MORP_CORE_ASSERT(x, ...) { if((x)) { MORP_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
#define MORP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define RC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#ifdef MORP_FLOAT_X64
using FLOAT = Morpheus::FLOAT64;
#else
using FLOAT = Morpheus::FLOAT32;
#endif