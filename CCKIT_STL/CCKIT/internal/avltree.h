#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <cstdlib>
#include <cassert>
#include <initializer_list>
#include "config.h"
#include "../iterator.h"
#include "../utility.h"
#include "../type_traits.h"
#include "../functional.h"
#include "../algorithm.h"

namespace cckit
{
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	class avltree;

	template<typename Key, typename T, typename ExtractKey>
	struct avltree_node_base;
	template<typename Key>
	struct avltree_node_base<Key, Key, use_self<Key> > { typedef Key value_type; };
	template<typename Key, typename T>
	struct avltree_node_base<Key, T, use_first<pair<const Key, T> > > { typedef pair<const Key, T> value_type; };

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	struct avltree_node : public avltree_node_base<Key, T, ExtractKey>
	{
		typedef avltree_node<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys> this_type;
		typedef avltree_node_base<Key, T, ExtractKey> base_type;
		typedef avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys> owner_type;
		typedef typename base_type::value_type value_type;
		typedef T mapped_type;

		this_type* mpLeft;
		this_type* mpRight;
		this_type* mpParent;
		this_type* mpNext;
		this_type* mpPrev;
		const owner_type** mppOwner;
		int mHeight;
		value_type mVal;

		avltree_node() 
			: mpLeft(nullptr), mpRight(nullptr), mpParent(nullptr), mpNext(nullptr), mpPrev(nullptr), mppOwner(nullptr), mHeight(-2), mVal() {}
		explicit avltree_node(const value_type& _val)
			: mpLeft(nullptr), mpRight(nullptr), mpParent(nullptr), mpNext(nullptr), mpPrev(nullptr), mppOwner(nullptr), mHeight(-2), mVal(_val) {}
		avltree_node(const this_type&) = delete;

		void SetLeft(this_type* _pOther) { if (mpLeft = _pOther) { mpLeft->mpParent = this; } }
		void SetRight(this_type* _pOther) { if (mpRight = _pOther) { mpRight->mpParent = this; } }
		void SetNext(this_type* _pOther) { if (mpNext = _pOther) { mpNext->mpPrev = this; } }
		void Clone(const this_type& _other, bool _bOnInception = true);// invokable on all nodes except the sentinel
	};

	template<typename Node, typename Pointer, typename Reference>
	struct avltree_iterator : public iterator<cckit::bidirectional_iterator_tag, typename Node::value_type>
	{
		typedef avltree_iterator<Node, Pointer, Reference> this_type;
		typedef iterator<cckit::bidirectional_iterator_tag, typename Node::value_type> base_type;
		typedef Node node;
		typedef avltree_iterator<Node, const typename Node::mapped_type*, const typename Node::mapped_type&> const_iterator;

		typedef typename base_type::iterator_category iterator_category;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::reference reference;

		node* mpNode;

		explicit avltree_iterator(const node* _pNode = nullptr);
		avltree_iterator(const this_type& _src);
		this_type& operator=(const this_type& _rhs);

		reference operator*() const;
		pointer   operator->() const;

		this_type& operator++();
		this_type  operator++(int);

		this_type& operator--();
		this_type  operator--(int);

