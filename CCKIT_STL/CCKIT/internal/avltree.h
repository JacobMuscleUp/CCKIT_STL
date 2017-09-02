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
		owner_type* mpOwner;
		int mHeight;
		value_type mVal;

		explicit avltree_node(owner_type* _pOwner, const value_type& _val = value_type())
			: mpLeft(nullptr), mpRight(nullptr), mpParent(nullptr), mpNext(nullptr), mpPrev(nullptr), mpOwner(_pOwner), mVal(_val) {}
		avltree_node(const this_type&) = delete;
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
		typedef conditional_t<is_same<ExtractKey, use_self<Key> >::value
			, Key
			, conditional_t<is_same<ExtractKey, use_first<pair<const Key, T> > >::value
				, pair<const Key, T>
				, void> > value_type;
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

		avltree() : avltree(key_compare()) {}
		explicit avltree(const key_compare& _compare, const allocator_type& _allocator = allocator_type());

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

		pair<iterator, bool> insert(const value_type& _val);

		void clear();

		template<class UnaryFunction>
		void preorder_walk(UnaryFunction _func, const_iterator _root) const;
		template<class UnaryFunction>
		void inorder_walk(UnaryFunction _func, const_iterator _root) const;
		template<class UnaryFunction>
		void postorder_walk(UnaryFunction _func, const_iterator _root) const;

	protected:
		node* CreateNode(const value_type& _val);
		void FreeNode(node* _pNode);
		void CascadeFreeNode(node* _pNode);

		node* Find(const key_type& _key);
		node* Min(node* _pRoot);
		node* Max(node* _pRoot);
		node* ParentSuccessor(node* _pNode);
		node* ParentPredecessor(node* _pNode);
		node* Successor(node* _pNode);
		node* Predecessor(node* _pNode);

		template<class UnaryFunction>
		void PreorderWalk(UnaryFunction _func, node* _pNode) const;
		template<class UnaryFunction>
		void InorderWalk(UnaryFunction _func, node* _pNode) const;
		template<class UnaryFunction>
		void PostorderWalk(UnaryFunction _func, node* _pNode) const;

	protected:
		node* mpSentinel;
		size_type mSize;
		key_compare mCompare;
		allocator_type mAllocator;
		ExtractKey mExtractKey;

		template<typename Node, typename Pointer, typename Reference>
		friend class avltree_iterator;
	};
}

namespace cckit
{

#pragma region avltree_iterator<Node, Pointer, Reference>
	template<typename Node, typename Pointer, typename Reference>
	avltree_iterator<Node, Pointer, Reference>::avltree_iterator(const node* _pNode)
		: mpNode(const_cast<node*>(_pNode))
	{}

	template<typename Node, typename Pointer, typename Reference>
	avltree_iterator<Node, Pointer, Reference>::avltree_iterator(const this_type& _src)
		: mpNode(_src.mpNode)
	{}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::this_type& 
		avltree_iterator<Node, Pointer, Reference>::operator=(const this_type& _rhs)
	{
		mpNode = _rhs.mpNode;
		return *this;
	}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::reference 
		avltree_iterator<Node, Pointer, Reference>::operator*() const
	{
		return mpNode->mVal;
	}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::pointer
		avltree_iterator<Node, Pointer, Reference>::operator->() const
	{
		return &(**this);
	}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::this_type&
		avltree_iterator<Node, Pointer, Reference>::operator++()
	{
		mpNode = mpNode->mpOwner->Successor(mpNode);
		return *this;
	}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::this_type
		avltree_iterator<Node, Pointer, Reference>::operator++(int)
	{
		this_type temp = *this;
		++*this;
		return temp;
	}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::this_type&
		avltree_iterator<Node, Pointer, Reference>::operator--()
	{
		mpNode = mpNode->mpOwner->Predecessor(mpNode);
		return *this;
	}

