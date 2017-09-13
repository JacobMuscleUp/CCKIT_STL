#ifndef CCKIT_PRIORITY_QUEUE_H
#define CCKIT_PRIORITY_QUEUE_H

#include "internal/binary_heap.h"

namespace cckit
{
	/*
	A priority queue is a container adaptor that provides constant time lookup of the largest (by default) element, at the expense of logarithmic
	insertion and extraction. A user-provided Compare can be supplied to change the ordering, e.g. using std::greater<T> would cause the smallest
	element to appear as the top(). Working with a priority_queue is similar to managing a heap in some random access container, with the benefit
	of not being able to accidentally invalidate the heap.

	"T"
	The type of the stored elements. The behavior is undefined if T is not the same type as Container::value_type.

	"Container"
	The type of the underlying container to use to store the elements. The container must satisfy the requirements of SequenceContainer, and its
	iterators must satisfy the requirements of RandomAccessIterator. Additionally, it must provide the following functions with the usual semantics:
	front()
	push_back()
	pop_back()
	The standard containers std::vector and std::deque satisfy these requirements.

	"Compare"
	A Compare type providing a strict weak ordering.
	*/
	template<typename T, typename Container = cckit::vector<T>, typename Compare = cckit::less<T>
		, template<typename, typename, typename> class DataStructure = cckit::binary_heap>
	class priority_queue : public DataStructure<T, Container, Compare>
	{
	private:
		typedef priority_queue<T, Container, Compare> this_type;
		typedef DataStructure<T, Container, Compare> base_type;
	public:
		typedef Container container_type;
		typedef Compare value_compare;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;

	public:
		priority_queue(const value_compare& _compare, const container_type& _container)
			: base_type(_compare, _container)
		{}
		priority_queue(const value_compare& _compare = value_compare(), container_type&& _container = container_type())
			: base_type(_compare, cckit::move(_container))
		{}
		priority_queue(const this_type& _other)
			: base_type(_other)
		{}
		priority_queue(this_type&& _other)
			: base_type(cckit::move(_other))
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit priority_queue(const Allocator& _allocator)
			: base_type(_allocator) 
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit priority_queue(const value_compare& _compare, const Allocator& _allocator)
			: base_type(_compare, _allocator)
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit priority_queue(const value_compare& _compare, const container_type& _container, const Allocator& _allocator)
			: base_type(_compare, _container, _allocator)
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit priority_queue(const value_compare& _compare, container_type&& _container, const Allocator& _allocator)
			: base_type(_compare, cckit::move(_container), _allocator)
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		priority_queue(const this_type& _other, const Allocator& _allocator)
			: base_type(_other, _allocator)
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		priority_queue(this_type&& _other, const Allocator& _allocator)
			: base_type(cckit::move(_other), _allocator)
		{}
		template<typename InputIterator, enable_if_t<cckit::is_iterator<InputIterator>::value>* = 0>
		priority_queue(InputIterator _first, InputIterator _last, const value_compare& _compare, const container_type& _container)
			: base_type(_first, _last, _compare, _container)
		{}
		template<typename InputIterator, enable_if_t<cckit::is_iterator<InputIterator>::value>* = 0>
		priority_queue(InputIterator _first, InputIterator _last
			, const value_compare& _compare = value_compare(), container_type&& _container = container_type())
			: base_type(_first, _last, _compare, cckit::move(_container))
		{}

		this_type& operator=(const this_type& _other) { static_cast<base_type&>(*this) = _other; return *this; }
		this_type& operator=(this_type&& _other) { static_cast<base_type&>(*this) = cckit::move(_other); return *this; }
	};
}

#endif // !CCKIT_PRIORITY_QUEUE_H