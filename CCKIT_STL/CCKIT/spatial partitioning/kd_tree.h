#ifndef CCKIT_KD_TREE_H
#define CCKIT_KD_TREE_H

#include "../internal/config.h"

namespace cckit
{
	template<typename T, size_t Dim>
	class kd_tree;

	template<typename T, size_t Dim>
	class kd_tree_node
	{
	public:
		typedef T value_type;
	private:
		typedef kd_tree_node<T, Dim> this_type;
	private:
		kd_tree_node(value_type _pt[Dim]);
		void Free();
	private:
		value_type mPt[Dim];
		this_type *mpLeft, *mpRight;

		friend kd_tree<T, Dim>;
	};

	template<typename T, size_t Dim>
	class kd_tree
	{
	public:
		typedef T value_type;
		typedef kd_tree_node<T, Dim> node_type;
	private:
		typedef kd_tree<T, Dim> this_type;
	public:
		kd_tree();
		~kd_tree();
		void insert(value_type _pt[Dim]);
	private:
		node_type* InsertRec(value_type _pt[Dim], node_type*& _pRoot, size_t _depth);
	private:
		node_type* mpRoot;
	};

	// kd_tree_node<T, Dim>
	template<typename T, size_t Dim>
	kd_tree_node<T, Dim>::kd_tree_node(typename kd_tree_node<T, Dim>::value_type _pt[Dim]) 
		: mpLeft(nullptr), mpRight(nullptr) {
		for (size_t i = 0; i < Dim; ++i)
			mPt[i] = _pt[i];
	}

	template<typename T, size_t Dim>
	void kd_tree_node<T, Dim>::Free() {
		if (mpLeft)
			mpLeft->Free();
		if (mpRight)
			mpRight->Free();
		delete this;
	}

	// kd_tree<T, Dim>
	template<typename T, size_t Dim>
	kd_tree<T, Dim>::kd_tree()
		: mpRoot(nullptr) {

	}

	template<typename T, size_t Dim>
	kd_tree<T, Dim>::~kd_tree() {
		if (mpRoot)
			mpRoot->Free();
	}

	template<typename T, size_t Dim>
	void kd_tree<T, Dim>::insert(typename kd_tree<T, Dim>::value_type _pt[Dim]) {
		InsertRec(_pt, mpRoot, 0);
	}

	template<typename T, size_t Dim>
	typename kd_tree<T, Dim>::node_type* 
		kd_tree<T, Dim>::InsertRec(
			typename kd_tree<T, Dim>::value_type _pt[Dim]
			, typename kd_tree<T, Dim>::node_type*& _pRoot
			, size_t _depth) {
		if (!_pRoot) return _pRoot = new node_type(_pt);

		size_t curDim = (_depth) % Dim;
		if (_pt[curDim] < _pRoot->mPt[curDim])
			_pRoot->mpLeft = InsertRec(_pt, _pRoot->mpLeft, _depth + 1);
		else
			_pRoot->mpRight = InsertRec(_pt, _pRoot->mpRight, _depth + 1);

		return _pRoot;
	}
}

#endif // !CCKIT_KD_TREE_H