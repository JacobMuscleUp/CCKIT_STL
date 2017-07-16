#ifndef CCKIT_CONFIG_H
#define CCKIT_CONFIG_H

#include <cassert>

#define CCKIT_CONSTEXPR constexpr
#define CCKIT_NOEXCEPT noexcept
#define CCKIT_DEBUG 1
#undef CCKIT_USER_DEFINED_ALLOCATOR

#define CCKIT_ALLOCATOR_DEFAULT_NAME "CCKIT_ALLOCATOR"
#define CCKIT_DEFAULT_ALLOCATOR_TYPE cckit::allocator

#endif // !CCKIT_CONFIG_H