#ifndef CCKIT_LIST_H
#define CCKIT_LIST_H

#include <cstdlib>
#include <cassert>
#include <initializer_list>
#include "internal/config.h"
#include "iterator.h"
#include "utility.h"
#include "type_traits.h"
#include "functional.h"

namespace cckit
{
	// FORWARD DECLARATION
	template<class T>
	class ListNode;
	template<typename T, typename Pointer, typename Reference>
	class ListIterator;
	template<typename T, typename Allocator>
	class ListBase;
	template<typename T, typename Allocator>
	class list;
	
	template<typename T, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
	bool operator== (const ListIterator<T, Pointer0, Reference0>& _lhs, const ListIterator<T, Pointer1, Reference1>& _rhs) CCKIT_NOEXCEPT;
	template<typename T, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
	bool operator!= (const ListIterator<T, Pointer0, Reference0>& _lhs, const ListIterator<T, Pointer1, Reference1>& _rhs) CCKIT_NOEXCEPT;
	//! FORWARD DECLARATION

	template<class T>
	class ListNode
	{
		typedef ListNode<T> this_type;
	public:
		typedef T value_type;

	private:
		explicit ListNode(const value_type& _val = value_type());
		~ListNode() {}
		void Insert(this_type* _pNext) CCKIT_NOEXCEPT;
		void Remove() CCKIT_NOEXCEPT;
		void Splice(this_type* _pFirst, this_type* _pLast) CCKIT_NOEXCEPT;
		void Swap(this_type* _pOther) CCKIT_NOEXCEPT; // BUGGY

	private:
		this_type* mpPrev;
		this_type* mpNext;
		value_type mVal;

		template<typename T, typename Pointer, typename Reference>
		friend class ListIterator;
		template<typename T, typename Allocator>
		friend class ListBase;
		template<typename T, typename Allocator>
		friend class list;
	};// ListNode

	template<typename T, typename Pointer, typename Reference>
	class ListIterator
	{
		typedef ListIterator<T, Pointer, Reference> this_type;
	public:
		typedef ListIterator<T, const T*, const T&> const_iterator;
		typedef size_t size_type;  
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef ListNode<T> node_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef cckit::bidirectional_iterator_tag iterator_category;

	public:
		explicit ListIterator(const node_type* _pNode = nullptr) CCKIT_NOEXCEPT;
		ListIterator(const this_type& _src) CCKIT_NOEXCEPT;

		this_type next() const CCKIT_NOEXCEPT;
		this_type prev() const CCKIT_NOEXCEPT;
		CCKIT_CONSTEXPR reference operator*() const CCKIT_NOEXCEPT;
		CCKIT_CONSTEXPR pointer operator->() const CCKIT_NOEXCEPT;
		this_type& operator++() CCKIT_NOEXCEPT;
		this_type operator++(int) CCKIT_NOEXCEPT;
		this_type& operator--() CCKIT_NOEXCEPT;
		this_type operator--(int) CCKIT_NOEXCEPT;


		operator const_iterator() const;

		template<typename T0, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
		friend bool operator== <T0>(const ListIterator<T0, Pointer0, Reference0>& _lhs, const ListIterator<T0, Pointer1, Reference1>& _rhs) CCKIT_NOEXCEPT;
		template<typename T0, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
		friend bool operator!= <T0>(const ListIterator<T0, Pointer0, Reference0>& _lhs, const ListIterator<T0, Pointer1, Reference1>& _rhs) CCKIT_NOEXCEPT;

	private:
		const node_type* mpNode;

		template<typename T, typename Allocator>
		friend class list;
	};// ListIterator

	template<typename T, typename Allocator>
	class ListBase
	{
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Allocator allocator_type;
		typedef ListNode<T> node_type;

	public:
		const allocator_type& get_allocator() const CCKIT_NOEXCEPT;
		allocator_type& get_allocator() CCKIT_NOEXCEPT;
		void set_allocator(const allocator_type& _allocator);

	protected:
		ListBase();
		ListBase(const allocator_type& _allocator);
		~ListBase();

		node_type* AllocateNode();
		void FreeNode(node_type* _pNode);

		void Init() CCKIT_NOEXCEPT;
		void Clear();

	protected:
		node_type* mpSentinel;
		size_type  mSize;
		allocator_type mAllocator;
	};// ListBase

	template<typename T, typename Allocator = CCKIT_DEFAULT_ALLOCATOR_TYPE>
	class list : public ListBase<T, Allocator>
	{
		typedef ListBase<T, Allocator> base_type;
		typedef list<T, Allocator> this_type;
	public:
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef ListIterator<T, T*, T&> iterator;
		typedef ListIterator<T, const T*, const T&> const_iterator;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::allocator_type allocator_type;
		typedef typename base_type::node_type node_type;
		typedef cckit::reverse_iterator<iterator> reverse_iterator;
		typedef cckit::reverse_iterator<const_iterator> const_reverse_iterator;

		using base_type::mpSentinel;
		using base_type::mSize;
		using base_type::mAllocator;
		using base_type::AllocateNode;
		using base_type::FreeNode;
		using base_type::Clear;
		using base_type::Init;
		using base_type::get_allocator;

	public:
		list();
		list(const allocator_type& _allocator);
		explicit list(size_type _size, const allocator_type& _allocator = allocator_type());
		list(size_type _size, const value_type& _val, const allocator_type& _allocator = allocator_type());
		list(const this_type& _src);
		list(const this_type& _src, const allocator_type& _allocator);
		list(this_type&& _src);
		list(this_type&& _src, const allocator_type& _allocator);

