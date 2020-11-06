#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstddef>
#include <functional>
#include <optional>

#ifdef MORP_PLATFORM_WINDOWS
#endif

#define MORP_CORE_LOGGING

#ifdef MORP_CORE_LOGGING
	#define MORP_CORE_TRACE(...)		::Morpheus::MorpheusLogger::LogTrace(__VA_ARGS__)
	#define MORP_CORE_INFO(...)			::Morpheus::MorpheusLogger::LogInfo(__VA_ARGS__)
	#define MORP_CORE_WARN(...)			::Morpheus::MorpheusLogger::LogWarn(__VA_ARGS__)
	#define MORP_CORE_ERROR(...)		::Morpheus::MorpheusLogger::LogError(__VA_ARGS__)
	#define MORP_CORE_SPECIAL(...)		::Morpheus::MorpheusLogger::LogSpecial(__VA_ARGS__)
	#define MORP_CORE_SPECIAL_2(...)	::Morpheus::MorpheusLogger::LogSpecialOverride(__VA_ARGS__)
#else
	#define MORP_CORE_TRACE(...)
	#define MORP_CORE_INFO(...)
	#define MORP_CORE_WARN(...)
	#define MORP_CORE_ERROR(...)
	#define MORP_CORE_SPECIAL(...)
	#define MORP_CORE_SPECIAL_2(...)
#endif

#define MORP_CORE_ASSERTS

#define MORP_ERROR true
#ifdef MORP_CORE_ASSERTS
	#define MORP_CORE_ASSERT(x, ...) { if((x)) { MORP_CORE_ERROR(__VA_ARGS__); __debugbreak(); }}
#else
	#define MORP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define MORP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Morpheus {

	//	unique_ptr (Scope)
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{ return std::make_unique<T>(std::forward<Args>(args)...); }

	//	shared_ptr (Ref)
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{ return std::make_shared<T>(std::forward<Args>(args)...); }

	template<typename T, typename ... Args>
	constexpr Ref<T> CastRef(Args&& ... args)
	{ return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...); }

	template <typename T1, typename T2>
		struct offset_of_impl {
		static T2 object;
		static constexpr size_t offset(T1 T2::* member) {
			return size_t(&(offset_of_impl<T1, T2>::object.*member)) -
				size_t(&offset_of_impl<T1, T2>::object);
		}
	};

	template <typename T1, typename T2>
	T2 offset_of_impl<T1, T2>::object;

	template <typename T1, typename T2>
	inline constexpr size_t offset_of(T1 T2::* member) {
		return offset_of_impl<T1, T2>::offset(member);
	}

	template<typename T>
	using Function = std::function<T>;

	template<typename T>
	using List = std::list<T>;

	template<typename T>
	using Vector = std::vector<T>;

	template<typename T>
	using Optional = std::optional<T>;

	template<typename T, typename R>
	using Pair = std::pair<T, R>;

	using String = std::string;
	using Ostream = std::ostream;

	typedef std::uint64_t uint64;
	typedef std::uint32_t uint32;
	typedef std::uint16_t uint16;
	typedef std::uint8_t uint8;

	typedef std::int64_t int64;
	typedef std::int32_t int32;
	typedef std::int16_t int16;
	typedef std::int8_t int8;

	typedef double float64;
	typedef float float32;
	typedef short float16;
	typedef char float8;

	typedef float64** pfloat64;
	typedef float32** pfloat32;
	typedef float16** pfloat16;
	typedef float8** pfloat8;

	typedef float32 floatm;

	typedef std::nullptr_t NULLPTR;
	typedef void* VOIDPTR;
	
	//using FVector8 = Vector<const float8*>;
}