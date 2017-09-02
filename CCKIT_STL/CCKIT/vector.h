#ifndef CCKIT_VECTOR_H
#define CCKIT_VECTOR_H

#include <cstdlib>
#include <cassert>
#include <initializer_list>
#include "internal/config.h"
#include "iterator.h"
#include "utility.h"
#include "type_traits.h"
#include "functional.h"
#include "algorithm.h"

namespace cckit
{
	template<typename T, typename Allocator = CCKIT_DEFAULT_ALLOCATOR_TYPE>
	class vector
	{
		typedef vector<T, Allocator> this_type;
	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef std::size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef cckit::reverse_iterator<iterator> reverse_iterator;
		typedef cckit::reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		vector();
		explicit vector(const allocator_type& _allocator);
		vector(size_type _size, const value_type& _val, const allocator_type& _allocator = allocator_type());
		vector(size_type _size, const allocator_type& _allocator = allocator_type());
		template <typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
			vector(InputIterator _first, InputIterator _last, const allocator_type& _allocator = allocator_type());
		vector(const this_type& _src, const allocator_type& _allocator = allocator_type());
		vector(this_type&& _src, const allocator_type& _allocator = allocator_type());
		vector(std::initializer_list<value_type> _ilist, const allocator_type& _allocator = allocator_type());

		~vector();

		this_type& operator=(const this_type& _rhs);
		this_type& operator=(this_type&& _rhs);
		this_type& operator=(std::initializer_list<value_type> _ilist);

		void assign(size_type _size, const value_type& _val);
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		void assign(InputIterator _first, InputIterator _last);
		void assign(std::initializer_list<value_type> _ilist);

		allocator_type get_allocator() const;

		reference at(size_type _pos);
		const_reference at(size_type _pos) const;
		reference operator[](size_type _pos);
		const_reference operator[](size_type _pos) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		value_type* data();
		const value_type* data() const;

		bool empty() const;
		size_type size() const;
		//size_type max_size() const;
		void reserve(size_type _cap);
		size_type capacity() const;
		void shrink_to_fit();

		void clear();

		iterator begin();
		const_iterator begin() const;
		const_iterator cbegin() const;
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		iterator insert(const_iterator _pos, const value_type& _val);
		iterator insert(const_iterator _pos, value_type&& _val);
		iterator insert(const_iterator _pos, size_type _count, const value_type& _val);
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		iterator insert(const_iterator _pos, InputIterator _first, InputIterator _last);
		iterator insert(const_iterator _pos, std::initializer_list<value_type> _ilist);

		template <typename... Args>
		iterator emplace(const_iterator _pos, Args&&... _args);

		iterator erase(const_iterator _pos);
		iterator erase(const_iterator _first, const_iterator _last);

		void push_back(const value_type& _val);
		void push_back(value_type&& _val);
		template <typename... Args>
		reference emplace_back(Args&&... _args);
		void pop_back();

		void resize(size_type _count, const value_type& _val = value_type());

		void swap(this_type& _other);

		// ADDITIONAL OPERATION
		iterator find(size_type _index);
		const_iterator find(size_type _index) const;
		//! ADDITIONAL OPERATION

	private:
		value_type* Allocate(size_type _count);
		void Deallocate(value_type* _ptr, size_type _count);
		void Reserve(size_type _cap);

	private:
		value_type* mArray;
		size_type mCap;
		size_type mSize;
		allocator_type mAllocator;
	};
}// namespace cckit

