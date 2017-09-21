#ifndef CCKIT_BINARY_HEAP_H
#define CCKIT_BINARY_HEAP_H

#include "../vector.h"
#include "../deque.h"
#include "../algorithm.h"

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
	template<typename T, typename Container = cckit::vector<T>, typename Compare = cckit::less<T> >
	class binary_heap
	{
	private:
		typedef binary_heap<T, Container, Compare> this_type;
	public:
		typedef Container container_type;
		typedef Compare value_compare;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;

	public:
		binary_heap(const value_compare& _compare, const container_type& _container)
			: mCompare(_compare), mContainer(_container) {
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		binary_heap(const value_compare& _compare = value_compare(), container_type&& _container = container_type())
			: mCompare(_compare), mContainer(cckit::move(_container)) {
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		binary_heap(const this_type& _other)
			: binary_heap(_other.mCompare, _other.mContainer)
		{}
		binary_heap(this_type&& _other)
			: binary_heap(_other.mCompare, cckit::move(_other.mContainer))
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit binary_heap(const Allocator& _allocator)
			: mCompare(), mContainer(_allocator) {
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit binary_heap(const value_compare& _compare, const Allocator& _allocator)
			: mCompare(_compare), mContainer(_allocator) {
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit binary_heap(const value_compare& _compare, const container_type& _container, const Allocator& _allocator)
			: mCompare(_compare), mContainer(_container, _allocator) {
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		explicit binary_heap(const value_compare& _compare, container_type&& _container, const Allocator& _allocator)
			: mCompare(_compare), mContainer(cckit::move(_container), _allocator) {
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		binary_heap(const this_type& _other, const Allocator& _allocator)
			: binary_heap(_other.mCompare, _other.mContainer, _allocator)
		{}
		template<typename Allocator, enable_if_t<cckit::uses_allocator<Container, Allocator>::value>* = 0>
		binary_heap(this_type&& _other, const Allocator& _allocator)
			: binary_heap(_other.mCompare, cckit::move(_other.mContainer), _allocator)
		{}
		template<typename InputIterator, enable_if_t<cckit::is_iterator<InputIterator>::value>* = 0>
		binary_heap(InputIterator _first, InputIterator _last, const value_compare& _compare, const container_type& _container)
			: mCompare(_compare), mContainer(_container) {
			mContainer.insert(mContainer.cend(), _first, _last);
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		template<typename InputIterator, enable_if_t<cckit::is_iterator<InputIterator>::value>* = 0>
		binary_heap(InputIterator _first, InputIterator _last
			, const value_compare& _compare = value_compare(), container_type&& _container = container_type())
			: mCompare(_compare), mContainer(cckit::move(_container)) {
			mContainer.insert(mContainer.cend(), _first, _last);
			cckit::make_heap(mContainer.begin(), mContainer.end(), mCompare);
		}

		this_type& operator=(const this_type& _other) { mContainer = _other.mContainer; return *this; }
		this_type& operator=(this_type&& _other) { mContainer = cckit::move(_other.mContainer); return *this; }

		const_reference top() const { return mContainer.front(); }
		bool empty() const { return mContainer.empty(); }
		size_type size() const { return mContainer.size(); }

		void push(const value_type& _val) {
			mContainer.push_back(_val);
			cckit::push_heap(mContainer.begin(), mContainer.end(), mCompare);
		}
		void push(value_type&& _val) {
			mContainer.push_back(cckit::move(_val));
			cckit::push_heap(mContainer.begin(), mContainer.end(), mCompare);
		}

		template<typename... Args >
		void emplace(Args&&... _args) {
			mContainer.emplace_back(cckit::forward<Args>(_args)...);
			cckit::push_heap(mContainer.begin(), mContainer.end(), mCompare);
		}

		void pop() {
			cckit::pop_heap(mContainer.begin(), mContainer.end(), mCompare);
			mContainer.pop_back();
		}

		void swap(this_type& _other) noexcept(noexcept(cckit::swap(mCompare, _other.mCompare)) 
			&& noexcept(cckit::swap(mContainer, _other.mContainer))) {
			cckit::swap(mCompare, _other.mCompare);
			cckit::swap(mContainer, _other.mContainer);
		}

		void remove(size_type _index) {
			assert((_index >= 0)); assert((_index < size()));
			auto first = mContainer.begin();
			cckit::remove_heap(first, mContainer.end(), first + _index, mCompare);
			mContainer.pop_back();
		}

		template<typename... Args>
		void modify(size_type _index, Args... _args) {
			assert((_index >= 0)); assert((_index < size()));
			cckit::initialized_init(&mContainer[_index], cckit::forward<Args>(_args)...);
			auto first = mContainer.begin();
			cckit::change_heap(first, mContainer.end(), first + _index, mCompare);
		}

		const_reference operator[](size_type _index) const { return mContainer[_index]; }
		bool validate() const { return cckit::is_heap(mContainer.begin(), mContainer.end(), mCompare); }

	protected:
		container_type mContainer;
		value_compare mCompare;
	};
}

#endif // !CCKIT_BINARY_HEAP_H