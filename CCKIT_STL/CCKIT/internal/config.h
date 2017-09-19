#ifndef CCKIT_CONFIG_H
#define CCKIT_CONFIG_H

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
#ifndef _AFXDLL
#define _AFXDLL
#endif // !_AFXDLL
#include <afxwin.h> // MFC core and standard components
#include <afxext.h> // MFC extensions

#include <cassert>

#define CCKIT_CONSTEXPR constexpr
#define CCKIT_NOEXCEPT noexcept
#define CCKIT_DEBUG 1
#undef CCKIT_USER_DEFINED_ALLOCATOR

#define CCKIT_ALLOCATOR_DEFAULT_NAME "CCKIT_ALLOCATOR"
#define CCKIT_DEFAULT_ALLOCATOR_TYPE cckit::allocator
#define CCKIT_ASSERT(expr) assert((expr))

typedef size_t cckit_size_t;
typedef ptrdiff_t cckit_ptrdiff_t;

#endif // !CCKIT_CONFIG_H