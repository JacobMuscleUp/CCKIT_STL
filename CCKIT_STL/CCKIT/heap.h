#ifndef CCKIT_HEAP_H
#define CCKIT_HEAP_H

#include "internal/config.h"
#include "utility.h"

namespace cckit
{
	namespace
	{
		inline int Parent(int _pos) { return (_pos - 1) >> 1; }
		inline int Left(int _pos) { return (_pos << 1) + 1; }
		inline int Right(int _pos) { return (_pos << 1) + 2; }

		template<typename RandomAccessIterator, typename StrictWeakOrdering>
		void PromoteHeap(RandomAccessIterator _first, int _pos, StrictWeakOrdering _compare)
		{
			for (decltype(_pos) parentPos = cckit::Parent(_pos);
				_pos > 0 && _compare(*(_first + parentPos), *(_first + _pos));
				_pos = parentPos, parentPos = cckit::Parent(_pos))
				cckit::swap(*(_first + parentPos), *(_first + _pos));
		}

		template<typename RandomAccessIterator, typename StrictWeakOrdering>
		void DemoteHeap(RandomAccessIterator _first, int _heapSize, int _pos, StrictWeakOrdering _compare)
		{
			decltype(_pos) right = cckit::Right(_pos), left = right - 1, largest = _pos;
			if (left < _heapSize && _compare(*(_first + largest), *(_first + left))) 
				largest = left;
			if (right < _heapSize && _compare(*(_first + largest), *(_first + right)))
				largest = right;
			if (largest != _pos) {
				cckit::swap(*(_first + _pos), *(_first + largest));
				DemoteHeap(_first, _heapSize, largest, _compare);
			}
		}
	}

	template<typename RandomAccessIterator>
	inline bool is_heap(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		return cckit::is_heap(_first, _last, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	inline bool is_heap(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		return cckit::is_heap_until(_first, _last, _compare) == _last;
	}

	template<typename RandomAccessIterator>
	inline RandomAccessIterator is_heap_until(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		return cckit::is_heap_until(_first, _last, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	RandomAccessIterator is_heap_until(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		decltype(_first) current = _first;
		for (; current != _last; ++current) 
			if (_compare(*(_first + Parent(current - _first)), *current)) 
				break;
		return current;
	}

	template<typename RandomAccessIterator>
	inline void make_heap(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		cckit::make_heap(_first, _last, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	inline void make_heap(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		auto heapSize = _last - _first;
		for (decltype(heapSize) i = cckit::Parent(heapSize - 1); i >= 0; --i)
			cckit::DemoteHeap(_first, heapSize, i, _compare);
	}

	template<typename RandomAccessIterator>
	inline void push_heap(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		cckit::push_heap(_first, _last, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	inline void push_heap(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		cckit::PromoteHeap(_first, _last - _first - 1, _compare);
	}

	template<typename RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		cckit::pop_heap(_first, _last, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	inline void pop_heap(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		cckit::swap(*_first, *(_last - 1));
		cckit::DemoteHeap(_first, _last - _first - 1, 0, _compare);
	}

	template<typename RandomAccessIterator>
	inline void sort_heap(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		cckit::sort_heap(_first, _last, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	inline void sort_heap(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		for (; _last != _first; cckit::pop_heap(_first, _last, _compare), --_last) {}
	}

	template<typename RandomAccessIterator>
	inline void remove_heap(RandomAccessIterator _first, RandomAccessIterator _last, RandomAccessIterator _pos)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		cckit::remove_heap(_first, _last, _pos, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	void remove_heap(RandomAccessIterator _first, RandomAccessIterator _last, RandomAccessIterator _pos, StrictWeakOrdering _compare)
	{
		cckit::swap(*_pos, *(_last - 1));
		auto posOffset = _pos - _first;
		cckit::DemoteHeap(_first, _last - _first - 1, posOffset, _compare);
		cckit::PromoteHeap(_first, posOffset, _compare);
	}

	template<typename RandomAccessIterator>
	inline void change_heap(RandomAccessIterator _first, RandomAccessIterator _last, RandomAccessIterator _pos)
	{
		typedef typename cckit::iterator_traits<RandomAccessIterator>::value_type value_type;
		cckit::change_heap(_first, _last, _pos, cckit::less<value_type>());
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	inline void change_heap(RandomAccessIterator _first, RandomAccessIterator _last, RandomAccessIterator _pos, StrictWeakOrdering _compare)
	{
		cckit::remove_heap(_first, _last, _pos, _compare);
		cckit::PromoteHeap(_first, _last - _first - 1, _compare);
	}
}

#endif // !CCKIT_HEAP_H