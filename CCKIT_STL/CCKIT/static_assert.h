namespace cckit
{
#define STATIC_ASSERT_0(expr, msg) char ERROR_##msg[expr ? 1 : 0]
	template<int> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};
#define STATIC_ASSERT(expr, msg) \
    { CompileTimeError<(expr)> ERROR_##msg; /*(void)ERROR_##msg;*/ }
}