#ifndef CCKIT_STATIC_ASSERT_H
#define CCKIT_STATIC_ASSERT_H

namespace cckit
{
#define STATIC_ASSERT_0(expr, msg) char ERROR_##msg[expr ? 1 : 0]
	template<bool> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};
#define STATIC_ASSERT(expr, msg) \
    { CompileTimeError<(expr)> ERROR_##msg; }
}

#endif // !CCKIT_STATIC_ASSERT_H

