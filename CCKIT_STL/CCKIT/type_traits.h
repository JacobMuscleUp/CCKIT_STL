#ifndef CCKIT_TYPE_TRAITS_H
#define CCKIT_TYPE_TRAITS_H

#include <cstdlib>
#include "internal/config.h"

namespace cckit
{
	namespace detail
	{
		typedef char NoType;
		class YesType { NoType unnamed[2]; };
	}

#pragma region integral_constant
	template <typename T, T Val>
	struct integral_constant
	{
		typedef T value_type;
		typedef integral_constant<T, Val> type;
		static CCKIT_CONSTEXPR T value = Val;

		CCKIT_CONSTEXPR operator value_type() const CCKIT_NOEXCEPT { return value; }
		CCKIT_CONSTEXPR value_type operator()() const CCKIT_NOEXCEPT { return value; }
	};
	typedef integral_constant<bool, true>  true_type;
	typedef integral_constant<bool, false> false_type;
#pragma endregion integral_constant

#pragma region remove_reference
	template <typename T> struct remove_reference { typedef T type; };
	template <typename T> struct remove_reference<T&> { typedef T type; };
	template <typename T> struct remove_reference<T&&> { typedef T type; };
#pragma endregion remove_reference

#pragma region remove_const
	template <typename T> struct remove_const { typedef T type; };
	template <typename T> struct remove_const<const T> { typedef T type; };
	template <typename T> struct remove_const<const T[]> { typedef T type[]; };
	template <typename T, std::size_t N> struct remove_const<const T[N]> { typedef T type[N]; };
#pragma endregion remove_const

#pragma region enable_if
	template<bool, typename T = void>
	struct enable_if {};
	template<typename T>
	struct enable_if<true, T> { typedef T type; };
	template<bool Flag, typename T = void>
	using enable_if_t = typename enable_if<Flag, T>::type;
#pragma endregion enable_if

#pragma region disable_if
	template<bool, typename T = void>
	struct disable_if {};
	template <typename T>
	struct disable_if<false, T> { typedef T type; };
	template<bool Flag, typename T = void>
	using disable_if_t = typename disable_if<Flag, T>::type;
#pragma endregion disable_if

#pragma region conditional
	template<bool, typename T, typename U>
	struct conditional { typedef T type; };
	template<typename T, typename U>
	struct conditional<false, T, U> { typedef U type; };
	template<bool Flag, class T, class U>
	using conditional_t = typename conditional<Flag, T, U>::type;
#pragma endregion conditional

#pragma region is_same
	template<typename T, typename U>
	struct is_same : public false_type { };
	template<typename T>
	struct is_same<T, T> : public true_type { };
#pragma endregion is_same

#pragma region is_void
	template<typename T>
	struct IsVoid
	{
		enum {
			value =
			is_same<T, void>::value ||
			is_same<T, const void>::value ||
			is_same<T, volatile void>::value ||
			is_same<T, const volatile void>::value
		};
	};
	template<typename T>
	struct is_void : public integral_constant<bool, IsVoid<T>::value> {};
#pragma endregion is_void

#pragma region is_convertible
	template<typename From, typename To>
	class IsConvertible
	{
		class dummy {};
		typedef conditional_t<is_void<From>::value, dummy, From> From0;
		typedef conditional_t<is_void<From>::value, dummy, To> To0;

		typedef char tiny;
		class large { tiny unnamed[2]; };

		static detail::NoType Pimpl(...);
		static detail::YesType Pimpl(To0);
		static From0 MakeFrom();
	public:
		static const bool value = sizeof(Pimpl(MakeFrom())) == sizeof(detail::YesType);
	};
	template<typename From, typename To>
	struct is_convertible : public integral_constant<bool, IsConvertible<From, To>::value> {};
#pragma endregion is_convertible

#pragma region ParamTester
	template<typename...>
	struct ParamTester { typedef void type; };
	template<typename... Args>	
	using void_t = typename ParamTester<Args...>::type;
#pragma endregion ParamTester
}

#endif // !CCKIT_TYPE_TRAITS_H