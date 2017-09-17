#ifndef CCKIT_BLOCKMAP_H
#define CCKIT_BLOCKMAP_H

#include "config.h"
#include "../algorithm.h"
#include "../utility.h"
#include "../iterator.h"

namespace cckit
{
	template<typename T, typename Pointer, typename Reference, cckit_size_t BlockSize>
	class BlockMapIterator;
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	class blockmap;

	template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
	bool operator==(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _lhs
		, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _rhs);
	template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
	bool operator!=(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _lhs
		, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _rhs);
	template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
	cckit_ptrdiff_t operator-(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _arg0
		, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _arg1);
	
	template<typename T, typename Pointer, typename Reference, cckit_size_t BlockSize>
	class BlockMapIterator : public cckit::iterator<cckit::random_access_iterator_tag, T>
	{
		typedef BlockMapIterator<T, Pointer, Reference, BlockSize> this_type;
		typedef BlockMapIterator<T, T*, T&, BlockSize> iterator;
		typedef BlockMapIterator<T, const T*, const T&, BlockSize> const_iterator;

		value_type* mpCurrent;
		value_type** mpCurrentBlockPtr;

		BlockMapIterator(value_type* _pCurrent, value_type** _pCurentBlockPtr)
			: mpCurrent(_pCurrent), mpCurrentBlockPtr(_pCurentBlockPtr)
		{}

	public:
		BlockMapIterator() 
			: mpCurrent(nullptr), mpCurrentBlockPtr(nullptr)
		{}
		BlockMapIterator(const this_type& _other)
			: mpCurrent(_other.mpCurrent), mpCurrentBlockPtr(_other.mpCurrentBlockPtr)
		{}

		pointer operator->() const { return mpCurrent; }
		reference operator*() const { return *mpCurrent; }
		reference operator[](difference_type _index) { return *((*this + _index).mpCurrent); }

		this_type& operator++() {
			if (++mpCurrent == *mpCurrentBlockPtr + BlockSize)
				mpCurrent = *++mpCurrentBlockPtr;
			return *this;
		}
		this_type operator++(int) {
			this_type temp = *this;
			++*this;
			return temp;
		}

		this_type& operator--() {
			if (mpCurrent == *mpCurrentBlockPtr) {
				mpCurrent = *--mpCurrentBlockPtr;
				mpCurrent += BlockSize;
			}
			--mpCurrent;
			return *this;
		}
		this_type operator--(int) {
			this_type temp = *this;
			--*this;
			return temp;
		}

		this_type& operator+=(difference_type _offset) {
			if (_offset < 0)
				*this -= -_offset;
			else if (_offset > 0) {
				//for (; _offset > 0; --_offset, ++*this);
				decltype(_offset) temp = mpCurrent - *mpCurrentBlockPtr
					, temp0 = (temp + _offset) / BlockSize
					, temp1 = _offset - (*mpCurrentBlockPtr + BlockSize - mpCurrent) - (BlockSize * (temp0 - 1));
				mpCurrent = *(mpCurrentBlockPtr += temp0) + temp1;
			}
			return *this;
		}
		this_type& operator-=(difference_type _offset) {
			if (_offset < 0)
				*this += -_offset;
			else if (_offset > 0) {
				//for (; _offset > 0; --_offset, --*this);
				decltype(_offset) temp = mpCurrent - *mpCurrentBlockPtr
					, temp0 = (_offset - temp + BlockSize - 1) / BlockSize
					, temp1 = _offset - temp - (BlockSize * (temp0 - 1));
				mpCurrent = *(mpCurrentBlockPtr -= temp0) + BlockSize - temp1;
			}
			return *this;
		}

		this_type operator+(difference_type _offset) const {
			this_type temp = *this;
			temp += _offset;
			return temp;
		}
		this_type operator-(difference_type _offset) const {
			this_type temp = *this;
			temp -= _offset;
			return temp;
		}

		operator iterator() const { return iterator(mpCurrent, mpCurrentBlockPtr); }
		operator const_iterator() const { return const_iterator(mpCurrent, mpCurrentBlockPtr); }

		template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
		friend bool operator== <>(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _lhs
			, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _rhs);
		template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
		friend bool operator!= <>(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _lhs
			, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _rhs);
		template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
		friend cckit_ptrdiff_t operator-(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _arg0
			, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _arg1);

