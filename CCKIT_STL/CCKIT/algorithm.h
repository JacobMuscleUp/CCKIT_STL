#ifndef CCKIT_ALGORITHM
#define CCKIT_ALGORITHM

#include "internal/config.h"
#include "type_traits.h"
#include "utility.h"
#include "functional.h"
#include <iostream>
namespace cckit
{
	// NON-MODIFYING SEQUENCE OPERATION
#pragma region all_off
	template<typename InputIterator, typename UnaryPredicate>
	inline bool all_of(InputIterator _first, InputIterator _last, UnaryPredicate _pred)
	{
		return cckit::find_if_not(_first, _last, _pred) == _last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	inline bool any_of(InputIterator _first, InputIterator _last, UnaryPredicate _pred)
	{
		return cckit::find_if(_first, _last, _pred) != _last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	inline bool none_of(InputIterator _first, InputIterator _last, UnaryPredicate _pred)
	{
		return cckit::find_if(_first, _last, _pred) == _last;
	}
#pragma endregion all_off

#pragma region for_each
	template<typename InputIterator, typename UnaryFunction>
	UnaryFunction for_each(InputIterator _first, InputIterator _last, UnaryFunction _func)
	{
		while (_first != _last)
			_func(*(_first++));
		return _func;
	}
#pragma endregion for_each

#pragma region count
	template<typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator _first, InputIterator _last, const typename iterator_traits<InputIterator>::value_type& _val)
	{
		typedef typename iterator_traits<InputIterator>::value_type value_type;
		return cckit::count_if(_first, _last, [&_val](const value_type& _arg) {
			return _arg == _val;
		});
	}
	template<typename InputIterator, typename UnaryPredicate>
	typename iterator_traits<InputIterator>::difference_type
		count_if(InputIterator _first, InputIterator _last, UnaryPredicate _pred)
	{
		typename iterator_traits<InputIterator>::difference_type count = 0;
		for (; _first != _last; ++_first)
			if (_pred(*_first))
				++count;
		return count;
	}
#pragma endregion count

#pragma region equal
	template<typename InputIterator0, typename InputIterator1>
	inline bool equal(InputIterator0 _first0, InputIterator0 _last0, InputIterator1 _first1)
	{
		typedef typename iterator_traits<InputIterator0>::value_type value_type;
		return cckit::equal(_first0, _last0, _first1, [](const value_type& _arg0, const value_type& _arg1) {
			return _arg0 == _arg1;
		});
	}
	template<typename InputIterator0, typename InputIterator1, typename BinaryPredicate>
	bool equal(InputIterator0 _first0, InputIterator0 _last0, InputIterator1 _first1, BinaryPredicate _pred)
	{
		STATIC_ASSERT((is_same<typename iterator_traits<InputIterator1>::value_type
			, typename iterator_traits<InputIterator1>::value_type>::value)
			, VALUE_TYPE_MISMATCH);
		while (_first0 != _last0)
			if (!_pred(*(_first0++), *(_first1++)))
				return false;
		return true;
	}
#pragma endregion equal

#pragma region find
	template<typename InputIterator>
	inline InputIterator find(InputIterator _first, InputIterator _last, const typename iterator_traits<InputIterator>::value_type& _val)
	{
		typedef typename iterator_traits<InputIterator>::value_type value_type;
		return cckit::find_if(_first, _last, [&_val](const value_type& _arg) {
			return _arg == _val;
		});
	}
	template<typename InputIterator, typename UnaryPredicate>
	InputIterator find_if(InputIterator _first, InputIterator _last,
		UnaryPredicate _pred)
	{
		if (_first != _last) {
			for (; _first != _last; ++_first)
				if (_pred(*(_first)))
					return _first;
		}
		return _last;
	}
	template<typename InputIterator, typename UnaryPredicate>
	InputIterator find_if_not(InputIterator _first, InputIterator _last,
		UnaryPredicate _pred)
	{
		if (_first != _last) {
			for (; _first != _last; ++_first)
				if (!_pred(*(_first)))
					return _first;
		}
		return _last;
	}
#pragma endregion find

#pragma region find_end
	template<typename ForwardIterator0, typename ForwardIterator1>
	inline ForwardIterator0 find_end(ForwardIterator0 _first, ForwardIterator0 _last
		, ForwardIterator1 _seqFirst, ForwardIterator1 _seqLast)
	{
		typedef typename iterator_traits<ForwardIterator0>::value_type value_type;
		return cckit::find_end(_first, _last, _seqFirst, _seqLast, [](const value_type& _arg0, const value_type& _arg1) {
			return _arg0 == _arg1;
		});
	}
	template<typename ForwardIterator0, typename ForwardIterator1, typename BinaryPredicate>
	ForwardIterator0 find_end(ForwardIterator0 _first, ForwardIterator0 _last
		, ForwardIterator1 _seqFirst, ForwardIterator1 _seqLast
		, BinaryPredicate _pred)
	{
		STATIC_ASSERT((is_same<typename iterator_traits<ForwardIterator0>::value_type
			, typename iterator_traits<ForwardIterator1>::value_type>::value)
			, VALUE_TYPE_MISMATCH);
		
		ForwardIterator0 seqFirst = _last;
		for (; _first != _last; ++_first) {
			ForwardIterator0 seqFirst0 = cckit::search(_first, _last, _seqFirst, _seqLast, _pred);
			if (seqFirst0 == _last)
				return seqFirst;
			_first = seqFirst = seqFirst0;
		}
		return seqFirst;
	}
#pragma endregion find_end

#pragma region adjacent_find
	template<typename ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator _first, ForwardIterator _last)
	{
		typedef typename iterator_traits<ForwardIterator>::value_type value_type;
		return cckit::adjacent_find(_first, _last, [](const value_type& _arg0, const value_type& _arg1) {
			return _arg0 == _arg1;
		});
	}
	template<typename ForwardIterator, typename BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator _first, ForwardIterator _last, BinaryPredicate _pred)
	{
		ForwardIterator next = _first;
		++next;
		for (; _first != _last; ++_first, ++next) 
			if (_pred(*_first, *next))
				return _first;
		return _first;
	}