	template<typename Node, typename Pointer, typename Reference>
	typename avltree_iterator<Node, Pointer, Reference>::this_type
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
	avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
	::avltree(const key_compare& _compare, const allocator_type& _allocator)
		: mpSentinel(CreateNode(value_type())), mSize(0)
		, mCompare(_compare), mAllocator(_allocator), mExtractKey()
	{}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::~avltree()
	{
		CascadeFreeNode(mpSentinel->mpParent);
		FreeNode(mpSentinel);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::begin()
	{
		return iterator(Min(mpSentinel->mpParent));
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::begin() const
	{
		return const_iterator(Min(mpSentinel->mpParent));
	}
	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	inline typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::const_iterator
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::cbegin() const
	{
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
	pair<typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::iterator, bool> 
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::insert(const value_type& _val)
	{
		node *pCurrent = mpSentinel->mpParent, *pPrev = pCurrent;
		key_type currentKey, newKey = mExtractKey(_val);
		bool insertionSucceed = true;
		bool bKeyMin = true, bKeyMax = true;

		while (pCurrent) {
			pPrev = pCurrent;

			currentKey = mExtractKey(pCurrent->mVal);
			if (mCompare(currentKey, newKey)) {
				pCurrent = pCurrent->mpRight;
				bKeyMin = false;
			}
			else if (mCompare(newKey, currentKey)) {
				pCurrent = pCurrent->mpLeft;
				bKeyMax = false;
			}
			else
				break;
		}

		if (mpSentinel->mpParent) {// the tree is not empty
			if (!pCurrent) {// the insertion takes place at a leaf
				key_type parentKey = mExtractKey(pPrev->mVal);
				node* pNew = CreateNode(_val);

				if (mCompare(parentKey, newKey))
					pPrev->mpRight = pNew;
				else if (mCompare(newKey, parentKey))
					pPrev->mpLeft = pNew;
				pNew->mpParent = pPrev;

				pCurrent = pNew;

				if (bKeyMin)
					mpSentinel->mpNext = pCurrent;
				else if (bKeyMax)
					mpSentinel->mpPrev = pCurrent;
			}
			else {// an identical key is encountered
				if (bUniqueKeys)
					insertionSucceed = false;
				else {
					node* pNew = CreateNode(_val);
					for (; pCurrent->mpNext; pCurrent = pCurrent->mpNext) {}
					pCurrent->mpNext = pNew;
					pNew->mpPrev = pCurrent;
					pCurrent = pNew;
				}
			}
		}
		else {// the insertion takes place at the root
			node* pNew = CreateNode(_val);
			mpSentinel->mpParent = mpSentinel->mpPrev = mpSentinel->mpNext = pCurrent = pNew;
			pNew->mpParent = pNew->mpLeft = pNew->mpRight = nullptr;
		}

		return pair<iterator, bool>(iterator(pCurrent), insertionSucceed);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::clear()
	{
		CascadeFreeNode(mpSentinel->mpParent);
		mpSentinel->mpParent = nullptr;
		mSize = 0;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	template<class UnaryFunction>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
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
	template<class UnaryFunction>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
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
	template<class UnaryFunction>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
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
	typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::CreateNode(const value_type& _val)
	{
		return new(mAllocator.allocate(sizeof(node))) node(const_cast<this_type*>(this), _val);
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::FreeNode(node* _pNode)
	{
		if (_pNode) {
			_pNode->~node();
			mAllocator.deallocate(_pNode);
		}
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::CascadeFreeNode(node* _pNode)
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
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Find(const key_type& _key)
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
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Min(node* _pRoot)
	{
		if (_pRoot)
			for (; _pRoot->mpLeft; _pRoot = _pRoot->mpLeft) {}
		return _pRoot;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Max(node* _pRoot)
	{
		if (_pRoot)
			for (; _pRoot->mpRight; _pRoot = _pRoot->mpRight) {}
		return _pRoot;
	}

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::ParentSuccessor(node* _pNode)
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
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::ParentPredecessor(node* _pNode)
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
		::Successor(node* _pNode)
	{
		if (!_pNode)
			return _pNode;
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
	::Successor(node* _pNode)
	{
	if (auto pRightMin = Min(_pNode->mpRight))
	return pRightMin;
	return ParentSuccessor(_pNode);
	}*/

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		typename avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>::node*
		avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::Predecessor(node* _pNode)
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
	::predecessor(node* _pNode)
	{
	if (auto pLeftMax = Max(_pNode->mpLeft))
	return pLeftMax;
	return ParentPredecessor(_pNode);
	}*/

	template<typename Key, typename T, typename Compare, typename Allocator
		, typename ExtractKey, bool bMutableIterators, bool bUniqueKeys>
		template<class UnaryFunction>
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
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
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
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
	void avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
		::PostorderWalk(UnaryFunction _func, node* _pNode) const
	{
		if (_pNode) {
			PostorderWalk(_func, _pNode->mpLeft);
			PostorderWalk(_func, _pNode->mpRight);
			_func(_pNode);
			PostorderWalk(_func, _pNode->mpNext);
		}
	}
#pragma endregion avltree<Key, T, Compare, Allocator, ExtractKey, bMutableIterators, bUniqueKeys>
}

#endif // !AVL_TREE_H