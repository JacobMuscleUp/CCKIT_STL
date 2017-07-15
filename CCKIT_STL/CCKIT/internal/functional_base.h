#ifndef CCKIT_FUNCTIONAL_BASE_H
#define CCKIT_FUNCTIONAL_BASE_H

namespace cckit
{
	template <typename Arg, typename Result>
	struct unary_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};


	template <typename Arg0, typename Arg1, typename Result>
	struct binary_function
	{
		typedef Arg0 first_argument_type;
		typedef Arg1 second_argument_type;
		typedef Result result_type;
	};
}

#endif // !CCKIT_FUNCTIONAL_BASE_H