		template<typename, typename, typename, cckit_size_t>
		friend class BlockMapIterator;
		template<typename, typename, cckit_size_t>
		friend class blockmap;
	};
	template<typename T, typename Pointer, typename Reference, cckit_size_t BlockSize>
	BlockMapIterator<T, Pointer, Reference, BlockSize> operator+(cckit_ptrdiff_t _offset, const BlockMapIterator<T, Pointer, Reference, BlockSize>& _iter) {
		return _iter + _offset;
	}
	template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
	bool operator==(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _lhs
		, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _rhs) {
		return _lhs.mpCurrent == _rhs.mpCurrent;
	}
	template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
	bool operator!=(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _lhs
		, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _rhs) {
		return _lhs.mpCurrent != _rhs.mpCurrent;
	}
	template<typename U, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1, cckit_size_t BlockSize0>
	cckit_ptrdiff_t operator-(const BlockMapIterator<U, Pointer0, Reference0, BlockSize0>& _arg0
		, const BlockMapIterator<U, Pointer1, Reference1, BlockSize0>& _arg1) {
		return BlockSize0 * ((_arg0.mpCurrentBlockPtr - _arg1.mpCurrentBlockPtr) - 1)
			+ (_arg0.mpCurrent - *_arg0.mpCurrentBlockPtr)
			+ (*_arg1.mpCurrentBlockPtr + BlockSize0 - _arg1.mpCurrent);
	}

	template<typename T, typename Allocator = CCKIT_DEFAULT_ALLOCATOR_TYPE, cckit_size_t BlockSize = 5>
	class blockmap
	{
		typedef blockmap<T, Allocator, BlockSize> this_type;
	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef cckit_size_t size_type;
		typedef cckit_ptrdiff_t difference_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef cckit::BlockMapIterator<T, T*, T&, BlockSize> iterator;
		typedef cckit::BlockMapIterator<T, const T*, const T&, BlockSize> const_iterator;
		typedef cckit::reverse_iterator<iterator> reverse_iterator;
		typedef cckit::reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		blockmap() : blockmap(allocator_type()) {}
		explicit blockmap(const allocator_type& _allocator);

		~blockmap();
		
		reference operator[](size_type _pos) { return mBegin[_pos]; }
		const_reference operator[](size_type _pos) const { return (const_cast<this_type*>(this))->operator[](_pos); }