		list(std::initializer_list<value_type> _ilist, const allocator_type& _allocator = allocator_type());
		template <typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
		list(InputIterator _first, InputIterator _last);

		this_type& operator=(const this_type& _rhs);
		this_type& operator=(this_type&& _rhs) CCKIT_NOEXCEPT;
		this_type& operator=(std::initializer_list<value_type> _ilist);

		void assign(size_type _count, const value_type& _val);
		void assign(std::initializer_list<value_type> _ilist);
		template<typename InputIterator
			, typename = typename enable_if<is_iterator<InputIterator>::value>::type>
		void assign(InputIterator _first, InputIterator _last);

		reference       front();
		const_reference front() const;
		reference       back();
		const_reference back() const;

		iterator begin() CCKIT_NOEXCEPT;
		const_iterator begin() const CCKIT_NOEXCEPT;
		const_iterator cbegin() const CCKIT_NOEXCEPT;
		iterator end() CCKIT_NOEXCEPT;
		const_iterator end() const CCKIT_NOEXCEPT;
		const_iterator cend() const CCKIT_NOEXCEPT;
		reverse_iterator rbegin() CCKIT_NOEXCEPT;
		const_reverse_iterator crbegin() const CCKIT_NOEXCEPT;
		reverse_iterator rend() CCKIT_NOEXCEPT;
		const_reverse_iterator crend() const CCKIT_NOEXCEPT;

		bool empty() const CCKIT_NOEXCEPT;
		size_type size() const CCKIT_NOEXCEPT;
		void clear() CCKIT_NOEXCEPT;

		iterator insert(const_iterator _next, const value_type& _val = value_type());
		iterator insert(const_iterator _next, value_type&& _val);
		iterator insert(const_iterator _next, size_type _count, const value_type& _val);
		iterator insert(const_iterator _next, std::initializer_list<value_type> _ilist);
		template<typename InputIterator>
		enable_if_t<is_iterator<InputIterator>::value, iterator> 
			insert(const_iterator _next, InputIterator _first, InputIterator _last);

		template <typename... Args>
		iterator emplace(const_iterator _next, Args&&... _args);

		iterator erase(const_iterator _pos);
		iterator erase(const_iterator _first, const_iterator _last);

		void push_back(const value_type& _val);
		void push_back(value_type&& _val);
		template<typename... Args>
		reference emplace_back(Args&&... _args);
		void pop_back();

		void push_front(const value_type& _val);
		void push_front(value_type&& _val);
		template<typename... Args>
		reference emplace_front(Args&&... _args);
		void pop_front();

		void resize(size_type _count, const value_type& _val = value_type());

		void swap(this_type& _other) CCKIT_NOEXCEPT;

		void merge(this_type& _src);
		void merge(this_type&& _src);
		template<typename Compare>
		void merge(this_type& _src, Compare _compare);
		template<typename Compare>
		void merge(this_type&& _src, Compare _compare);

		void splice(const_iterator _next, this_type& _src);
		void splice(const_iterator _next, this_type&& _src);
		void splice(const_iterator _next, this_type& _src, const_iterator _target);
		void splice(const_iterator _next, this_type&& _src, const_iterator _target);
		void splice(const_iterator _next, this_type& _src, const_iterator _first, const_iterator _last);
		void splice(const_iterator _next, this_type&& _src, const_iterator _first, const_iterator _last);
		void splice(const_iterator _next, this_type& _src, size_type _index0, size_type _index1);
		void splice(const_iterator _next, this_type&& _src, size_type _index0, size_type _index1);

		void remove(const value_type& _val);
		template<typename UnaryPredicate>
		size_type remove_if(UnaryPredicate _pred);

		void reverse();

		void unique();
		template<typename BinaryPredicate>
		void unique(BinaryPredicate _pred);

		void sort();
		template<typename Compare>
		void sort(Compare _compare);

		// ADDITIONAL OPERATION
		reference operator[](size_type _index);
		const_reference operator[](size_type _index) const;
		iterator at(size_type _index);
		const_iterator at(size_type _index) const;
		this_type& absorb(this_type& _src);
		this_type& take_first(this_type& _src);
		this_type truncate(size_type _index);
		//! ADDITIONAL OPERATION

	private:
		void Assign(size_type _count, const value_type& _val);
		template <typename InputIterator
			, typename = enable_if_t<is_iterator<InputIterator>::value> >
		void Assign(InputIterator _first, InputIterator _last);

		node_type* Insert(node_type* _pNext, size_type _count, const value_type& _val);
		template <typename InputIterator
			, typename = enable_if_t<is_iterator<InputIterator>::value> >
		node_type* Insert(node_type* _pNext, InputIterator _first, InputIterator _last);
		template<typename... Args>
		node_type* InsertValue(node_type* _pNext, Args&&... _args);

		node_type* Erase(node_type* _pNode);

		template<typename... Args>
		node_type* CreateNode(Args&&... _args);
		
		node_type* At(size_type _index) CCKIT_NOEXCEPT;
		const node_type* At(size_type _index) const CCKIT_NOEXCEPT;
	};// list
}// namespace cckit

