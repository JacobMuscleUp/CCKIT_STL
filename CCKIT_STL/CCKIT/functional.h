#ifndef CCKIT_FUNCTIONAL_H
#define CCKIT_FUNCTIONAL_H

#include "internal/config.h"
#include "internal/functional_base.h"
#include "utility.h"

namespace cckit
{
#pragma region less
	template<typename T = void>
	struct less : public binary_function<T, T, bool>
	{
		CCKIT_CONSTEXPR bool operator()(const T& _arg0, const T& _arg1) const
		{
			return _arg0 < _arg1;
		}
	};
	// the parameter types and return type are deduced
	template<>
	struct less<void>
	{
		template<typename Arg0, typename Arg1>
		CCKIT_CONSTEXPR auto operator()(Arg0&& _arg0, Arg1&& _arg1) const
			-> decltype(cckit::forward<Arg0>(_arg0) < cckit::forward<Arg1>(_arg1))
		{
			return cckit::forward<Arg0>(_arg0) < cckit::forward<Arg1>(_arg1);
		}
	};
#pragma endregion less
#pragma region greater
	template<typename T = void>
	struct greater : public binary_function<T, T, bool>
	{
		CCKIT_CONSTEXPR bool operator()(const T& _arg0, const T& _arg1) const
		{
			using namespace cckit::rel_ops;
			return _arg0 > _arg1;
		}
	};
	// the parameter types and return type are deduced
	template<>
	struct greater<void>
	{
		template<typename Arg0, typename Arg1>
		CCKIT_CONSTEXPR auto operator()(Arg0&& _arg0, Arg1&& _arg1) const
			-> decltype(cckit::forward<Arg0>(_arg0) > cckit::forward<Arg1>(_arg1))
		{
			using namespace cckit::rel_ops;
			return cckit::forward<Arg0>(_arg0) > cckit::forward<Arg1>(_arg1);
		}
	};
#pragma endregion greater
}

#endif // !CCKIT_FUNCTIONAL_H