#pragma endregion adjacent_find

#pragma region search
	template<typename ForwardIterator0, typename ForwardIterator1>
	inline ForwardIterator0 search(ForwardIterator0 _first, ForwardIterator0 _last
		, ForwardIterator1 _seqFirst, ForwardIterator1 _seqLast)
	{
		typedef typename iterator_traits<ForwardIterator0>::value_type value_type;
		return cckit::search(_first, _last, _seqFirst, _seqLast, [](const value_type& _arg0, const value_type& _arg1) {
			return _arg0 == _arg1;
		});
	}
	template<typename ForwardIterator0, typename ForwardIterator1, typename BinaryPredicate>
	ForwardIterator0 search(ForwardIterator0 _first, ForwardIterator0 _last
		, ForwardIterator1 _seqFirst, ForwardIterator1 _seqLast
		, BinaryPredicate _pred)
	{
		STATIC_ASSERT((is_same<typename iterator_traits<ForwardIterator0>::value_type
			, typename iterator_traits<ForwardIterator1>::value_type>::value)
			, VALUE_TYPE_MISMATCH);

		for (; ; ++_first) {
			ForwardIterator0 current = _first;
			ForwardIterator1 seqCurrent = _seqFirst;
			for (; ; ++current, ++seqCurrent) {
				if (seqCurrent == _seqLast) // the subsequence is empty or a match exists
					return _first;
				if (current == _last) // no match exists
					return _last;
				if (!_pred(*current, *seqCurrent))
					break;
			}
		}
	}
#pragma endregion search

#pragma region search_n
	template<typename ForwardIterator, typename Size>
	ForwardIterator search_n(ForwardIterator _first, ForwardIterator _last
		, Size _count, const typename iterator_traits<ForwardIterator>::value_type& _val)
	{
		return cckit::search_n(_first, _last, _count, _val, [](const decltype(_val)& _arg0, const decltype(_val)& _arg1) {
			return _arg0 == _arg1;
		});
	}
	template<typename ForwardIterator, typename Size, typename BinaryPredicate>
	ForwardIterator search_n(ForwardIterator _first, ForwardIterator _last
		, Size _count, const typename iterator_traits<ForwardIterator>::value_type& _val, BinaryPredicate _pred)
	{
		Size count = 0;
		ForwardIterator seqFirst = _first;
		for (; _first != _last; ++_first) {
			if (!_pred(*_first, _val)) {
				count = 0;
				continue;
			}
			if (count++ == 0) // the head of a potentially matching subsequence is encountered
				seqFirst = _first;
			if (count == _count) // a matching subsequence exists
				return seqFirst;
		}
		if (_first == _last)
			seqFirst = _last;
		return seqFirst;
	}
#pragma endregion search_n
	//! NON-MODIFYING SEQUENCE OPERATION

	// MODIFYING SEQUENCE OPERATION
