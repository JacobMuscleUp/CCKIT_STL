#ifndef CCKIT_UTILITY_H
#define CCKIT_UTILITY_H

#include <cassert>
#include "internal/config.h"
#include "internal/functional_base.h"
#include "type_traits.h"

namespace cckit
{
#pragma region pair<T1, T2>
	template<typename T1, typename T2>
	struct pair
	{
		typedef pair<T1, T2> this_type;
		typedef T1 first_type;
		typedef T2 second_type;

		first_type first;
		second_type second;

		template<typename U1 = T1, typename U2 = T2
			, typename = enable_if_t<is_default_constructible<U1>::value && is_default_constructible<U2>::value> >
		CCKIT_CONSTEXPR pair();

		template<typename U1 = T1, typename U2 = T2
			, typename = enable_if_t<is_copy_constructible<U1>::value && is_copy_constructible<U2>::value> >
			CCKIT_CONSTEXPR pair(const first_type& _first, const second_type& _second);

		template<typename U1 = T1, typename U2 = T2
			, typename = enable_if_t<is_move_constructible<U1>::value && is_move_constructible<U2>::value> >
			CCKIT_CONSTEXPR pair(first_type&& _first, second_type&& _second);

		template<typename U1, typename U2
			, typename = enable_if_t<is_copy_constructible<U1>::value && is_copy_constructible<U2>::value> >
			CCKIT_CONSTEXPR pair(const pair<U1, U2>& _src);

		template<typename U1, typename U2
			, typename = enable_if_t<is_move_constructible<U1>::value && is_move_constructible<U2>::value> >
			CCKIT_CONSTEXPR pair(pair<U1, U2>&& _src);

		pair(const pair&) = default;
		pair(pair&&) = default;

		template<typename = enable_if_t<is_copy_assignable<first_type>::value && is_copy_assignable<second_type>::value> >
		this_type& operator=(const this_type&);
		template<typename = enable_if_t<is_move_assignable<first_type>::value && is_move_assignable<second_type>::value> >
		this_type& operator=(this_type&&);
		template<typename U1, typename U2
			, typename = enable_if_t<is_assignable<first_type&, const U1&>::value && is_assignable<second_type&, const U2&>::value> >
			this_type& operator=(const pair<U1, U2>&);
		template<typename U1, typename U2
			, typename = enable_if_t<is_assignable<first_type&, U1&&>::value && is_assignable<second_type&, U2&&>::value> >
			this_type& operator=(pair<U1, U2>&&);

		template<typename = enable_if_t<is_swappable<first_type>::value && is_swappable<second_type>::value> >
		void swap(this_type& _other);
	};
	template<typename T1, typename T2>
	CCKIT_CONSTEXPR pair<T1, T2> make_pair(T1&& _arg0, T2&& _arg1)
	{
		return pair<T1, T2>(cckit::forward<T1>(_arg0), cckit::forward<T2>(_arg1));
	}

	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	CCKIT_CONSTEXPR pair<T1,T2>::pair()
		: first(first_type()), second(second_type())
	{}

	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	CCKIT_CONSTEXPR pair<T1, T2>::pair(const first_type& _first, const second_type& _second)
		: first(_first), second(_second)
	{}

	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	CCKIT_CONSTEXPR pair<T1, T2>::pair(first_type&& _first, second_type&& _second)
		: first(cckit::forward<first_type>(_first)), second(cckit::forward<second_type>(_second))
	{}

	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	CCKIT_CONSTEXPR pair<T1, T2>::pair(const pair<U1, U2>& _src)
		: first(_src.first), second(_src.second)
	{}

	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	CCKIT_CONSTEXPR pair<T1, T2>::pair(pair<U1, U2>&& _src)
		: first(cckit::forward<U1>(_src.first)), second(cckit::forward<U2>(_src.second))
	{}

	template<typename T1, typename T2>
	template<typename>
	typename pair<T1, T2>::this_type& 
		pair<T1, T2>::operator=(const this_type& _rhs)
	{
		first = _rhs.first;
		second = _rhs.second;
		return *this;
	}
	template<typename T1, typename T2>
	template<typename>
	typename pair<T1, T2>::this_type&
		pair<T1, T2>::operator=(this_type&& _rhs)
	{
		first = cckit::move(_rhs.first);
		second = cckit::move(_rhs.second);
		return *this;
	}
	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	typename pair<T1, T2>::this_type&
		pair<T1, T2>::operator=(const pair<U1, U2>& _rhs)
	{
		first = _rhs.first;
		second = _rhs.second;
		return *this;
	}
	template<typename T1, typename T2>
	template<typename U1, typename U2, typename>
	typename pair<T1, T2>::this_type&
		pair<T1, T2>::operator=(pair<U1, U2>&& _rhs)
	{
		first = cckit::move(_rhs.first);
		second = cckit::move(_rhs.second);
		return *this;
	}

	template<typename T1, typename T2>
	template<typename>
	void pair<T1, T2>::swap(this_type& _other)
	{
		cckit::swap(first, _other.first);
		cckit::swap(second, _other.second);
	}
#pragma endregion pair<T1, T2>

#pragma region swap
	template<typename T, typename = enable_if_t<is_move_constructible<T>::value && is_move_assignable<T>::value> >
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

#pragma region declval
	template<typename T>
	typename cckit::add_rvalue_reference<T>::type declval();
#pragma endregion declval

	template<typename T>
	struct use_self : public unary_function<T, T> 
	{
		const result_type& operator()(const argument_type& _arg) const { return _arg; }
	};

	template<typename Pair>
	struct use_first : public unary_function<Pair, typename Pair::first_type>
	{
		const result_type& operator()(const argument_type& _arg) const { return _arg.first; }
	};
}

#endif // !CCKIT_UTILITY_H