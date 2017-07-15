#ifndef CCKIT_MEMORY_H
#define CCKIT_MEMORY_H

#include "internal/config.h"
#include "type_traits.h"

namespace cckit
{
#pragma region uses_allocator
	template<typename T>
	class HasAllocatorType
	{
		template<typename>
		static detail::NoType Pimpl(...);
		template<typename U>
		static detail::YesType Pimpl(typename U::allocator_type*);
	public:
		static const bool value = sizeof(detail::YesType) == sizeof(Pimpl<T>(nullptr));
	};
	template<typename T, typename Allocator, bool = HasAllocatorType<T>::value>
	struct UsesAllocator : integral_constant<bool, is_convertible<typename T::allocator_type, Allocator>::value> {};
	template<typename T, typename Allocator>
	struct UsesAllocator<T, Allocator, false> : false_type {};
	template<typename T, typename Allocator>
	struct uses_allocator : UsesAllocator<T, Allocator> {};
#pragma endregion uses_allocator
}

#endif // !CCKIT_MEMORY_H