namespace cckit
{
#pragma region vector<T, Allocator>::vector
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector()
		: mArray(nullptr)
		, mCap(0)
		, mSize(0)
		, mAllocator(allocator_type())
	{}
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector(const allocator_type& _allocator)
		: mArray(nullptr)
		, mCap(0)
		, mSize(0)
		, mAllocator(_allocator)
	{}
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector(size_type _size, const value_type& _val, const allocator_type& _allocator)
		: mArray(nullptr)
		, mCap(_size)
		, mSize(_size)
		, mAllocator(_allocator)
	{
		if (mSize == 0) return;
		mArray = Allocate(mCap);
		fill_n(mArray, mSize, _val);
	}
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector(size_type _size, const allocator_type& _allocator)
		: vector(_size, value_type(), _allocator)
	{}
	template<typename T, typename Allocator>
	template <typename InputIterator
		, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		inline vector<T, Allocator>::vector(InputIterator _first, InputIterator _last, const allocator_type& _allocator)
		: mArray(nullptr)
		, mCap(0)
		, mSize(cckit::distance(_first, _last))
		, mAllocator(_allocator)
	{
		if (mSize == 0) return;
		mArray = Allocate(mCap = mSize);
		copy(_first, _last, mArray);
	}
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector(const this_type& _src, const allocator_type& _allocator)
		: mArray(nullptr)
		, mCap(_src.mCap)
		, mSize(_src.mSize)
		, mAllocator(_allocator)
	{
		if (mSize == 0) return;
		mArray = Allocate(mCap); 
		cckit::copy(_src.mArray, _src.mArray + mSize, mArray);
	}
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector(this_type&& _src, const allocator_type& _allocator)
		: mArray(_src.mArray)
		, mCap(_src.mCap)
		, mSize(_src.mSize)
		, mAllocator(_allocator)
	{
		_src.mArray = nullptr;
		_src.mCap = _src.mSize = 0;
	}
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::vector(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
		: vector(_ilist.begin(), _ilist.end(), _allocator)
	{}
#pragma endregion vector<T, Allocator>::vector

#pragma region vector<T, Allocator>::~vector
	template<typename T, typename Allocator>
	inline vector<T, Allocator>::~vector()
	{
		Deallocate(mArray, mCap);
	}
#pragma endregion vector<T, Allocator>::~vector

#pragma region vector<T, Allocator>::operator=
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::this_type&
		vector<T, Allocator>::operator=(const this_type& _rhs)
	{
		this_type temp(_rhs, _rhs.mAllocator);
		swap(temp);
		return *this;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::this_type&
		vector<T, Allocator>::operator=(this_type&& _rhs)
	{
		this_type temp(cckit::move(_rhs), _rhs.mAllocator);
		swap(temp);
		return *this;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::this_type&
		vector<T, Allocator>::operator=(std::initializer_list<value_type> _ilist)
	{
		assign(_ilist);
		return *this;
	}
#pragma endregion vector<T, Allocator>::operator=

#pragma region vector<T, Allocator>::assign
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::assign(size_type _size, const value_type& _val)
	{
		Deallocate(mArray, mCap);
		mArray = Allocate(mCap = mSize = _size);
		fill_n(mArray, mSize, _val);
	}
	template<typename T, typename Allocator>
	template<typename InputIterator
		, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		inline void vector<T, Allocator>::assign(InputIterator _first, InputIterator _last)
	{
		this_type temp(_first, _last, mAllocator);
		swap(temp);
	}
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::assign(std::initializer_list<value_type> _ilist)
	{
		assign(_ilist.begin(), _ilist.end());
	}
#pragma endregion vector<T, Allocator>::assign

#pragma region vector<T, Allocator>::get_allocator
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::allocator_type
		vector<T, Allocator>::get_allocator() const
	{
		return mAllocator;
	}
#pragma endregion vector<T, Allocator>::get_allocator

#pragma region vector<T, Allocator>::at
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
		vector<T, Allocator>::at(size_type _pos)
	{
#if CCKIT_DEBUG
		assert(_pos >= 0); assert(_pos < size());
#endif
		return *(mArray + _pos);
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
		vector<T, Allocator>::at(size_type _pos) const
	{
#if CCKIT_DEBUG
		assert(_pos >= 0); assert(_pos < size());
#endif
		return *(mArray + _pos);
	}
#pragma endregion vector<T, Allocator>::at

#pragma region vector<T, Allocator>::operator[]
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
		vector<T, Allocator>::operator[](size_type _pos)
	{
		return *(mArray + _pos);
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
		vector<T, Allocator>::operator[](size_type _pos) const
	{
		return *(mArray + _pos);
	}
#pragma endregion vector<T, Allocator>::operator[]

#pragma region vector<T, Allocator>::front
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
		vector<T, Allocator>::front()
	{
		return *mArray;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
		vector<T, Allocator>::front() const
	{
		return *mArray;
	}
#pragma endregion vector<T, Allocator>::front

#pragma region vector<T, Allocator>::back
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::reference
		vector<T, Allocator>::back()
	{
		return *(mArray + mSize - 1);
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reference
		vector<T, Allocator>::back() const
	{
		return *(mArray + mSize - 1);
	}
#pragma endregion vector<T, Allocator>::back

#pragma region vector<T, Allocator>::data
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::value_type*
		vector<T, Allocator>::data()
	{
		return mArray;
	}
	template<typename T, typename Allocator>
	inline const typename vector<T, Allocator>::value_type*
		vector<T, Allocator>::data() const
	{
		return mArray;
	}
#pragma endregion vector<T, Allocator>::data

#pragma region vector<T, Allocator>::size
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::size_type
		vector<T, Allocator>::size() const
	{
		return mSize;
	}
#pragma endregion vector<T, Allocator>::size

#pragma region vector<T, Allocator>::empty
	template<typename T, typename Allocator>
	inline bool vector<T, Allocator>::empty() const
	{
		return mSize == 0;
	}
#pragma endregion vector<T, Allocator>::empty

#pragma region vector<T, Allocator>::reserve
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::reserve(size_type _cap)
	{
		if (_cap > mCap)
			Reserve(_cap);
	}
#pragma endregion vector<T, Allocator>::reserve

#pragma region vector<T, Allocator>::capacity
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::size_type
		vector<T, Allocator>::capacity() const
	{
		return mCap;
	}
#pragma endregion vector<T, Allocator>::capacity

#pragma region vector<T, Allocator>::shrink_to_fit
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::shrink_to_fit()
	{
		if (mCap != mSize)
			Reserve(mSize);
	}
#pragma endregion vector<T, Allocator>::shrink_to_fit

#pragma region vector<T, Allocator>::clear
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::clear()
	{
		Deallocate(mArray, mCap);
		mArray = nullptr;
		mCap = mSize = 0;
	}
#pragma endregion vector<T, Allocator>::clear

#pragma region vector<T, Allocator>::begin
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::begin()
	{
		return mArray;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
		vector<T, Allocator>::begin() const
	{
		return mArray;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
		vector<T, Allocator>::cbegin() const
	{
		return mArray;
	}
#pragma endregion vector<T, Allocator>::begin

#pragma region vector<T, Allocator>::end
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::end()
	{
		return mArray + mSize;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
		vector<T, Allocator>::end() const
	{
		return mArray + mSize;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
		vector<T, Allocator>::cend() const
	{
		return mArray + mSize;
	}
#pragma endregion vector<T, Allocator>::end

#pragma region vector<T, Allocator>::rbegin
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::reverse_iterator
		vector<T, Allocator>::rbegin()
	{
		return reverse_iterator(end());
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
		vector<T, Allocator>::rbegin() const
	{
		return const_reverse_iterator(end());
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
		vector<T, Allocator>::crbegin() const
	{
		return const_reverse_iterator(end());
	}
#pragma endregion vector<T, Allocator>::rbegin

#pragma region vector<T, Allocator>::rend
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::reverse_iterator
		vector<T, Allocator>::rend()
	{
		return reverse_iterator(begin());
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
		vector<T, Allocator>::rend() const
	{
		return const_reverse_iterator(begin());
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_reverse_iterator
		vector<T, Allocator>::crend() const
	{
		return const_reverse_iterator(begin());
	}
#pragma endregion vector<T, Allocator>::rend

#pragma region vector<T, Allocator>::insert
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::insert(const_iterator _pos, const value_type& _val)
	{
		return insert(_pos, 1, _val);
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::insert(const_iterator _pos, value_type&& _val)
	{
		difference_type offset = _pos - begin();
		size_type newSize = mSize + 1;
		
		if (newSize > mCap)
			reserve(2 * newSize);
		iterator current = end();
		for (iterator newPos = mArray + offset; current != newPos; --current)
			*current = *(current - 1);
		current->~value_type();
		::new(current) value_type(cckit::move(_val));
		mSize = newSize;

		return current;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::insert(const_iterator _pos, size_type _count, const value_type& _val)
	{
		difference_type offset = _pos - begin();
		size_type newSize = mSize + _count;
		
		if (newSize > mCap)
			reserve(2 * newSize);
		iterator current = end();
		for (iterator newPos = mArray + offset; current != newPos; --current)
			*(current - 1 + _count) = *(current - 1);
		for (size_type i = 0; i < _count; ++i, ++current)
			*current = _val;
		mSize = newSize;

		return current;
	}
	template<typename T, typename Allocator>
	template<typename InputIterator
		, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::insert(const_iterator _pos, InputIterator _first, InputIterator _last)
	{
		difference_type offset = _pos - begin()
			, count = cckit::distance(_first, _last);
		size_type newSize = mSize + count;

		if (newSize > mCap)
			reserve(2 * newSize);
		iterator current = end();
		for (iterator newPos = mArray + offset; current != newPos; --current)
			*(current - 1 + count) = *(current - 1);
		for (; _first != _last; ++_first, ++current)
			*current = *_first;
		mSize = newSize;

		return current;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::insert(const_iterator _pos, std::initializer_list<value_type> _ilist)
	{
		return insert(_pos, _ilist.begin(), _ilist.end());
	}
#pragma endregion vector<T, Allocator>::insert

#pragma region vector<T, Allocator>::emplace
	template<typename T, typename Allocator>
	template <typename... Args>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::emplace(const_iterator _pos, Args&&... _args)
	{
		difference_type offset = _pos - begin();
		size_type newSize = mSize + 1;
		
		if (newSize > mCap)
			reserve(2 * newSize);
		iterator current = end();
		for (iterator newPos = mArray + offset; current != newPos; --current)
			*current = *(current - 1);
		current->~value_type();
		::new(current) value_type(cckit::forward<Args>(_args)...);
		mSize = newSize;

		return current;
	}
#pragma endregion vector<T, Allocator>::emplace

#pragma region vector<T, Allocator>::erase
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::erase(const_iterator _pos)
	{
		return erase(_pos, _pos + 1);
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::erase(const_iterator _first, const_iterator _last)
	{
		difference_type offset = _first - begin(), count = _last - _first;
		size_type newSize = mSize - count;

		if (count > 0 && mSize > 0) {
			if (3 * newSize <= mCap)
				Reserve(2 * newSize);
			iterator current = mArray + offset;
			for (iterator endPos = end(); current != endPos; ++current)
				*current = *(current + count);
			mSize = newSize;
		}

		return mArray + offset;
	}
#pragma endregion vector<T, Allocator>::erase

#pragma region vector<T, Allocator>::push_back
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::push_back(const value_type& _val)
	{
		insert(cend(), _val);
	}
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::push_back(value_type&& _val)
	{
		insert(cend(), cckit::move(_val));
	}
#pragma endregion vector<T, Allocator>::push_back

#pragma region vector<T, Allocator>::emplace_back
	template<typename T, typename Allocator>
	template <typename... Args>
	inline typename vector<T, Allocator>::reference 
		vector<T, Allocator>::emplace_back(Args&&... _args)
	{
		iterator temp = emplace(cend(), cckit::forward<Args>(_args)...);
		return *temp;
	}
#pragma endregion vector<T, Allocator>::emplace_back

#pragma region vector<T, Allocator>::pop_back
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::pop_back()
	{
		erase(cend());
	}
#pragma endregion vector<T, Allocator>::pop_back

#pragma region vector<T, Allocator>::resize
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::resize(size_type _count, const value_type& _val)
	{
		int difference = static_cast<int>(_count) - mSize;
		
		if (difference > 0)
			insert(cend(), difference, _val);
		else if (difference < 0) {
			for (; difference != 0; ++difference)
				pop_back();
		}
	}
#pragma endregion vector<T, Allocator>::resize

#pragma region vector<T, Allocator>::swap
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::swap(this_type& _other)
	{
		cckit::swap(mArray, _other.mArray);
		cckit::swap(mCap, _other.mCap);
		cckit::swap(mSize, _other.mSize);
		cckit::swap(mAllocator, _other.mAllocator);
	}
#pragma endregion vector<T, Allocator>::swap

#pragma region vector<T, Allocator>::find
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::iterator
		vector<T, Allocator>::find(size_type _index)
	{
	#if CCKIT_DEBUG
		assert(_index >= 0); assert(_index < size());
	#endif
		return mArray + _index;
	}
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::const_iterator
		vector<T, Allocator>::find(size_type _index) const
	{
	#if CCKIT_DEBUG
		assert(_index >= 0); assert(_index < size());
	#endif
		return mArray + _index;
	}
#pragma endregion vector<T, Allocator>::find

#pragma region vector<T, Allocator>::Allocate
	template<typename T, typename Allocator>
	inline typename vector<T, Allocator>::value_type* 
		vector<T, Allocator>::Allocate(size_type _count)
	{
		value_type* pTemp =  ::new(mAllocator.allocate(_count * sizeof(value_type))) value_type;
		for (size_type i = 1; i < _count; ++i)
			::new(pTemp + i) value_type;
		return pTemp;
	}
#pragma endregion vector<T, Allocator>::Allocate

#pragma region vector<T, Allocator>::Deallocate
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::Deallocate(value_type* _ptr, size_type _count)
	{
		if (_ptr) {
			for (size_type i = 0; i < _count; ++i)
				(_ptr + i)->~value_type();
			mAllocator.deallocate(_ptr);
		}
	}
#pragma endregion vector<T, Allocator>::Deallocate

#pragma region vector<T, Allocator>::Reserve
	template<typename T, typename Allocator>
	inline void vector<T, Allocator>::Reserve(size_type _cap)
	{
		value_type* tempArray = Allocate(_cap);
		if (mArray) {
			cckit::copy(mArray, mArray + (cckit::min)(mSize, _cap), tempArray);
			Deallocate(mArray, mCap);
		}
		mArray = tempArray;
		mCap = _cap;
	}
#pragma endregion vector<T, Allocator>::Reserve
}// namespace cckit

#endif