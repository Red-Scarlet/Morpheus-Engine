#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cstddef>
#include <functional>
#include <optional>
#include <any>
#include <unordered_map>
#include <bitset>
#include <queue>
#include <array>

#include "Morpheus/Utilities/MorpheusLogger.h"

#ifdef MORP_PLATFORM_WINDOWS
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

	using AnyData = std::any;
	template<typename T>
	inline T AnyCast(const AnyData& _Data)
	{ return std::any_cast<T>(_Data); }

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

	template<typename T, std::size_t length>
	using Array = std::array<T, length>;

	template<std::size_t S>
	using Bitset = std::bitset<S>;

	template<typename T>
	using Queue = std::queue<T>;

	template<typename T>
	using Vector2D = Vector<Vector<T>>;

	template<typename T>
	using Optional = std::optional<T>;

	template<typename T, typename R>
	using Pair = std::pair<T, R>;

	template<typename T, typename R>
	using UnorderedMap = std::unordered_map<T, R>;

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

	typedef bool Boolean;

	typedef std::nullptr_t undefined32;
	//typedef Ref<void> Memory32;
	typedef void* Memory32;


	//using FVector8 = Vector<const float8*>;
}

#define uint64_max std::numeric_limits<Morpheus::uint64>::max()
#define uint32_max std::numeric_limits<Morpheus::uint32>::max()
#define uint16_max std::numeric_limits<Morpheus::uint16>::max()
#define uint8_max std::numeric_limits<Morpheus::uint8>::max()