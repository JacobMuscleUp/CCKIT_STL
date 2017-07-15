#ifndef CCKIT_UTILITY_H
#define CCKIT_UTILITY_H

#include "internal/config.h"
#include "type_traits.h"

namespace cckit
{
#pragma region swap
	template<typename T>
	inline void swap(T& _arg0, T& _arg1) CCKIT_NOEXCEPT
	{
		T temp = cckit::move(_arg0);
		_arg0 = cckit::move(_arg1);
		_arg1 = cckit::move(temp);
	}
#pragma endregion swap

#pragma region rel_ops
	namespace rel_ops
	{
		template <typename T>
		inline bool operator!=(const T& x, const T& y)
		{
			return !(x == y);
		}

		template <typename T>
		inline bool operator>(const T& x, const T& y)
		{
			return (y < x);
		}

		template <typename T>
		inline bool operator<=(const T& x, const T& y)
		{
			return !(y < x);
		}

		template <typename T>
		inline bool operator>=(const T& x, const T& y)
		{
			return !(x < y);
		}
	}
#pragma endregion rel_ops

#pragma region forward
	template<typename T>
	inline CCKIT_CONSTEXPR T&& forward(typename cckit::remove_reference<T>::type& _arg) CCKIT_NOEXCEPT
	{
		return static_cast<T&&>(_arg);
	}
	template<typename T>
	inline CCKIT_CONSTEXPR T&& forward(typename cckit::remove_reference<T>::type&& _arg) CCKIT_NOEXCEPT
	{
		return static_cast<T&&>(_arg);
	}
#pragma endregion forward

#pragma region move
	template<typename T>
	inline CCKIT_CONSTEXPR typename cckit::remove_reference<T>::type&&
		move(T&& _arg) CCKIT_NOEXCEPT
	{
		return static_cast<typename cckit::remove_reference<T>::type&&>(_arg);
	}
#pragma endregion move

#pragma region array_size
	template<typename T, size_t N>
	inline CCKIT_CONSTEXPR size_t array_size(T(&)[N]) CCKIT_NOEXCEPT
	{
		return N;
	}
#pragma endregion array_size
}

#endif // !CCKIT_UTILITY_H