		operator const_iterator() const { return const_iterator(mpNode); }
	};
	template<typename Node, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
	bool operator== (const avltree_iterator<Node, Pointer0, Reference0>& _lhs, const avltree_iterator<Node, Pointer1, Reference1>& _rhs) {
		return _lhs.mpNode == _rhs.mpNode;
	}
	template<typename Node, typename Pointer0, typename Reference0, typename Pointer1, typename Reference1>
	bool operator!= (const avltree_iterator<Node, Pointer0, Reference0>& _lhs, const avltree_iterator<Node, Pointer1, Reference1>& _rhs) {
		return _lhs.mpNode != _rhs.mpNode;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	class avltree
	{
	public:
		typedef avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys> this_type;
		typedef Key key_type;
		typedef conditional_t<
			is_same<ExtractKey, use_self<Key> >::value
			, Key
			, conditional_t<
			is_same<ExtractKey, use_first<pair<const Key, T> > >::value
			, pair<const Key, T>
			, void
			> 
		> value_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef Compare key_compare;
		typedef Allocator allocator_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef avltree_node<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys> node;
		typedef conditional_t<bMutableIterators
			, avltree_iterator<node, T*, T&>
			, avltree_iterator<node, const T*, const T&> > iterator;
		typedef avltree_iterator<node, const T*, const T&> const_iterator;
		typedef cckit::reverse_iterator<iterator> reverse_iterator;
		typedef cckit::reverse_iterator<const_iterator> const_reverse_iterator;

		// AVL TREE
	private:
		static const int MAX_NODE_HEIGHT_DIFF = 1;
		//! AVL TREE
	public:
		avltree() : avltree(key_compare()) {}
		explicit avltree(const key_compare& _compare, const allocator_type& _allocator = allocator_type());
		explicit avltree(const allocator_type& _allocator);
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			avltree(InputIterator _first, InputIterator _last
				, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type());
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			avltree(InputIterator _first, InputIterator _last, const allocator_type& _allocator)
			: avltree(_first, _last, key_compare(), _allocator) {}
		avltree(const this_type& _other, const allocator_type& _allocator = allocator_type());
		avltree(this_type&& _other, const allocator_type& _allocator = allocator_type());
		avltree(std::initializer_list<value_type> _ilist
			, const key_compare& _compare = key_compare(), const allocator_type& _allocator = allocator_type());
		avltree(std::initializer_list<value_type> _ilist, const allocator_type& _allocator)
			: avltree(_ilist, key_compare(), _allocator) {}

		this_type& operator=(const this_type& _rhs);
		this_type& operator=(this_type&& _rhs);
		this_type& operator=(std::initializer_list<value_type> _ilist);

		~avltree();

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

		iterator root();
		const_iterator root() const;

		bool empty() const { return mSize == 0; }
		size_type size() const { return mSize; }

		pair<iterator, bool> insert(const value_type& _val);
		pair<iterator, bool> insert(value_type&& _val);
		iterator insert(const_iterator _hint, const value_type& _val);
		iterator insert(const_iterator _hint, value_type&& _val);
		template<typename InputIterator
			, typename = typename enable_if_t<is_iterator<InputIterator>::value>
			, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
			void insert(InputIterator _first, InputIterator _last);
		void insert(std::initializer_list<value_type> _ilist);

		template<typename... Args>
		pair<iterator, bool> emplace(Args&&... _args);
		template<typename... Args>
		iterator emplace_hint(const_iterator _hint, Args&&... _args);

		iterator erase(const_iterator _pos);
		iterator erase(const_iterator _first, const_iterator _last);
		size_type erase(const key_type& _key);

		void swap(this_type& _other);
		void clear();

		size_type count(const key_type& _key) const;
		iterator find(const key_type& _key);
		const_iterator find(const key_type& _key) const;

		pair<iterator, iterator> equal_range(const key_type& _key) { return pair<iterator, iterator>(lower_bound(_key), upper_bound(_key)); }
		pair<const_iterator, const_iterator> equal_range(const key_type& _key) const { return pair<const_iterator, const_iterator>(lower_bound(_key), upper_bound(_key)); }
		iterator lower_bound(const key_type& _key);
		const_iterator lower_bound(const key_type& _key) const { return (const_cast<this_type*>(this))->lower_bound(_key); } 
		iterator upper_bound(const key_type& _key);
		const_iterator upper_bound(const key_type& _key) const { return (const_cast<this_type*>(this))->upper_bound(_key); }

		key_compare key_comp() const { return mCompare; }

		template<typename UnaryFunction
			, typename = typename enable_if_t<is_same<decltype(declval<UnaryFunction>()(iterator()), char()), char>::value>>
		void preorder_walk(UnaryFunction _func, const_iterator _root) const;
		template<typename UnaryFunction
			, typename = typename enable_if_t<is_same<decltype(declval<UnaryFunction>()(iterator()), char()), char>::value>>
		void inorder_walk(UnaryFunction _func, const_iterator _root) const;
		template<typename UnaryFunction
			, typename = typename enable_if_t<is_same<decltype(declval<UnaryFunction>()(iterator()), char()), char>::value>>
		void postorder_walk(UnaryFunction _func, const_iterator _root) const;

	protected:
		node* CreateSentinelNode() const;
		node* CreateNode() const;
		node* CreateNode(const value_type& _val) const;
		void FreeNode(node* _pNode) const;
		void CascadeFreeNode(node* _pNode) const;

		node* Find(const key_type& _key) { return const_cast<const this_type*>(this)->Find(_key); }
		node* Find(const key_type& _key) const;
		node* Min(node* _pRoot) const;
		node* Max(node* _pRoot) const;
		node* ParentSuccessor(node* _pNode) const;
		node* ParentPredecessor(node* _pNode) const;
		node* Successor(node* _pNode) const;
		node* Predecessor(node* _pNode) const;

		pair<node*, bool> Insert(node* _pNew, node* _pHint, const key_type& _key = key_type(), bool _bCustomKeyUsed = false);
		void Transplant(node* _pOld, node* _pNew);
		node* Erase(node* _pNode);

		template<class UnaryFunction>
		void PreorderWalk(UnaryFunction _func, node* _pNode) const;
		template<class UnaryFunction>
		void InorderWalk(UnaryFunction _func, node* _pNode) const;
		template<class UnaryFunction>
		void PostorderWalk(UnaryFunction _func, node* _pNode) const;

		// AVL TREE
		int Height(node* _pNode) const { return _pNode ? _pNode->mHeight : -1; }
		void UpdateHeight(node* _pNode) const;
		void RotateLeft(node* _pRoot);
		void RotateRight(node* _pRoot);
		void RotateLeftRight(node* _pRoot);
		void RotateRightLeft(node* _pRoot);
		void Balance(node* _pNode);
		//! AVL TREE

	protected:
		node* mpSentinel;// mpSentinel->mpParent == root, mpSentinel->mpNext == min, mpSentinel->mpPrev == max
		size_type mSize;
		key_compare mCompare;
		allocator_type mAllocator;
		ExtractKey mExtractKey;

		template<typename Key, typename T, typename Compare, typename Allocator
			, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
			friend struct avltree_node;
		template<typename Node, typename Pointer, typename Reference>
		friend struct avltree_iterator;
	};
}

namespace cckit
{
#pragma region avltree_node<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		void avltree_node<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Clone(const this_type& _other, bool _bOnInception = true)
	{
		if (_bOnInception) {
			if (mpPrev || _other.mpPrev) return;
			(*mppOwner)->CascadeFreeNode(mpNext);
			(*mppOwner)->CascadeFreeNode(mpLeft);
			(*mppOwner)->CascadeFreeNode(mpRight);
		}
		
		mHeight = _other.mHeight;
		cckit::initialized_init(&mVal, cckit::move(_other.mVal));

		SetLeft(_other.mpLeft ? (*mppOwner)->CreateNode() : nullptr);
		SetRight(_other.mpRight ? (*mppOwner)->CreateNode() : nullptr);
		SetNext(_other.mpNext ? (*mppOwner)->CreateNode() : nullptr);

		if (mpLeft) 
			mpLeft->Clone(*_other.mpLeft, false);
		if (mpRight) 
			mpRight->Clone(*_other.mpRight, false);
		if (mpNext) 
			mpNext->Clone(*_other.mpNext, false);
	}
#pragma endregion avltree_node<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>

#pragma region avltree_iterator<Node, Pointer, Reference>
	template<typename Node, typename Pointer, typename Reference>
	inline avltree_iterator<Node, Pointer, Reference>::avltree_iterator(const node* _pNode)
		: mpNode(const_cast<node*>(_pNode))
	{}

