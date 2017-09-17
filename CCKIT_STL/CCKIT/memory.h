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

	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator _first, InputIterator _last, ForwardIterator _dFirst)
	{
		typedef typename cckit::iterator_traits<ForwardIterator>::value_type value_type;
		decltype(_dFirst) current = _dFirst;
		try {
			for (; _first != _last; ++_first, ++current)
				cckit::uninitialized_init(&*current, *_first);
		}
		catch (...) {
			cckit::destroy(_dFirst, current);
			throw;
		}
		return current;
	}
	template<typename InputIterator, typename Size, typename ForwardIterator>
	ForwardIterator uninitialized_copy_n(InputIterator _first, Size _count, ForwardIterator _dFirst)
	{
		typedef typename cckit::iterator_traits<ForwardIterator>::value_type value_type;
		decltype(_dFirst) current = _dFirst;
		try {
			for (; _count > 0; --_count, ++_first, ++current)
				cckit::uninitialized_init(&*current, *_first);
		}
		catch (...) {
			cckit::destroy(_dFirst, current);
			throw;
		}
		return current;
	}

	template<typename ForwardIterator>
	void uninitialized_fill(ForwardIterator _first, ForwardIterator _last
		, const typename cckit::iterator_traits<ForwardIterator>::value_type& _val)
	{
		typedef typename cckit::iterator_traits<ForwardIterator>::value_type value_type;
		decltype(_first) current = _first;
		try {
			for (; current != _last; ++current)
				cckit::uninitialized_init(&*current, _val);
		}
		catch (...) {
			cckit::destroy(_first, current);
			throw;
		}
	}

	template<typename ForwardIterator, typename Size>
	ForwardIterator uninitialized_fill_n(ForwardIterator _first, Size _count
		, const typename cckit::iterator_traits<ForwardIterator>::value_type& _val)
	{
		typedef typename cckit::iterator_traits<ForwardIterator>::value_type value_type;
		decltype(_first) current = _first;
		try {
			for (; _count > 0; --_count, ++current)
				cckit::uninitialized_init(&*current, _val);
		}
		catch (...) {
			cckit::destroy(_first, current);
			throw;
		}
		return current;
	}

	template<typename T>
	inline void destroy_at(T* _pTarget)
	{
		_pTarget->~T();
	}

	template<typename ForwardIterator>
	void destroy(ForwardIterator _first, ForwardIterator _last)
	{
		for (; _first != _last; ++_first)
			cckit::destroy_at(&*_first);
	}

	template<typename T, typename... Args>
	inline void uninitialized_init(T* _pTarget, Args&&... _args)
	{
		::new(static_cast<void*>(_pTarget)) T(cckit::forward<Args>(_args)...);
	}
	template<typename T>
	inline void uninitialized_default_init(T* _pTarget)
	{
		::new(static_cast<void*>(_pTarget)) T;
	}

	template<typename T, typename... Args>
	inline void initialized_init(T* _pTarget, Args&&... _args)
	{
		destroy_at(_pTarget);
		uninitialized_init(_pTarget, cckit::forward<Args>(_args)...);
	}
	template<typename T>
	inline void initialized_default_init(T* _pTarget)
	{
		destroy_at(_pTarget);
		uninitialized_default_init(_pTarget);
	}

	template<typename InputIterator, typename RandomAccessIterator>
	RandomAccessIterator initialized_insert(InputIterator _insertFirst, InputIterator _insertLast
		, RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		auto count = cckit::distance(_insertFirst, _insertLast);
		decltype(_last) current = _last;
		try {
			for (; current != _first; --current) {
				::new(static_cast<void*>(&*(current - 1 + count))) value_type(cckit::move(*(current - 1)));
				cckit::destroy_at(&*(current - 1));
			}
			for (; _insertFirst != _insertLast; ++_insertFirst, ++current)
				::new(static_cast<void*>(&*current)) value_type(cckit::move(*_insertFirst));
		}
		catch (...) {
			cckit::destroy(_first, _last);
			throw;
		}
		return current;
	}

	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_insert_n(RandomAccessIterator _first, RandomAccessIterator _last
		, typename cckit::iterator_traits<RandomAccessIterator>::value_type&& _val)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		decltype(_last) current = _last;
		try {
			for (; current != _first; --current) {
				::new(static_cast<void*>(&*current)) value_type(cckit::move(*(current - 1)));
				cckit::destroy_at(&*(current - 1));
			}
			::new(static_cast<void*>(&*current)) value_type(cckit::move(_val));
		}
		catch (...) {
			cckit::destroy(_first, _last);
			throw;
		}
		return current;
	}
	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_insert_n(RandomAccessIterator _first, RandomAccessIterator _last
		, const typename cckit::iterator_traits<RandomAccessIterator>::value_type& _val)
	{
		remove_const_t<remove_reference_t<decltype(_val)> > temp = _val;
		return cckit::initialized_insert_n(_first, _last, cckit::move(temp));
	}
	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_insert_n(RandomAccessIterator _first, RandomAccessIterator _last, std::size_t _count
		, typename cckit::iterator_traits<RandomAccessIterator>::value_type&& _val)
	{
		assert((_count > 0));
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		decltype(_last) current = _last;
		try {
			for (; current != _first; --current) {
				::new(static_cast<void*>(&*(current - 1 + _count))) value_type(cckit::move(*(current - 1)));
				cckit::destroy_at(&*(current - 1));
			}
			for (decltype(_count) i = 0; i < _count; ++i, ++current)
				::new(static_cast<void*>(&*current)) value_type(_val);
		}
		catch (...) {
			cckit::destroy(_first, _last);
			throw;
		}
		return current;
	}
	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_insert_n(RandomAccessIterator _first, RandomAccessIterator _last, std::size_t _count
		, const typename cckit::iterator_traits<RandomAccessIterator>::value_type& _val)
	{
		remove_const_t<remove_reference_t<decltype(_val)> > temp = _val;
		return cckit::initialized_insert_n(_first, _last, _count, cckit::move(temp));
	}

	template<typename RandomAccessIterator, typename... Args>
	RandomAccessIterator initialized_emplace(RandomAccessIterator _first, RandomAccessIterator _last, Args&&... _args)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		decltype(_last) current = _last;
		try {
			for (; current != _first; --current) {
				::new(static_cast<void*>(&*current)) value_type(cckit::move(*(current - 1)));
				cckit::destroy_at(&*(current - 1));
			}
			::new(static_cast<void*>(&*current)) value_type(cckit::forward<Args>(_args)...);
		}
		catch (...) {
			cckit::destroy(_first, _last);
			throw;
		}
		return current;
	}

	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_erase(RandomAccessIterator _eraseFirst, std::size_t _count, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		decltype(_eraseFirst) current = _eraseFirst;
		try {
			for (auto newLast = _last - _count; current != newLast; ++current) {
				cckit::destroy_at(&*current);
				::new(static_cast<void*>(&*current)) value_type(cckit::move(*(current + _count)));
			}
			for (; current != _last; cckit::destroy_at(&*current), ++current) {}
		}
		catch (...) {
			cckit::destroy(_eraseFirst, _last);
			throw;
		}
		return _eraseFirst;
	}

	//
	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_insert_after_n(RandomAccessIterator _first, RandomAccessIterator _last
		, typename cckit::iterator_traits<RandomAccessIterator>::value_type&& _val)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		decltype(_first) current = _first;
		try {
			for (; current != _last; ++current) {
				::new(static_cast<void*>(&*(current - 1))) value_type(cckit::move(*current));
				cckit::destroy_at(&*current);
			}
			::new(static_cast<void*>(&*(current - 1))) value_type(cckit::move(_val));
		}
		catch (...) {
			cckit::destroy(_first, _last);
			throw;
		}
		return current;
	}
	template<typename RandomAccessIterator>
	RandomAccessIterator initialized_insert_after_n(RandomAccessIterator _first, RandomAccessIterator _last
		, const typename cckit::iterator_traits<RandomAccessIterator>::value_type& _val)
	{
		remove_const_t<remove_reference_t<decltype(_val)> > temp = _val;
		return cckit::initialized_insert_after_n(_first, _last, cckit::move(temp));
	}
}

#endif // !CCKIT_MEMORY_H