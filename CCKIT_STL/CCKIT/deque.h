#ifndef CCKIT_DEQUE_H
#define CCKIT_DEQUE_H

#include "internal/blockmap.h"

namespace cckit
{
	template<typename T, typename Allocator = CCKIT_DEFAULT_ALLOCATOR_TYPE
		, typename DataStructure = cckit::blockmap<T, Allocator> >
		class deque : public DataStructure
	{
	private:
		typedef deque<T, Allocator, DataStructure> this_type;
		typedef DataStructure base_type;
	public:
		typedef typename base_type::value_type value_type;
		typedef typename base_type::allocator_type allocator_type;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;		
		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::reverse_iterator reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	public:
		deque() : deque(allocator_type()) {}
		explicit deque(const allocator_type& _allocator)
			: base_type(_allocator) {}
		deque(size_type _count, const value_type& _val, const allocator_type& _allocator = allocator_type())
			: base_type(_count, _val, _allocator) {}
		deque(size_type _count, const allocator_type& _allocator = allocator_type())
			: base_type(_count, _allocator) {}
		template <typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
			deque(InputIterator _first, InputIterator _last, const allocator_type& _allocator = allocator_type())
			: base_type(_first, _last, _allocator) {}
		deque(const this_type& _other)
			: base_type(_other) {}
		deque(const this_type& _other, const allocator_type& _allocator)
			: base_type(_other, _allocator) {}
		deque(this_type&& _other)
			: base_type(cckit::move(_other)) {}
		deque(this_type&& _other, const allocator_type& _allocator)
			: base_type(cckit::move(_other, _allocator)) {}
		deque(std::initializer_list<value_type> _ilist, const allocator_type& _allocator = allocator_type())
			: base_type(_ilist, _allocator) {}

		this_type& operator=(const this_type& _rhs) { static_cast<base_type&>(*this) = _rhs; return *this; }
		this_type& operator=(this_type&& _rhs) { static_cast<base_type&>(*this) = cckit::move(_rhs); return *this; }
		this_type& operator=(std::initializer_list<value_type> _ilist) { static_cast<base_type&>(*this) = _ilist; return *this; }
	};
}

#endif // !CCKIT_DEQUE_H