namespace cckit
{
	// ListNode<T>
#pragma region ListNode<T>::ListNode
	template<typename T>
	inline ListNode<T>::ListNode(const value_type& _val)
		: mVal(_val)
	{}
#pragma endregion ListNode<T>::ListNode

#pragma region ListNode<T>::Insert
	template<class T>
	inline void ListNode<T>::Insert(this_type* _pNext) CCKIT_NOEXCEPT
	{
		this->mpPrev = _pNext->mpPrev;
		_pNext->mpPrev->mpNext = this;
		this->mpNext = _pNext;
		_pNext->mpPrev = this;
	}
#pragma endregion ListNode<T>::Insert

#pragma region ListNode<T>::Remove
	template<class T>
	inline void ListNode<T>::Remove() CCKIT_NOEXCEPT
	{
		mpPrev->mpNext = mpNext;
		mpNext->mpPrev = mpPrev;
	}
#pragma endregion ListNode<T>::Remove

#pragma region ListNode<T>::Splice
	template<class T>
	inline void ListNode<T>::Splice(this_type* _pFirst, this_type* _pLast) CCKIT_NOEXCEPT
	{
		this_type* pTemp = _pLast->mpPrev;

		_pFirst->mpPrev->mpNext = _pLast;
		_pLast->mpPrev = _pFirst->mpPrev;

		this->mpPrev->mpNext = _pFirst;
		_pFirst->mpPrev = this->mpPrev;
		this->mpPrev = pTemp;
		pTemp->mpNext = this;
	}
#pragma endregion ListNode<T>::Splice

#pragma region ListNode<T>::Swap
	template<class T>
	inline void ListNode<T>::Swap(this_type* _pOther) CCKIT_NOEXCEPT
	{
		this_type *pTemp0 = this->mpPrev, *pTemp1 = this->mpNext;
		cckit::swap(this->mVal, _pOther->mVal);

		_pOther->mpPrev->mpNext = this;
		this->mpPrev = _pOther->mpPrev;
		_pOther->mpNext->mpPrev = this;
		this->mpNext = _pOther->mpNext;

		pTemp0->mpNext = _pOther;
		_pOther->mpPrev = pTemp0;
		pTemp1->mpPrev = _pOther;
		_pOther->mpNext = pTemp1;
	}
#pragma endregion ListNode<T>::Swap
	//! ListNode<T>

	// ListIterator
#pragma region ListIterator<T, Pointer, Reference>::ListIterator
	template <typename T, typename Pointer, typename Reference>
	inline ListIterator<T, Pointer, Reference>::ListIterator(const node_type* _pNode) CCKIT_NOEXCEPT
		: mpNode(_pNode) 
	{}
	template <typename T, typename Pointer, typename Reference>
	inline ListIterator<T, Pointer, Reference>::ListIterator(const this_type& _src) CCKIT_NOEXCEPT
		: mpNode(_src.mpNode)
	{}
#pragma endregion ListIterator<T, Pointer, Reference>::ListIterator

#pragma region ListIterator<T, Pointer, Reference>::next
	template <typename T, typename Pointer, typename Reference>
	inline typename ListIterator<T, Pointer, Reference>::this_type
		ListIterator<T, Pointer, Reference>::next() const CCKIT_NOEXCEPT
	{
		return this_type(mpNode->mpNext);
	}
#pragma endregion ListIterator<T, Pointer, Reference>::next

#pragma region ListIterator<T, Pointer, Reference>::prev
	template <typename T, typename Pointer, typename Reference>
	inline typename ListIterator<T, Pointer, Reference>::this_type
		ListIterator<T, Pointer, Reference>::prev() const CCKIT_NOEXCEPT
	{
		return this_type(mpNode->mpPrev);
	}
#pragma endregion ListIterator<T, Pointer, Reference>::prev

#pragma region ListIterator<T, Pointer, Reference>::operator*
	template <typename T, typename Pointer, typename Reference>
	inline CCKIT_CONSTEXPR typename ListIterator<T, Pointer, Reference>::reference
		ListIterator<T, Pointer, Reference>::operator*() const CCKIT_NOEXCEPT
	{
		return const_cast<reference>(mpNode->mVal);
	}
#pragma endregion ListIterator<T, Pointer, Reference>::operator*

#pragma region ListIterator<T, Pointer, Reference>::operator->
	template <typename T, typename Pointer, typename Reference>
	inline CCKIT_CONSTEXPR typename ListIterator<T, Pointer, Reference>::pointer
		ListIterator<T, Pointer, Reference>::operator->() const CCKIT_NOEXCEPT
	{
		return &(**this);
	}
#pragma endregion ListIterator<T, Pointer, Reference>::operator->

#pragma region ListIterator<T, Pointer, Reference>::operator++
	template <typename T, typename Pointer, typename Reference>
	inline typename ListIterator<T, Pointer, Reference>::this_type&
		ListIterator<T, Pointer, Reference>::operator++() CCKIT_NOEXCEPT
	{
		mpNode = mpNode->mpNext;
		return *this;
	}
	template <typename T, typename Pointer, typename Reference>
	inline typename ListIterator<T, Pointer, Reference>::this_type
		ListIterator<T, Pointer, Reference>::operator++(int) CCKIT_NOEXCEPT
	{
		this_type temp(*this);
		mpNode = mpNode->mpNext;
		return temp;
	}
#pragma endregion ListIterator<T, Pointer, Reference>::operator++

#pragma region ListIterator<T, Pointer, Reference>::operator--
	template <typename T, typename Pointer, typename Reference>
	inline typename ListIterator<T, Pointer, Reference>::this_type&
		ListIterator<T, Pointer, Reference>::operator--() CCKIT_NOEXCEPT
	{
		mpNode = mpNode->mpPrev;
		return *this;
	}
	template <typename T, typename Pointer, typename Reference>
	inline typename ListIterator<T, Pointer, Reference>::this_type
		ListIterator<T, Pointer, Reference>::operator--(int) CCKIT_NOEXCEPT
	{
		this_type temp(*this);
		mpNode = mpNode->mpPrev;
		return temp;
	}
#pragma endregion ListIterator<T, Pointer, Reference>::operator--

#pragma region ListIterator<T, Pointer, Reference>::operator_const_iterator
	template <typename T, typename Pointer, typename Reference>
	ListIterator<T, Pointer, Reference>::operator const_iterator() const
	{
		return const_iterator(mpNode);
	}
#pragma endregion ListIterator<T, Pointer, Reference>::operator_const_iterator