		iterator begin() { return mBegin; }
		const_iterator begin() const { return mBegin; }
		const_iterator cbegin() const { return mBegin; }
		iterator end() { return mEnd; }
		const_iterator end() const { return mEnd; }
		const_iterator cend() const { return mEnd; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
		const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

		size_type size() const { return mEnd - mBegin; }
		void shrink_to_fit();

		iterator insert(const_iterator _pos, const value_type& _val);
		iterator insert(const_iterator _pos, value_type&& _val);
		iterator insert(const_iterator _pos, size_type _count, const value_type& _val);
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
			iterator insert(const_iterator _pos, InputIterator _first, InputIterator _last);
		iterator insert(const_iterator _pos, std::initializer_list<value_type> _ilist);

	private:
		struct ShiftLeft {};
		struct ShiftRight {};

		value_type* AllocateBlock() const;
		value_type** AllocateBlockMap(size_type _blockCount) const;
		void DeallocateBlock(value_type* _pBlock) const;
		void DeallocateBlockMap(value_type** _pMap) const;
		difference_type Distance(const_iterator _first, const_iterator _last) const;

		template<typename U
			, typename = typename cckit::enable_if_t<cckit::is_same<cckit::remove_const_t<cckit::remove_reference_t<U> >, T>::value> >
			iterator Insert(const_iterator _pos, U&& _val);
		template<typename U
			, typename = typename cckit::enable_if_t<cckit::is_same<cckit::remove_const_t<cckit::remove_reference_t<U> >, T>::value> >
		iterator Insert(const_iterator _pos, U&& _val, ShiftLeft);
		template<typename U
			, typename = typename cckit::enable_if_t<cckit::is_same<cckit::remove_const_t<cckit::remove_reference_t<U> >, T>::value> >
		iterator Insert(const_iterator _pos, U&& _val, ShiftRight);

	public:
		value_type** mpBlockMap;
		size_type mMapSize;
		iterator mBegin;
		iterator mEnd;
		allocator_type mAllocator;
	};
}

namespace cckit
{
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline blockmap<T, Allocator, BlockSize>::blockmap(const allocator_type& _allocator)
		: mpBlockMap(nullptr), mMapSize(1), mBegin(), mEnd(), mAllocator(_allocator)
	{
		mpBlockMap = AllocateBlockMap(1);
		mBegin = mEnd = iterator(*mpBlockMap = AllocateBlock(), mpBlockMap);
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline blockmap<T, Allocator, BlockSize>::~blockmap()
	{
		cckit::destroy(mBegin, mEnd);
		if (mEnd.mpCurrent != *(mEnd.mpCurrentBlockPtr)
			|| mBegin == mEnd)
			DeallocateBlock(*(mEnd.mpCurrentBlockPtr));
		for (auto pBlock = mBegin.mpCurrentBlockPtr; pBlock != mEnd.mpCurrentBlockPtr; ++pBlock)
			DeallocateBlock(*pBlock);
		DeallocateBlockMap(mpBlockMap);
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	void blockmap<T, Allocator, BlockSize>::shrink_to_fit()
	{
		auto usedBlockCount = (mBegin == mEnd) ? 1
			: ((mEnd.mpCurrent == *(mEnd.mpCurrentBlockPtr)) ? (mEnd.mpCurrentBlockPtr - mBegin.mpCurrentBlockPtr)
				: (mEnd.mpCurrentBlockPtr - mBegin.mpCurrentBlockPtr + 1));
		auto beginOffsetInBlock = mBegin.mpCurrent - *(mBegin.mpCurrentBlockPtr);
		auto size = mEnd - mBegin;
		
		value_type** pTemp = AllocateBlockMap(mMapSize = usedBlockCount);
		cckit::copy(mBegin.mpCurrentBlockPtr, mBegin.mpCurrentBlockPtr + mMapSize, pTemp);
		DeallocateBlockMap(mpBlockMap);
		mpBlockMap = pTemp;

		mBegin = iterator(*mpBlockMap + beginOffsetInBlock, mpBlockMap);
		mEnd = mBegin + size;
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::insert(const_iterator _pos, const value_type& _val)
	{
		return Insert(_pos, _val);
	}
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::insert(const_iterator _pos, value_type&& _val)
	{
		return Insert(_pos, cckit::move(_val));
	}
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::insert(const_iterator _pos, size_type _count, const value_type& _val)
	{
		iterator insertPos = _pos;
		for (; _count-- > 0; insertPos = Insert(insertPos, _val)) {}
		return insertPos;
	}
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	template<typename InputIterator
		, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::insert(const_iterator _pos, InputIterator _first, InputIterator _last)
	{
		iterator insertPos = _pos;
		for (; _last != _first; insertPos = Insert(insertPos, *(--_last))) {}
		return insertPos;
	}
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::insert(const_iterator _pos, std::initializer_list<value_type> _ilist)
	{
		return insert(_pos, _ilist.begin(), _ilist.end());
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::value_type*
		blockmap<T, Allocator, BlockSize>::AllocateBlock() const
	{
		return static_cast<value_type*>(mAllocator.allocate(sizeof(value_type) * BlockSize));
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::value_type**
		blockmap<T, Allocator, BlockSize>::AllocateBlockMap(size_type _blockCount) const
	{
		value_type** pTemp = static_cast<value_type**>(mAllocator.allocate(sizeof(value_type*) * _blockCount));
		for (size_type i = 0; i < _blockCount; ::new(pTemp + i) value_type*(nullptr), ++i) {}
		return pTemp;
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline void blockmap<T, Allocator, BlockSize>::DeallocateBlock(value_type* _pBlock) const
	{
		if (_pBlock)
			mAllocator.deallocate(_pBlock);
	}
	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline void blockmap<T, Allocator, BlockSize>::DeallocateBlockMap(value_type** _pMap) const
	{
		if (_pMap)
			mAllocator.deallocate(_pMap);
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	inline typename blockmap<T, Allocator, BlockSize>::difference_type
		blockmap<T, Allocator, BlockSize>::Distance(const_iterator _first, const_iterator _last) const
	{
		auto blockDiff = _last.mpCurrentBlockPtr - _first.mpCurrentBlockPtr;
		if (blockDiff == 0)
			return _last.mpCurrent - _first.mpCurrent;
		else {
			return (blockDiff - 1) * BlockSize
				+ (*(_first.mpCurrentBlockPtr) + BlockSize - _first.mpCurrent)
				+ (_last.mpCurrent - *(_last.mpCurrentBlockPtr));
		}
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	template<typename U, typename>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::Insert(const_iterator _pos, U&& _val)
	{
		//size_type index = Distance(mBegin, _pos);
		size_type index = _pos - mBegin;
		if (index < size() / 2)
			return Insert(_pos, cckit::forward<U>(_val), ShiftLeft());
		else
			return Insert(_pos, cckit::forward<U>(_val), ShiftRight());
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	template<typename U, typename>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::Insert(const_iterator _pos, U&& _val, ShiftLeft)
	{
		auto posOffset = _pos - mBegin;

		if (mBegin.mpCurrent == *mpBlockMap) {
			auto size = mEnd - mBegin;
			auto usedBlockCount = (mBegin == mEnd) ? 1
				: ((mEnd.mpCurrent == *(mEnd.mpCurrentBlockPtr)) ? (mEnd.mpCurrentBlockPtr - mBegin.mpCurrentBlockPtr)
					: (mEnd.mpCurrentBlockPtr - mBegin.mpCurrentBlockPtr + 1));
			auto newBeginBlockOffset = ((mMapSize == 1) ? mMapSize : mMapSize / 2);
			auto beginOffsetInBlock = mBegin.mpCurrent - *(mBegin.mpCurrentBlockPtr);

			value_type** pTemp = AllocateBlockMap(mMapSize *= 2);
			cckit::copy(mBegin.mpCurrentBlockPtr, mBegin.mpCurrentBlockPtr + usedBlockCount, pTemp + newBeginBlockOffset);
			DeallocateBlockMap(mpBlockMap);
			mpBlockMap = pTemp;

			mBegin = iterator(*(mpBlockMap + newBeginBlockOffset) + beginOffsetInBlock, mpBlockMap + newBeginBlockOffset);
			*(mBegin.mpCurrentBlockPtr - 1) = AllocateBlock();
			mEnd = mBegin + size;

			cckit::initialized_insert_after_n(mBegin, mBegin + posOffset, cckit::forward<U>(_val));
		}
		else if (mBegin != mEnd && mBegin.mpCurrent == *(mBegin.mpCurrentBlockPtr)) {
			auto size = mEnd - mBegin;

			*(mBegin.mpCurrentBlockPtr - 1) = AllocateBlock();
			cckit::initialized_insert_after_n(mBegin, mBegin + posOffset, cckit::forward<U>(_val));
		}
		else
			cckit::initialized_insert_after_n(cbegin(), _pos, cckit::forward<U>(_val));

		--mBegin;
		return mBegin + posOffset;
	}

	template<typename T, typename Allocator, cckit_size_t BlockSize>
	template<typename U, typename>
	inline typename blockmap<T, Allocator, BlockSize>::iterator
		blockmap<T, Allocator, BlockSize>::Insert(const_iterator _pos, U&& _val, ShiftRight)
	{
		auto posOffset = _pos - mBegin;

		if (mEnd.mpCurrentBlockPtr == mpBlockMap + mMapSize) {
			auto size = mEnd - mBegin;
			auto usedBlockCount = (mBegin == mEnd) ? 1
				: ((mEnd.mpCurrent == *(mEnd.mpCurrentBlockPtr)) ? (mEnd.mpCurrentBlockPtr - mBegin.mpCurrentBlockPtr)
					: (mEnd.mpCurrentBlockPtr - mBegin.mpCurrentBlockPtr + 1));
			auto newBeginBlockOffset = mMapSize / 2;
			auto beginOffsetInBlock = mBegin.mpCurrent - *(mBegin.mpCurrentBlockPtr);

			value_type** pTemp = AllocateBlockMap(mMapSize *= 2);
			cckit::copy(mBegin.mpCurrentBlockPtr, mBegin.mpCurrentBlockPtr + usedBlockCount, pTemp + newBeginBlockOffset);
			DeallocateBlockMap(mpBlockMap);
			mpBlockMap = pTemp;

			mBegin = iterator(*(mpBlockMap + newBeginBlockOffset) + beginOffsetInBlock, mpBlockMap + newBeginBlockOffset);
			*(mBegin.mpCurrentBlockPtr + ((mBegin.mpCurrent != *(mBegin.mpCurrentBlockPtr))
				? (size / BlockSize + 1) : size / BlockSize)) = AllocateBlock();
			mEnd = mBegin + size;

			cckit::initialized_insert_n(mBegin + posOffset, mEnd, cckit::forward<U>(_val));
		}
		else if (mBegin != mEnd && mEnd.mpCurrent == *(mEnd.mpCurrentBlockPtr)) {
			auto size = mEnd - mBegin;

			*(mBegin.mpCurrentBlockPtr + ((mBegin.mpCurrent != *(mBegin.mpCurrentBlockPtr))
				? (size / BlockSize + 1) : size / BlockSize)) = AllocateBlock();
			mEnd = mBegin + size;
			cckit::initialized_insert_n(mBegin + posOffset, mEnd, cckit::forward<U>(_val));
		}
		else
			cckit::initialized_insert_n(_pos, cend(), cckit::forward<U>(_val));

		++mEnd;
		return mBegin + posOffset;
	}
}

#endif // !CCKIT_BLOCKMAP_H