	template<typename Node, typename Pointer, typename Reference>
	inline avltree_iterator<Node, Pointer, Reference>::avltree_iterator(const this_type& _src)
		: mpNode(_src.mpNode)
	{}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::this_type&
		avltree_iterator<Node, Pointer, Reference>::operator=(const this_type& _rhs)
	{
		mpNode = _rhs.mpNode;
		return *this;
	}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::reference
		avltree_iterator<Node, Pointer, Reference>::operator*() const
	{
		return mpNode->mVal;
	}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::pointer
		avltree_iterator<Node, Pointer, Reference>::operator->() const
	{
		return &(**this);
	}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::this_type&
		avltree_iterator<Node, Pointer, Reference>::operator++()
	{
		mpNode = (*(mpNode->mppOwner))->Successor(mpNode);
		return *this;
	}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::this_type
		avltree_iterator<Node, Pointer, Reference>::operator++(int)
	{
		this_type temp = *this;
		++*this;
		return temp;
	}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::this_type&
		avltree_iterator<Node, Pointer, Reference>::operator--()
	{
		mpNode = (*(mpNode->mppOwner))->Predecessor(mpNode);
		return *this;
	}

	template<typename Node, typename Pointer, typename Reference>
	inline typename avltree_iterator<Node, Pointer, Reference>::this_type
		avltree_iterator<Node, Pointer, Reference>::operator--(int)
	{
		this_type temp = *this;
		--*this;
		return temp;
	}
#pragma endregion avltree_iterator<Node, Pointer, Reference>

#pragma region avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::avltree(const key_compare& _compare, const allocator_type& _allocator)
		: mpSentinel(CreateSentinelNode()), mSize(0)
		, mCompare(_compare), mAllocator(_allocator), mExtractKey()
	{}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::avltree(const allocator_type& _allocator)
		: avltree(key_compare(), _allocator)
	{}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename InputIterator
		, typename = typename enable_if_t<is_iterator<InputIterator>::value>
		, typename = typename enable_if_t<is_same<typename iterator_traits<InputIterator>::value_type, value_type>::value> >
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::avltree(InputIterator _first, InputIterator _last, const key_compare& _compare, const allocator_type& _allocator)
		: avltree(_compare, _allocator)
	{
		insert(_first, _last);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::avltree(const this_type& _other, const allocator_type& _allocator = allocator_type())
		: avltree(_allocator)
	{
		if (_other.mpSentinel->mpParent) {
			(mpSentinel->mpParent = CreateNode())->Clone(*_other.mpSentinel->mpParent);
			mpSentinel->mpNext = Min(mpSentinel->mpParent);
			mpSentinel->mpPrev = Max(mpSentinel->mpParent);
			mSize = _other.mSize;
		}
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::avltree(this_type&& _other, const allocator_type& _allocator = allocator_type())
		: avltree(_allocator)
	{
		if (_other.mSize > 0) 
			swap(_other);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::avltree(std::initializer_list<value_type> _ilist, const key_compare& _compare, const allocator_type& _allocator)
		: avltree(_ilist.begin(), _ilist.end(), _compare, _allocator)
	{}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::this_type& 
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::operator=(const this_type& _rhs)
	{
		this_type temp(_rhs, _rhs.mAllocator);
		swap(temp);
		return *this;
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::this_type&
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::operator=(this_type&& _rhs)
	{
		this_type temp(cckit::move(_rhs), _rhs.mAllocator);
		swap(temp);
		return *this;
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::this_type&
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::operator=(std::initializer_list<value_type> _ilist)
	{
		this_type temp(_ilist.begin(), _ilist.end(), mAllocator);
		swap(temp);
		return *this;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::~avltree()
	{
		mAllocator.deallocate(mpSentinel->mppOwner);
		if (mpSentinel->mpParent != mpSentinel)
			CascadeFreeNode(mpSentinel->mpParent);
		FreeNode(mpSentinel);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::begin()
	{
		return iterator(mpSentinel->mpNext);
		return iterator(Min(mpSentinel->mpParent));
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::begin() const
	{
		return const_iterator(mpSentinel->mpNext);
		return const_iterator(Min(mpSentinel->mpParent));
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::cbegin() const
	{
		return const_iterator(mpSentinel->mpNext);
		return const_iterator(Min(mpSentinel->mpParent));
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::end()
	{
		return iterator(mpSentinel);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::end() const
	{
		return const_iterator(mpSentinel);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::cend() const
	{
		return const_iterator(mpSentinel);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::reverse_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::rbegin()
	{
		return reverse_iterator(end());
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_reverse_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::rbegin() const
	{
		return const_reverse_iterator(end());
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_reverse_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::crbegin() const
	{
		return const_reverse_iterator(end());
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::reverse_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::rend()
	{
		return reverse_iterator(begin());
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_reverse_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::rend() const
	{
		return const_reverse_iterator(begin());
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_reverse_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::crend() const
	{
		return const_reverse_iterator(begin());
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::root()
	{
		return iterator(mpSentinel->mpParent);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::root() const
	{
		return const_iterator(mpSentinel->mpParent);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline pair<typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator, bool>
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(const value_type& _val)
	{
		pair<node*, bool> pairResult = cckit::move(Insert(CreateNode(_val), mpSentinel->mpParent));
		return pair<iterator, bool>(iterator(pairResult.first), pairResult.second);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline pair<typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator, bool>
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(value_type&& _val)
	{
		pair<node*, bool> pairResult = cckit::move(Insert(CreateNode(), mpSentinel->mpParent, mExtractKey(_val), true));
		cckit::initialized_init(&(pairResult.first->mVal), cckit::forward<value_type>(_val));
		return pair<iterator, bool>(iterator(pairResult.first), pairResult.second);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(const_iterator _hint, const value_type& _val)
	{
		pair<node*, bool> pairResult = cckit::move(Insert(CreateNode(_val), _hint.mpNode));
		return iterator(pairResult.first);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(const_iterator _hint, value_type&& _val)
	{
		return emplace_hint(_hint, cckit::forward<value_type>(_val));
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename InputIterator, typename, typename>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(InputIterator _first, InputIterator _last)
	{
		for (; _first != _last; insert(cckit::move(*_first)), ++_first) {}
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(std::initializer_list<value_type> _ilist)
	{
		insert(_ilist.begin(), _ilist.end());
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename... Args>
	inline pair<typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator, bool>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::emplace(Args&&... _args)
	{
		node* pNew = CreateNode();
		cckit::initialized_init(&(pNew->mVal), cckit::forward<Args>(_args)...);
		pair<node*, bool> pairResult = cckit::move(Insert(pNew, mpSentinel->mpParent));
		return pair<iterator, bool>(iterator(pairResult.first), pairResult.second);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename... Args>
	inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::emplace_hint(const_iterator _hint, Args&&... _args)
	{
		node* pNew = CreateNode();
		cckit::initialized_init(&(pNew->mVal), cckit::forward<Args>(_args)...);
		pair<node*, bool> pairResult = cckit::move(Insert(pNew, _hint.mpNode));
		return iterator(pairResult.first);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::erase(const_iterator _pos)
	{
		return iterator(Erase(_pos.mpNode));
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::erase(const_iterator _first, const_iterator _last)
	{
		iterator first = iterator(_first.mpNode), last = iterator(_last.mpNode);
		for (; first != last; first = erase(first)) {}
		return first;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::size_type
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::erase(const key_type& _key)
	{
		size_type nCount = 0;
		if (node* pTarget = Find(_key)) 
			for (; pTarget != mpSentinel && mExtractKey(pTarget->mVal) == _key; pTarget = Erase(pTarget), ++nCount) {}
		return nCount;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::swap(this_type& _other)
	{
		cckit::swap(mpSentinel, _other.mpSentinel);
		cckit::swap(mSize, _other.mSize);

		*(mpSentinel->mppOwner) = this;
		*(_other.mpSentinel->mppOwner) = &_other;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::clear()
	{
		CascadeFreeNode(mpSentinel->mpParent);
		mpSentinel->mpParent = nullptr;
		mSize = 0;
		// ALTERNATIVE
		//for (auto current = begin(); size() > 0; current = erase(current)) {}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::size_type
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::count(const key_type& _key) const
	{
		size_type nCount = 0;
		for (node* pIdentical = Find(_key); pIdentical; pIdentical = pIdentical->mpNext, ++nCount) {}
		return nCount;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator 
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::find(const key_type& _key)
	{
		node* pIdentical = Find(_key);
		return iterator(pIdentical ? pIdentical : mpSentinel);
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator 
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::find(const key_type& _key) const
	{
		node* pIdentical = Find(_key);
		return const_iterator(pIdentical ? pIdentical : mpSentinel);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::lower_bound(const key_type& _key)
	{
		iterator current = begin(), last = end(); 
		for (; current != last && mCompare(mExtractKey(current.mpNode->mVal), _key); ++current) {}
		return current;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::upper_bound(const key_type& _key)
	{
		iterator current = begin(), last = end();
		for (; current != last && !mCompare(_key, mExtractKey(current.mpNode->mVal)); ++current) {}
		return current;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename UnaryFunction, typename>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::preorder_walk(UnaryFunction _func, const_iterator _root) const
	{
		if (_root.mpNode) {
			_func(iterator(_root.mpNode));
			preorder_walk(_func, const_iterator(_root.mpNode->mpNext));
			preorder_walk(_func, const_iterator(_root.mpNode->mpLeft));
			preorder_walk(_func, const_iterator(_root.mpNode->mpRight));
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename UnaryFunction, typename>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::inorder_walk(UnaryFunction _func, const_iterator _root) const
	{
		if (_root.mpNode) {
			inorder_walk(_func, const_iterator(_root.mpNode->mpLeft));
			_func(iterator(_root.mpNode));
			inorder_walk(_func, const_iterator(_root.mpNode->mpNext));
			inorder_walk(_func, const_iterator(_root.mpNode->mpRight));
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<typename UnaryFunction, typename>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::postorder_walk(UnaryFunction _func, const_iterator _root) const
	{
		if (_root.mpNode) {
			postorder_walk(_func, const_iterator(_root.mpNode->mpLeft));
			postorder_walk(_func, const_iterator(_root.mpNode->mpRight));
			_func(iterator(_root.mpNode));
			postorder_walk(_func, const_iterator(_root.mpNode->mpNext));
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::CreateSentinelNode() const
	{
		node* pTemp = ::new(static_cast<void*>(mAllocator.allocate(sizeof(node)))) node();
		pTemp->mpParent = pTemp->mpPrev = pTemp->mpNext = pTemp;
		pTemp->mppOwner = ::new(static_cast<void*>(mAllocator.allocate(sizeof(const this_type*)))) const this_type*;
		*(pTemp->mppOwner) = this;
		return pTemp;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::CreateNode() const
	{
		node* pTemp = ::new(static_cast<void*>(mAllocator.allocate(sizeof(node)))) node();
		pTemp->mppOwner = mpSentinel->mppOwner;
		return pTemp;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::CreateNode(const value_type& _val) const
	{
		node* pTemp = ::new(static_cast<void*>(mAllocator.allocate(sizeof(node)))) node(_val);
		pTemp->mppOwner = mpSentinel->mppOwner;
		return pTemp;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::FreeNode(node* _pNode) const
	{
		if (_pNode) {
			_pNode->~node();
			mAllocator.deallocate(_pNode);
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::CascadeFreeNode(node* _pNode) const
	{
		if (_pNode) {
			CascadeFreeNode(_pNode->mpNext);
			CascadeFreeNode(_pNode->mpLeft);
			CascadeFreeNode(_pNode->mpRight);

			FreeNode(_pNode);
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Find(const key_type& _key) const
	{
		node* pCurrent = mpSentinel->mpParent;

		while (pCurrent) {
			key_type currentKey = mExtractKey(pCurrent->mVal);
			if (mCompare(currentKey, _key))
				pCurrent = pCurrent->mpRight;
			else if (mCompare(_key, currentKey))
				pCurrent = pCurrent->mpLeft;
			else
				break;
		}
		return pCurrent;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Min(node* _pRoot) const
	{
		if (_pRoot)
			for (; _pRoot->mpLeft; _pRoot = _pRoot->mpLeft) {}
		return _pRoot;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Max(node* _pRoot) const
	{
		if (_pRoot)
			for (; _pRoot->mpRight; _pRoot = _pRoot->mpRight) {}
		return _pRoot;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::ParentSuccessor(node* _pNode) const
	{
		node* pParent = _pNode;
		if (pParent) {
			while (pParent && pParent->mpLeft != _pNode) {
				_pNode = pParent;
				pParent = pParent->mpParent;
			}
		}
		return pParent;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::ParentPredecessor(node* _pNode) const
	{
		node* pParent = _pNode;
		if (pParent) {
			while (pParent && pParent->mpRight != _pNode) {
				_pNode = pParent;
				pParent = pParent->mpParent;
			}
		}
		return pParent;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Successor(node* _pNode) const
	{
		assert((_pNode));
		if (_pNode == mpSentinel)
			return _pNode->mpNext;
		if (_pNode->mpNext)
			return _pNode->mpNext;

		for (; _pNode->mpPrev; _pNode = _pNode->mpPrev) {}
		if (auto pRightMin = Min(_pNode->mpRight))
			_pNode = pRightMin;
		else
			_pNode = ParentSuccessor(_pNode);

		return (_pNode) ? _pNode : mpSentinel;
	}

	/*template<typename Key, typename T, typename Compare, typename Allocator
	, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
	avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
	::Successor(node* _pNode) const
	{
	if (auto pRightMin = Min(_pNode->mpRight))
	return pRightMin;
	return ParentSuccessor(_pNode);
	}*/

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Predecessor(node* _pNode) const
	{
		if (!_pNode)
			return _pNode;
		if (_pNode == mpSentinel)
			return _pNode->mpPrev;
		if (_pNode->mpPrev)
			return _pNode->mpPrev;

		if (auto pLeftMax = Max(_pNode->mpLeft))
			_pNode = pLeftMax;
		else
			_pNode = ParentPredecessor(_pNode);
		if (_pNode)
			for (; _pNode->mpNext; _pNode = _pNode->mpNext) {}

		return (_pNode) ? _pNode : mpSentinel;
	}

	/*template<typename Key, typename T, typename Compare, typename Allocator
	, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
	avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
	::predecessor(node* _pNode) const
	{
	if (auto pLeftMax = Max(_pNode->mpLeft))
	return pLeftMax;
	return ParentPredecessor(_pNode);
	}*/

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		pair<typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*, bool>
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Insert(node* _pNew, node* _pHint, const key_type& _key, bool _bCustomKeyUsed)
	{
		node *pCurrent = _pHint, *pPrev = pCurrent;
		key_type currentKey, newKey = (_bCustomKeyUsed ? _key : mExtractKey(_pNew->mVal));
		bool bElemInserted = true, bIdenticalKeyEncountered = false;
		
		while (pCurrent != mpSentinel->mpParent) {
			key_type parentKey = mExtractKey(pCurrent->mpParent->mVal);
			if (mCompare(newKey, parentKey) && pCurrent == pCurrent->mpParent->mpRight){ 
				pCurrent = pCurrent->mpParent; continue;
			}
			if (mCompare(parentKey, newKey) && pCurrent == pCurrent->mpParent->mpLeft) {
				pCurrent = pCurrent->mpParent; continue;
			}
			break;
		}
		
		while (pCurrent) {
			pPrev = pCurrent;
			
			currentKey = mExtractKey(pCurrent->mVal);
			if (mCompare(currentKey, newKey)) 
				pCurrent = pCurrent->mpRight;
			else if (mCompare(newKey, currentKey)) 
				pCurrent = pCurrent->mpLeft;
			else
				break;
		}

		if (mpSentinel->mpParent != mpSentinel) {// the tree is not empty
			if (!pCurrent) {// the insertion takes place at a leaf
				key_type parentKey = mExtractKey(pPrev->mVal);
				
				if (mCompare(parentKey, newKey))
					pPrev->mpRight = _pNew;
				else if (mCompare(newKey, parentKey))
					pPrev->mpLeft = _pNew;
				_pNew->mpParent = pPrev;

				pCurrent = _pNew;

				if (mCompare(newKey, mExtractKey(mpSentinel->mpNext->mVal)))// newKey is min
					mpSentinel->mpNext = pCurrent;
				else if (mCompare(mExtractKey(mpSentinel->mpPrev->mVal), newKey))// newKey is max
					mpSentinel->mpPrev = pCurrent;
			}
			else {// an identical key is encountered
				bIdenticalKeyEncountered = true;
				if (bUniqueKeys) {
					FreeNode(_pNew);
					bElemInserted = false;
				}
				else {
					for (; pCurrent->mpNext; pCurrent = pCurrent->mpNext) {}
					pCurrent->mpNext = _pNew;
					_pNew->mpPrev = pCurrent;
					pCurrent = _pNew;
				}
			}
		}
		else {// the insertion takes place at the root
			mpSentinel->mpParent = mpSentinel->mpPrev = mpSentinel->mpNext = (pCurrent = _pNew);
			_pNew->mpParent = _pNew->mpLeft = _pNew->mpRight = nullptr;
		}

		if (bElemInserted) {
			++mSize;
			if (!bIdenticalKeyEncountered) { Balance(pCurrent); }// AVL TREE
		}
		return pair<node*, bool>(pCurrent, bElemInserted);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Transplant(node* _pOld, node* _pNew)
	{
		assert((_pOld));
		if (!_pOld->mpParent)
			mpSentinel->mpParent = _pNew;
		else if (_pOld->mpParent->mpLeft == _pOld)
			_pOld->mpParent->mpLeft = _pNew;
		else if (_pOld->mpParent->mpRight == _pOld)
			_pOld->mpParent->mpRight = _pNew;
		if (_pNew)
			_pNew->mpParent = _pOld->mpParent;
		// ALTERNATIVE
		/*if (_pNew)
			_pNew->mpParent = _pOld->mpParent;
		if (_pOld->mpParent) {
			if (_pOld->mpParent->mpLeft == _pOld)
				_pOld->mpParent->mpLeft = _pNew;
			else if (_pOld->mpParent->mpRight == _pOld)
				_pOld->mpParent->mpRight = _pNew;
		}
		else
			mpSentinel->mpParent = _pNew;*/
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Erase(node* _pNode)
	{
		assert((_pNode));
		node *pSuccessor = Successor(_pNode), *pPredecessor = Predecessor(_pNode)
			, *pBalanceSource = nullptr, *pReplacement = nullptr;
		
		if (_pNode->mpPrev || _pNode->mpNext) {
			if (_pNode->mpNext)
				_pNode->mpNext->mpPrev = _pNode->mpPrev;
			if (_pNode->mpPrev)
				_pNode->mpPrev->mpNext = _pNode->mpNext;
			else {
				Transplant(_pNode, pReplacement = _pNode->mpNext);
				_pNode->mpNext->SetLeft(_pNode->mpLeft);
				_pNode->mpNext->SetRight(_pNode->mpRight);
			}
		}// if (_pNode->mpPrev || _pNode->mpNext)
		else if (!_pNode->mpLeft) {
			Transplant(_pNode, pReplacement = _pNode->mpRight);
			pBalanceSource = (_pNode->mpRight ? _pNode->mpRight : _pNode->mpParent);// AVL TREE
		}
		else if (!_pNode->mpRight) {
			Transplant(_pNode, pReplacement = _pNode->mpLeft);
			pBalanceSource = _pNode->mpLeft;// AVL TREE
		}
		else {
			node* pLeftMax = Max(_pNode->mpLeft);
			pBalanceSource = (pLeftMax->mpParent != _pNode) ? pLeftMax->mpParent : pLeftMax;// AVL TREE

			if (pLeftMax->mpParent != _pNode) {
				Transplant(pLeftMax, pLeftMax->mpLeft);
				pLeftMax->mpLeft = _pNode->mpLeft;
				pLeftMax->mpLeft->mpParent = pLeftMax;
			}
			Transplant(_pNode, pReplacement = pLeftMax);
			pLeftMax->mpRight = _pNode->mpRight;
			pLeftMax->mpRight->mpParent = pLeftMax;
		}

		if (mpSentinel->mpParent == _pNode)
			mpSentinel->mpParent = (pReplacement ? pReplacement : mpSentinel);
		if (mpSentinel->mpNext == _pNode)
			mpSentinel->mpNext = pSuccessor;
		if (mpSentinel->mpPrev == _pNode)
			mpSentinel->mpPrev = pPredecessor;

		FreeNode(_pNode);
		--mSize;
		Balance(pBalanceSource);// AVL TREE
		
		return pSuccessor;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<class UnaryFunction>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::PreorderWalk(UnaryFunction _func, node* _pNode) const
	{
		if (_pNode) {
			_func(_pNode);
			PreorderWalk(_func, _pNode->mpNext);
			PreorderWalk(_func, _pNode->mpLeft);
			PreorderWalk(_func, _pNode->mpRight);
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<class UnaryFunction>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::InorderWalk(UnaryFunction _func, node* _pNode) const
	{
		if (_pNode) {
			InorderWalk(_func, _pNode->mpLeft);
			_func(_pNode);
			InorderWalk(_func, _pNode->mpNext);
			InorderWalk(_func, _pNode->mpRight);
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<class UnaryFunction>
	inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::PostorderWalk(UnaryFunction _func, node* _pNode) const
	{
		if (_pNode) {
			PostorderWalk(_func, _pNode->mpLeft);
			PostorderWalk(_func, _pNode->mpRight);
			_func(_pNode);
			PostorderWalk(_func, _pNode->mpNext);
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::UpdateHeight(node* _pNode) const
	{
		assert((_pNode));
		_pNode->mHeight = cckit::max(Height(_pNode->mpLeft), Height(_pNode->mpRight)) + 1;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::RotateLeft(node* _pRoot)
	{
		assert((_pRoot));
		assert((_pRoot->mpRight));

		node *pRight = _pRoot->mpRight, *pParent = _pRoot->mpParent;

		Transplant(_pRoot, pRight);// this statement must precede "pRight->SetLeft(_pRoot)" which modifies _pRoot->mpParent
		_pRoot->SetRight(pRight->mpLeft);
		pRight->SetLeft(_pRoot);
		
		UpdateHeight(pRight->mpLeft);
		UpdateHeight(pRight);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::RotateRight(node* _pRoot)
	{
		assert((_pRoot));
		assert((_pRoot->mpLeft));

		node *pLeft = _pRoot->mpLeft; 

		Transplant(_pRoot, pLeft);// this statement must precede "pLeft->SetRight(_pRoot);" which modifies _pRoot->mpParent
		_pRoot->SetLeft(pLeft->mpRight);
		pLeft->SetRight(_pRoot);

		UpdateHeight(pLeft->mpRight);
		UpdateHeight(pLeft);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::RotateLeftRight(node* _pRoot)
	{
		assert((_pRoot));
		RotateLeft(_pRoot->mpLeft);
		RotateRight(_pRoot);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		inline void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::RotateRightLeft(node* _pRoot)
	{
		assert((_pRoot));
		RotateRight(_pRoot->mpRight);
		RotateLeft(_pRoot);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Balance(node* _pNode)
	{
		if (_pNode) {
			UpdateHeight(_pNode);

			node *pLeft = _pNode->mpLeft, *pRight = _pNode->mpRight;
			if (Height(pLeft) - Height(pRight) > MAX_NODE_HEIGHT_DIFF) {
				node *pLeftLeft = pLeft->mpLeft, *pLeftRight = pLeft->mpRight;
				if (Height(pLeftLeft) >= Height(pLeftRight))
					RotateRight(_pNode);
				else
					RotateLeftRight(_pNode);
			}
			else if (Height(pRight) - Height(pLeft) > MAX_NODE_HEIGHT_DIFF) {
				node *pRightRight = pRight->mpRight, *pRightLeft = pRight->mpLeft;
				if (Height(pRightRight) >= Height(pRightLeft))
					RotateLeft(_pNode);
				else
					RotateRightLeft(_pNode);
			}

			Balance(_pNode->mpParent);
		}
	}
#pragma endregion avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
}

#endif // !AVL_TREE_H