#pragma region copy
	template<typename InputIterator, typename OutputIterator>
	OutputIterator copy(InputIterator _first, InputIterator _last, OutputIterator _dFirst)
	{
		for (; _first != _last; ++_first, ++_dFirst)
			*_dFirst = *_first;
		return _dFirst;
	}
	template<typename InputIterator, typename OutputIterator, typename UnaryPredicate>
	OutputIterator copy_if(InputIterator _first, InputIterator _last, OutputIterator _dFirst, UnaryPredicate _pred)
	{
		while (_first != _last) {
			if (_pred(*_first))
				*_dFirst++ = *_first;
			++_first;
		}
		return _dFirst;
	}
	template<typename InputIterator, typename Size, typename OutputIterator>
	OutputIterator copy_n(InputIterator _first, Size _count, OutputIterator _dFirst)
	{
		for (; _count > 0; --_count, ++_first, ++_dFirst)
			*_dFirst = *_first;
		return _dFirst;
	}
#pragma endregion copy

#pragma region copy_backward
	template<typename BidirectionalIterator0, typename BidirectionalIterator1>
	BidirectionalIterator1 copy_backward(BidirectionalIterator0 _first, BidirectionalIterator0 _last, BidirectionalIterator1 _dLast)
	{
		while (_last != _first)
			*(--_dLast) = *(--_last);
		return _dLast;
	}
#pragma endregion copy_backward

#pragma region move
	template<typename InputIterator, typename OutputIterator>
	OutputIterator move(InputIterator _first, InputIterator _last, OutputIterator _dFirst)
	{
		for (; _first != _last; ++_first, ++_dFirst) 
			*_dFirst = move(*_first);
		return _dFirst;
	}
#pragma endregion move

#pragma region move_backward
	template<typename BidirectionalIterator0, typename BidirectionalIterator1>
	BidirectionalIterator1 move_backward(BidirectionalIterator0 _first, BidirectionalIterator0 _last, BidirectionalIterator1 _dLast)
	{
		while (_last != _first)
			*(--_dLast) = move(*(--_last));
		return _dLast;
	}
#pragma endregion move_backward

#pragma region fill
	template<typename ForwardIterator>
	void fill(ForwardIterator _first, ForwardIterator _last, const typename iterator_traits<ForwardIterator>::value_type& _val)
	{
		for (; _first != _last; ++_first)
			*_first = _val;
	}
#pragma endregion fill

#pragma region fill_n
	template<typename ForwardIterator, typename Size>
	ForwardIterator fill_n(ForwardIterator _first, Size _count, const typename iterator_traits<ForwardIterator>::value_type& _val)
	{
		for (; _count > 0; --_count, ++_first)
			*_first = _val;
		return _first;
	}
#pragma endregion fill_n

#pragma region transform
	template<typename InputIterator, typename OutputIterator, typename UnaryOperation>
	OutputIterator transform(InputIterator _first, InputIterator _last, OutputIterator _dFirst, UnaryOperation _unaryOp)
	{
		for (; _first != _last; ++_first, ++_dFirst)
			*_dFirst = _unaryOp(*_first);
		return _dFirst;
	}
	template<typename InputIterator0, typename InputIterator1, typename OutputIterator, typename BinaryOperation>
		OutputIterator transform(InputIterator0 _first0, InputIterator0 _last0, InputIterator1 _first1,
			OutputIterator _dFirst, BinaryOperation _binaryOp)
	{
		for (; _first0 != _last0; ++_first0, ++_first1, ++_dFirst)
			*_dFirst = _binaryOp(*_first0, *_first1);
		return _dFirst;
	}
#pragma endregion transform

#pragma region generate
		template<typename ForwardIterator, typename Generator>
		inline void generate(ForwardIterator _first, ForwardIterator _last, Generator _generator)
		{
			cckit::fill(_first, _last, _generator());
		}
#pragma endregion generate

#pragma region generate_n
		template<typename ForwardIterator, typename Size, typename Generator>
		inline ForwardIterator generate_n(ForwardIterator _first, Size _count, Generator _generator)
		{
			return cckit::fill_n(_first, _count, _generator());
		}
#pragma endregion generate_n
	//! MODIFYING SEQUENCE OPERATION

	// SORTING OPERATION
#pragma region is_sorted
	template<typename ForwardIterator, typename StrickWeakOrdering>
	inline bool is_sorted(ForwardIterator _first, ForwardIterator _last, StrickWeakOrdering _compare)
	{
		return cckit::is_sorted_until(_first, _last, _compare) == _last;
	}
	template<typename ForwardIterator>
	inline bool is_sorted(ForwardIterator _first, ForwardIterator _last)
	{
		return cckit::is_sorted_until(_first, _last) == _last;
	}
#pragma endregion is_sorted

#pragma region is_sorted_until
	template<typename ForwardIterator, typename StrickWeakOrdering>
	ForwardIterator is_sorted_until(ForwardIterator _first, ForwardIterator _last, StrickWeakOrdering _compare)
	{
		if (_first != _last) {
			ForwardIterator next = _first;
			while (++next != _last) {
				if (_compare(*next, *_first)) {
					assert(!_compare(*_first, *next));
					return next;
				}
				++_first;
			}
		}
		return _last;
	}
	template<typename ForwardIterator>
	inline ForwardIterator is_sorted_until(ForwardIterator _first, ForwardIterator _last)
	{
		return cckit::is_sorted_until(_first, _last, less<decltype(*_first)>());
	}