	template<typename T, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
	bool operator== (const ListIterator<T, Pointer0, Reference0>& _lhs, const ListIterator<T, Pointer1, Reference1>& _rhs) CCKIT_NOEXCEPT
	{
		return _lhs.mpNode == _rhs.mpNode;
	}
	template<typename T, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
	bool operator!= (const ListIterator<T, Pointer0, Reference0>& _lhs, const ListIterator<T, Pointer1, Reference1>& _rhs) CCKIT_NOEXCEPT
	{
		return _lhs.mpNode != _rhs.mpNode;
	}
	//! ListIterator

	// ListBase<T, Allocator>
#pragma region ListBase<T, Allocator>::get_allocator
	template<typename T, typename Allocator>
	inline const typename ListBase<T, Allocator>::allocator_type& 
		ListBase<T, Allocator>::get_allocator() const CCKIT_NOEXCEPT
	{
		return mAllocator;
	}
	template<typename T, typename Allocator>
	inline typename ListBase<T, Allocator>::allocator_type&
		ListBase<T, Allocator>::get_allocator() CCKIT_NOEXCEPT
	{
		return mAllocator;
	}
#pragma endregion ListBase<T, Allocator>::get_allocator

#pragma region ListBase<T, Allocator>::set_allocator
	template<typename T, typename Allocator>
	inline void ListBase<T, Allocator>::set_allocator(const allocator_type& _allocator)
	{
		mAllocator = _allocator;
	}
#pragma endregion ListBase<T, Allocator>::set_allocator

#pragma region ListBase<T, Allocator>::ListBase
	template<typename T, typename Allocator>
	inline ListBase<T, Allocator>::ListBase()
		: mSize(0)
		, mAllocator()
	{
		mpSentinel = AllocateNode();
		Init();
	}
	template<typename T, typename Allocator>
	inline ListBase<T, Allocator>::ListBase(const allocator_type& _allocator)
		: mSize(0)
		, mAllocator(_allocator)
	{
		mpSentinel = AllocateNode();
		Init();
	}
#pragma endregion ListBase<T, Allocator>::ListBase
	
#pragma region ListBase<T, Allocator>::~ListBase
	template<typename T, typename Allocator>
	inline ListBase<T, Allocator>::~ListBase()
	{
		Clear();
		FreeNode(mpSentinel);
	}
#pragma endregion ListBase<T, Allocator>::~ListBase

#pragma region ListBase<T, Allocator>::AllocateNode
	template<typename T, typename Allocator>
	inline typename ListBase<T, Allocator>::node_type* 
		ListBase<T, Allocator>::AllocateNode()
	{
		return new(mAllocator.allocate(sizeof(node_type))) node_type;
	}
#pragma endregion ListBase<T, Allocator>::AllocateNode

#pragma region ListBase<T, Allocator>::FreeNode
	template<typename T, typename Allocator>
	inline void ListBase<T, Allocator>::FreeNode(node_type* _pNode)
	{
		_pNode->~node_type();
		mAllocator.deallocate(_pNode);
	}
#pragma endregion ListBase<T, Allocator>::FreeNode

#pragma region ListBase<T, Allocator>::Init
	template<typename T, typename Allocator>
	inline void ListBase<T, Allocator>::Init() CCKIT_NOEXCEPT
	{
		mpSentinel->mpPrev = mpSentinel;
		mpSentinel->mpNext = mpSentinel;
	}
#pragma endregion ListBase<T, Allocator>::Init

#pragma region ListBase<T, Allocator>::Clear
	template<typename T, typename Allocator>
	inline void ListBase<T, Allocator>::Clear()
	{
		node_type *pCurrent = mpSentinel->mpNext, *pTemp;
		while (pCurrent != mpSentinel) {
			pTemp = pCurrent;
			pCurrent = pCurrent->mpNext;
			FreeNode(pTemp);
		}
		mSize = 0;

		Init();
	}
#pragma endregion ListBase<T, Allocator>::Clear
	//! ListBase<T, Allocator>