#pragma endregion is_sorted_until

#pragma region insertion_sort
	template<typename RandomAccessIterator>
	inline void insertion_sort(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		cckit::insertion_sort(_first, _last, [](const decltype(*_first)& _arg0, const decltype(*_first)& _arg1) {
			return _arg0 < _arg1;
		});
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	void insertion_sort(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		for (auto current = _first; ++current != _last;) {
			remove_reference_t<decltype(*current)> key = *current;

			auto current0 = current;
			auto end = _first; 
			for (--end; --current0 != end && _compare(key, *current0);)
				*(current0 + 1) = *current0;
			*(current0 + 1) = key;
		}
	}
	template<typename T, size_t N>
	void InsertionSort(T(&_arr)[N])
	{
		for (size_t i = 1; i < N; ++i) {
			T key = _arr[i];

			size_t j = i - 1;
			for (; j >= 0 && _arr[j] > key; --j) 
				_arr[j + 1] = _arr[j];
			_arr[j + 1] = key;
		}
	}
#pragma endregion insertion_sort

#pragma region selection_sort
	template<typename RandomAccessIterator>
	inline void selection_sort(RandomAccessIterator _first, RandomAccessIterator _last)
	{
		cckit::selection_sort(_first, _last, [](const decltype(*_first)& _arg0, const decltype(*_first)& _arg1) {
			return _arg0 < _arg1;
		});
	}
	template<typename RandomAccessIterator, typename StrictWeakOrdering>
	void selection_sort(RandomAccessIterator _first, RandomAccessIterator _last, StrictWeakOrdering _compare)
	{
		auto end = _last;
		for (--end; _first != end; ++_first) {
			auto min = cckit::min_element(_first, _last, _compare);
			swap(*_first, *min);
		}
	}
	template<typename T, size_t N>
	void SelectionSort(T(&_arr)[N])
	{
		for (size_t i = 0; i < N - 1; ++i) {
			size_t min = i;

			for (size_t j = i + 1; j < N; ++j)
				if (_arr[j] < _arr[min])
					min = j;
			swap(_arr[i], _arr[min]);
		}
	}
#pragma endregion selection_sort

#pragma region counting_sort
	template<typename T, size_t N>
	void CountingSort(T(&A)[N], enable_if_t<is_integral<T>::value, void>* = nullptr)
	{
		int k = A[0];
		for (int i = 1; i < N; ++i)
			if (A[i] > k)
				k = A[i];

		int *B = new int[N], *C = new int[k + 1];

		for (int i = 0; i <= k; ++i)
			C[i] = 0;
		for (int j = 0; j < N; ++j)
			++C[A[j]];
		for (int i = 1; i <= k; ++i)
			C[i] += C[i - 1];

		for (int j = N - 1; j >= 0; --j) {
			B[C[A[j]] - 1] = A[j];
			--C[A[j]];
		}

		for (int j = 0; j < N; ++j)
			A[j] = move(B[j]);

		delete[] B;
		delete[] C;
	}
#pragma endregion counting_sort
	//! SORTING OPERATION

	// MIN/MAX OPERATION
#pragma region min_element
	template<typename ForwardIterator, typename Compare>
	ForwardIterator min_element(ForwardIterator _first, ForwardIterator _last, Compare _compare)
	{
		if (_first != _last) {
			ForwardIterator min = _first;
			while (++_first != _last) {
				if (_compare(*_first, *min))
					min = _first;
			}
			return min;
		}
		return _last;
	}
	template<typename ForwardIterator>
	ForwardIterator min_element(ForwardIterator _first, ForwardIterator _last)
	{
		return min_element(_first, _last, less<decltype(*_first)>());
	}
#pragma endregion min_element

#pragma region max_element
	template<typename ForwardIterator, typename Compare>
	ForwardIterator max_element(ForwardIterator _first, ForwardIterator _last, Compare _compare)
	{
		if (_first != _last) {
			ForwardIterator max = _first;
			while (++_first != _last) {
				if (_compare(*max, *_first))
					max = _first;
			}
			return max;
		}
		return _last;
	}
	template<typename ForwardIterator>
	ForwardIterator max_element(ForwardIterator _first, ForwardIterator _last)
	{
		return max_element(_first, _last, less<decltype(*_first)>());
	}
#pragma endregion max_element
	//! MIN/MAX OPERATION
}

#endif // !CCKIT_ALGORITHM