	// list<T, Allocator>
#pragma region list<T, Allocator>::list
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list()
		: base_type()
	{}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(const allocator_type& _allocator)
		: base_type(_allocator)
	{}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(size_type _size, const allocator_type& _allocator)
		: base_type(_allocator)
	{
		Insert(mpSentinel, _size, value_type());
	}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(size_type _size, const value_type& _val, const allocator_type& _allocator)
		: base_type(_allocator)
	{
		Insert(mpSentinel, _size, _val);
	}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(const this_type& _src)
		: base_type(_src.mAllocator)
	{
		Insert(mpSentinel, _src.cbegin(), _src.cend());
	}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(const this_type& _src, const allocator_type& _allocator)
		: base_type(_allocator)
	{
		Insert(mpSentinel, _src.cbegin(), _src.cend());
	}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(this_type&& _src)
		: base_type(_src.mAllocator)
	{
		swap(_src);
	}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(this_type&& _src, const allocator_type& _allocator)
		: base_type(_allocator)
	{
		swap(_src);
	}
	template<typename T, typename Allocator>
	inline list<T, Allocator>::list(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
		: base_type(_allocator)
	{
		Insert(mpSentinel, _ilist.begin(), _ilist.end());
	}
	template<typename T, typename Allocator>
	template<typename InputIterator 
		, typename = typename enable_if_t<is_iterator<InputIterator>::value> >
	inline list<T, Allocator>::list(InputIterator _first, InputIterator _last)
		: base_type()
	{
		Insert(mpSentinel, _first, _last);
	}
#pragma endregion list<T, Allocator>::list

#pragma region list<T, Allocator>::operator=
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::this_type& 
		list<T, Allocator>::operator=(const this_type& _rhs)
	{
		if (this != &_rhs)
			Assign(_rhs.cbegin(), _rhs.cend());
		return *this;
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::this_type&
		list<T, Allocator>::operator=(this_type&& _rhs) CCKIT_NOEXCEPT
	{
		if (this != &_rhs)
			swap(_rhs);
		return *this;
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::this_type&
		list<T, Allocator>::operator=(std::initializer_list<value_type> _ilist)
	{
		Assign(_ilist.begin(), _ilist.end());
		return *this;
	}
#pragma endregion list<T, Allocator>::operator=

#pragma region list<T, Allocator>::assign
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::assign(size_type _count, const value_type& _val)
	{
		Assign(_count, _val);
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::assign(std::initializer_list<value_type> _ilist)
	{
		Assign(_ilist.begin(), _ilist.end());
	}
	template<typename T, typename Allocator>
	template<typename InputIterator
		, typename = typename enable_if<is_iterator<InputIterator>::value>::type>
		inline void list<T, Allocator>::assign(InputIterator _first, InputIterator _last)
	{
		Assign(_first, _last);
	}
#pragma endregion list<T, Allocator>::assign

#pragma region list<T, Allocator>::front
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::reference 
		list<T, Allocator>::front()
	{
		assert(!empty()); 
		return (*this)[0];
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_reference 
		list<T, Allocator>::front() const
	{
		assert(!empty());
		return (*this)[0];
	}
#pragma endregion list<T, Allocator>::front

#pragma region list<T, Allocator>::back
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::reference 
		list<T, Allocator>::back()
	{
		assert(!empty());
		return (*this)[mSize - 1];
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_reference 
		list<T, Allocator>::back() const
	{
		assert(!empty());
		return (*this)[mSize - 1];
	}
#pragma endregion list<T, Allocator>::back

#pragma region list<T, Allocator>::ITERATOR
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::begin() CCKIT_NOEXCEPT
	{
		return iterator(mpSentinel->mpNext);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_iterator
		list<T, Allocator>::begin() const CCKIT_NOEXCEPT
	{
		return const_iterator(mpSentinel->mpNext);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_iterator 
		list<T, Allocator>::cbegin() const CCKIT_NOEXCEPT
	{
		return const_iterator(mpSentinel->mpNext);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::end() CCKIT_NOEXCEPT
	{
		return iterator(mpSentinel);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_iterator
		list<T, Allocator>::end() const CCKIT_NOEXCEPT
	{
		return const_iterator(mpSentinel);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_iterator 
		list<T, Allocator>::cend() const CCKIT_NOEXCEPT
	{
		return const_iterator(mpSentinel);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::reverse_iterator 
		list<T, Allocator>::rbegin() CCKIT_NOEXCEPT
	{
		return reverse_iterator(end());
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_reverse_iterator 
		list<T, Allocator>::crbegin() const CCKIT_NOEXCEPT
	{
		return const_reverse_iterator(cend());
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::reverse_iterator 
		list<T, Allocator>::rend() CCKIT_NOEXCEPT
	{
		return reverse_iterator(begin());
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_reverse_iterator 
		list<T, Allocator>::crend() const CCKIT_NOEXCEPT
	{
		return const_reverse_iterator(cbegin());
	}
#pragma endregion list<T, Allocator>::ITERATOR

#pragma region list<T, Allocator>::empty
	template<typename T, typename Allocator>
	inline bool list<T, Allocator>::empty() const CCKIT_NOEXCEPT
	{
		return mSize == 0;
	}
#pragma endregion list<T, Allocator>::empty

#pragma region list<T, Allocator>::size
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::size_type 
		list<T, Allocator>::size() const CCKIT_NOEXCEPT
	{
		return mSize;
	}
#pragma endregion list<T, Allocator>::size

#pragma region list<T, Allocator>::clear
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::clear() CCKIT_NOEXCEPT
	{
		Clear();
	}
#pragma endregion list<T, Allocator>::clear

#pragma region list<T, Allocator>::insert
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator 
		list<T, Allocator>::insert(const_iterator _next, const value_type& _val)
	{
		node_type* pFirst = InsertValue(const_cast<node_type*>(_next.mpNode), _val);
		return iterator(pFirst);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::insert(const_iterator _next, value_type&& _val)
	{
		node_type* pFirst = InsertValue(const_cast<node_type*>(_next.mpNode), cckit::move(_val));
		return iterator(pFirst);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::insert(const_iterator _next, size_type _count, const value_type& _val)
	{
		node_type* pFirst = Insert(const_cast<node_type*>(_next.mpNode), _count, _val);
		return iterator(pFirst);
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::insert(const_iterator _next, std::initializer_list<value_type> _ilist)
	{
		node_type* pFirst = Insert(const_cast<node_type*>(_next.mpNode), _ilist.begin(), _ilist.end());
		return iterator(pFirst);
	}
	template<typename T, typename Allocator>
	template<typename InputIterator>
	inline enable_if_t<is_iterator<InputIterator>::value, typename list<T, Allocator>::iterator>
		list<T, Allocator>::insert(const_iterator _next, InputIterator _first, InputIterator _last)
	{
		node_type* pFirst = Insert(const_cast<node_type*>(_next.mpNode), _first, _last);
		return iterator(pFirst);
	}
#pragma endregion list<T, Allocator>::insert

#pragma region list<T, Allocator>::emplace
	template<typename T, typename Allocator>
	template <typename... Args>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::emplace(const_iterator _next, Args&&... _args)
	{
		InsertValue(const_cast<node_type*>(_next.mpNode), cckit::forward<Args>(_args)...);
		return iterator(_next.mpNode->mpPrev);
	}
#pragma endregion list<T, Allocator>::emplace

#pragma region list<T, Allocator>::erase
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::erase(const_iterator _pos)
	{
		return iterator(Erase(const_cast<node_type*>(_pos.mpNode)));
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator
		list<T, Allocator>::erase(const_iterator _first, const_iterator _last)
	{
		while (_first != _last)
			_first = erase(_first);
		return iterator(_last.mpNode);
	}
#pragma endregion list<T, Allocator>::erase

#pragma region list<T, Allocator>::push_back
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::push_back(const value_type& _val)
	{
		insert(cend(), _val);
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::push_back(value_type&& _val)
	{
		insert(cend(), cckit::move(_val));
	}
#pragma endregion list<T, Allocator>::push_back

#pragma region list<T, Allocator>::emplace_back
	template<typename T, typename Allocator>
	template<typename... Args>
	inline typename list<T, Allocator>::reference
		list<T, Allocator>::emplace_back(Args&&... _args)
	{
		iterator temp = emplace(cend(), cckit::forward<Args>(_args)...);
		return const_cast<reference>(temp.mpNode->mVal);
	}
#pragma endregion list<T, Allocator>::emplace_back

#pragma region list<T, Allocator>::pop_back
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::pop_back()
	{
		erase(--end());
	}
#pragma endregion list<T, Allocator>::pop_back

#pragma region list<T, Allocator>::push_front
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::push_front(const value_type& _val)
	{
		insert(cbegin(), _val);
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::push_front(value_type&& _val)
	{
		insert(cbegin(), cckit::move(_val));
	}
#pragma endregion list<T, Allocator>::push_front

#pragma region list<T, Allocator>::emplace_front
	template<typename T, typename Allocator>
	template<typename... Args>
	inline typename list<T, Allocator>::reference 
		list<T, Allocator>::emplace_front(Args&&... _args)
	{
		iterator temp = emplace(cbegin(), cckit::forward<Args>(_args)...);
		return const_cast<reference>(temp.mpNode->mVal);
	}
#pragma endregion list<T, Allocator>::emplace_front

#pragma region list<T, Allocator>::pop_front
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::pop_front()
	{
		erase(cbegin());
	}
#pragma endregion list<T, Allocator>::pop_front

	template<typename T, typename Allocator>
	inline void list<T, Allocator>::resize(size_type _count, const value_type& _val)
	{
		if (mSize < _count) 
			insert(cend(), _count - mSize, _val);
		else if (_count < mSize) {
			while (_count < mSize)
				erase(--end());
		}
	}
	
#pragma region list<T, Allocator>::swap
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::swap(this_type& _other) CCKIT_NOEXCEPT
	{
		cckit::swap(this->mpSentinel, _other.mpSentinel);
		cckit::swap(this->mSize, _other.mSize);
	}
#pragma endregion list<T, Allocator>::swap

#pragma region list<T, Allocator>::merge
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::merge(this_type& _src)
	{
		merge(_src, cckit::less<T>());
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::merge(this_type&& _src)
	{
		merge(_src);
	}
	template<typename T, typename Allocator>
	template<typename Compare>
	inline void list<T, Allocator>::merge(this_type& _src, Compare _compare)
	{
		this_type temp;

		while (this->mSize > 0 && _src.mSize > 0)
		{
			if (_compare(this->front(), _src.front()))
				temp.take_first(*this);
			else
				temp.take_first(_src);
		}
		while (this->mSize > 0)
			temp.take_first(*this);
		while (_src.mSize > 0)
			temp.take_first(_src);

		swap(temp);
	}
	template<typename T, typename Allocator>
	template<typename Compare>
	inline void list<T, Allocator>::merge(this_type&& _src, Compare _compare)
	{
		merge(_src, _compare);
	}
#pragma endregion list<T, Allocator>::merge

#pragma region list<T, Allocator>::splice
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type& _src)
	{
		if (this != &_src) {
			const_cast<node_type*>(_next.mpNode)->Splice(_src.mpSentinel->mpNext, _src.mpSentinel);
			mSize += _src.mSize;
			_src.mSize = 0;
		}
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type&& _src)
	{
		splice(_next, _src);
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type& _src, const_iterator _target)
	{
		if (this != &_src) {
			iterator target0(_target.mpNode);
			++target0;
			const_cast<node_type*>(_next.mpNode)->Splice(const_cast<node_type*>(_target.mpNode), const_cast<node_type*>(target0.mpNode));
			++mSize;
			--_src.mSize;
		}
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type&& _src, const_iterator _target)
	{
		splice(_next, _src, _target);
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type& _src, const_iterator _first, const_iterator _last)
	{
		if (this != &_src) {
			size_type distance = cckit::distance(_first, _last);
			const_cast<node_type*>(_next.mpNode)->Splice(const_cast<node_type*>(_first.mpNode), const_cast<node_type*>(_last.mpNode));
			mSize += distance;
			_src.mSize -= distance;
		}
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type&& _src, const_iterator _first, const_iterator _last)
	{
		splice(_next, _src, _first, _last);
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type& _src, size_type _index0, size_type _index1)
	{
	#if CCKIT_DEBUG
		assert(!_src.empty());
		assert(_index0 < _src.size() && _index1 <= _src.size());
		assert(_index0 < _index1);
	#endif
		if (this != &_src) {
			size_type distance = _index1 - _index0;
			const_cast<node_type*>(_next.mpNode)->Splice(_src.At(_index0), _src.At(_index1));
			mSize += distance;
			_src.mSize -= distance;
		}
	}
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::splice(const_iterator _next, this_type&& _src, size_type _index0, size_type _index1)
	{
		splice(_next, _src, _index0, _index1);
	}
#pragma endregion list<T, Allocator>::splice

#pragma region list<T, Allocator>::remove
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::remove(const value_type& _val)
	{
		remove_if([&_val](const_reference _arg) {
			return _val == _arg;
		});
	}
#pragma endregion list<T, Allocator>::remove

#pragma region list<T, Allocator>::remove_if
	template<typename T, typename Allocator>
	template<typename UnaryPredicate>
	inline typename list<T, Allocator>::size_type
		list<T, Allocator>::remove_if(UnaryPredicate _pred)
	{
		size_type count = 0;

		iterator last = end();
		for (iterator current = begin(); current != last;) {
			iterator prev = current;
			++current;
			if (_pred(*prev)) {
				erase(prev);
				++count;
			}
		}
		return count;
	}
#pragma endregion list<T, Allocator>::remove_if

#pragma region list<T, Allocator>::reverse
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::reverse()
	{
		this_type temp;
		while (!empty()) {
			value_type val = cckit::move(back());
			temp.push_back(cckit::move(val));
			pop_back();
		}
		absorb(temp);
	}
#pragma endregion list<T, Allocator>::reverse

#pragma region list<T, Allocator>::unique
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::unique()
	{
		unique([](const_reference _arg0, const_reference _arg1) {
			return _arg0 == _arg1;
		});
	}
	template <typename T, typename Allocator>
	template <typename BinaryPredicate>
	void list<T, Allocator>::unique(BinaryPredicate _pred)
	{
		iterator first = begin(), last = end();
		if (first != last) {
			iterator next = first;
			while (++next != last) {
				if (_pred(*first, *next))
					erase(next);
				else
					first = next;
				next = first;
			}
		}
	}
#pragma endregion list<T, Allocator>::unique

#pragma region list<T, Allocator>::sort
	template<typename T, typename Allocator>
	inline void list<T, Allocator>::sort()
	{
		sort(cckit::less<T>());
	}
	template<typename T, typename Allocator>
	template<typename Compare>
	void list<T, Allocator>::sort(Compare _compare)
	{
		if (this->mSize > 1) {
			this_type half1, half2;

			half1.absorb(*this);
			half2.absorb(half1.truncate(half1.mSize / 2));
			half1.sort(_compare);
			half2.sort(_compare);

			half1.merge(half2, _compare);
			absorb(half1);
		}
	}
#pragma endregion list<T, Allocator>::sort

#pragma region list<T, Allocator>::operator[]
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::reference 
		list<T, Allocator>::operator[](size_type _index)
	{
	#if CCKIT_DEBUG
		assert(!empty());
		assert(_index < size());
	#endif
		return At(_index)->mVal;
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_reference
		list<T, Allocator>::operator[](size_type _index) const
	{
	#if CCKIT_DEBUG
		assert(!empty());
		assert(_index < size());
	#endif
		return At(_index)->mVal;
	}
#pragma endregion list<T, Allocator>::operator[]

#pragma region list<T, Allocator>::at
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::iterator 
		list<T, Allocator>::at(size_type _index)
	{
	#if CCKIT_DEBUG
		assert(!empty());
		assert(_index < size());
	#endif
		return iterator(At(_index));
	}
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::const_iterator 
		list<T, Allocator>::at(size_type _index) const
	{
	#if CCKIT_DEBUG
		assert(!empty());
		assert(_index < size());
	#endif
		return const_iterator(At(_index));
	}
#pragma endregion list<T, Allocator>::at
	
#pragma region list<T, Allocator>::absorb
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::this_type&
		list<T, Allocator>::absorb(this_type& _src)
	{
		if (!_src.empty())
			splice(cend(), _src, 0, _src.mSize);
		return *this;
	}
#pragma endregion list<T, Allocator>::absorb

#pragma region list<T, Allocator>::take_first
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::this_type&
		list<T, Allocator>::take_first(this_type& _src)
	{
		if (!_src.empty())
			splice(cend(), _src, 0, 1);
		return *this;
	}
#pragma endregion list<T, Allocator>::take_first

#pragma region list<T, Allocator>::truncate
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::this_type 
		list<T, Allocator>::truncate(size_type _index)
	{
	#if CCKIT_DEBUG
		assert(!empty());
		assert(_index > 0 && _index <= size());
	#endif
		this_type remainder;
		remainder.splice(remainder.cend(), *this, 0, _index);
		swap(remainder);
		return remainder;
	}
#pragma endregion list<T, Allocator>::truncate

#pragma region list<T, Allocator>::Assign
	template <typename T, typename Allocator>
	void list<T, Allocator>::Assign(size_type _count, const value_type& _val)
	{
		node_type* pNode = mpSentinel->mpNext;
		for (; (pNode != mpSentinel) && (_count > 0); --_count) {
			pNode->mVal = _val;
			pNode = pNode->mpNext;
		}

		if (pNode == mpSentinel)
			Insert(pNode, _count, _val);
		else
			erase(const_iterator(pNode), cend());
	}
	template <typename T, typename Allocator>
	template <typename InputIterator
		, typename = enable_if_t<is_iterator<InputIterator>::value> >
		void list<T, Allocator>::Assign(InputIterator _first, InputIterator _last)
	{
		node_type* pNode = mpSentinel->mpNext;
		for (; (pNode != mpSentinel) && (_first != _last); ++_first) {
			pNode->mVal = *_first;
			pNode = pNode->mpNext;
		}

		if (pNode != mpSentinel)
			erase(const_iterator(pNode), cend());
		else
			Insert(pNode, _first, _last);
	}
#pragma endregion list<T, Allocator>::Assign

#pragma region list<T, Allocator>::Insert
	template<typename T, typename Allocator>
	inline typename list<T, Allocator>::node_type*
		list<T, Allocator>::Insert(node_type* _pNext, size_type _count, const value_type& _val)
	{
		node_type* pTemp = _pNext->mpPrev;
		for (; _count > 0; --_count)
			InsertValue(_pNext, _val);
		return pTemp->mpNext;
	}
	template<typename T, typename Allocator>
	template <typename InputIterator
		, typename = enable_if_t<is_iterator<InputIterator>::value> >
	inline typename list<T, Allocator>::node_type*
		list<T, Allocator>::Insert(node_type* _pNext, InputIterator _first, InputIterator _last)
	{
		node_type* pTemp = _pNext->mpPrev;
		for (; _first != _last; ++_first)
			InsertValue(_pNext, *_first);
		return pTemp->mpNext;
	}
#pragma endregion list<T, Allocator>::Insert

#pragma region list<T, Allocator>::InsertValue
	template<typename T, typename Allocator>
	template<typename... Args>
	inline typename list<T, Allocator>::node_type*
		list<T, Allocator>::InsertValue(node_type* _pNext, Args&&... _args)
	{
		node_type* pNew = CreateNode(cckit::forward<Args>(_args)...);
		pNew->Insert(_pNext);
		++mSize;
		return pNew;
	}
#pragma endregion list<T, Allocator>::InsertValue

#pragma region list<T, Allocator>::Erase
	template <typename T, typename Allocator>
	inline typename list<T, Allocator>::node_type* 
		list<T, Allocator>::Erase(node_type* _pNode)
	{
		node_type* pEnd = _pNode->mpNext;
		_pNode->Remove();
		FreeNode(_pNode);
		--mSize;
		return pEnd;
	}
#pragma endregion list<T, Allocator>::Erase
	
#pragma region list<T, Allocator>::CreateNode
	template<typename T, typename Allocator>
	template<typename... Args>
	inline typename list<T, Allocator>::node_type* 
		list<T, Allocator>::CreateNode(Args&&... _args)
	{
		node_type* pNode = AllocateNode();
		pNode->mVal = cckit::move(value_type(cckit::forward<Args>(_args)...));
		//::new(&pNode->mVal) value_type(cckit::forward<Args>(_args)...);
		return pNode;
	}
#pragma endregion list<T, Allocator>::CreateNode

#pragma region list<T, Allocator>::At
	template <typename T, typename Allocator>
	inline typename list<T, Allocator>::node_type*
		list<T, Allocator>::At(size_type _index) CCKIT_NOEXCEPT
	{
		if (_index == mSize) return mpSentinel;

		node_type* pCurrent = mpSentinel;
		if (_index < mSize / 2)
			for (size_type i = 0; i <= _index; ++i)
				pCurrent = pCurrent->mpNext;
		else
			for (size_type i = mSize; i > _index; --i)
				pCurrent = pCurrent->mpPrev;
		return pCurrent;
	}
	template <typename T, typename Allocator>
	inline const typename list<T, Allocator>::node_type*
		list<T, Allocator>::At(size_type _index) const CCKIT_NOEXCEPT
	{
		return const_cast<this_type*>(this)->At(_index);
	}
#pragma endregion list<T, Allocator>::At
	//! list<T, Allocator>

}// namespace cckit

#endif // !CCKIT_LIST_H