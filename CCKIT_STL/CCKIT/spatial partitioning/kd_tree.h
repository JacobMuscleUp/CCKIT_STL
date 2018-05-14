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
		static const size_t DIM = Dim;

		kd_tree();
		~kd_tree();
		void insert(value_type _pt[Dim]);	
		bool search(value_type _pt[Dim]);
		value_type* nearest_neighbor(value_type _pt[Dim]) const;

		template<typename Func>
		void iterate(Func _func);
	private:
		node_type* InsertRec(value_type _pt[Dim], node_type*& _pRoot, size_t _depth);
		bool SearchRec(value_type _pt[Dim], node_type* _pRoot, size_t _depth) const;
		bool Identical(value_type _pt0[Dim], value_type _pt1[Dim]) const;
		void NearestNeighbor(node_type* _pRoot, value_type _pt[Dim], value_type(&_closestPt)[Dim], value_type& _minDist, size_t _depth) const;
		void AssignArray(value_type(&_pt0)[Dim], value_type _pt1[Dim]) const;
		value_type Distance(value_type _pt0[Dim], value_type _pt1[Dim]) const;

		template<typename Func>
		void IterateRec(Func _func, node_type* _pRoot);
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
	bool kd_tree<T, Dim>::search(typename kd_tree<T, Dim>::value_type _pt[Dim]) {
		return SearchRec(_pt, mpRoot, 0);
	}

	template<typename T, size_t Dim>
	typename kd_tree<T, Dim>::value_type* 
		kd_tree<T, Dim>::nearest_neighbor(value_type _pt[Dim]) const {
		value_type closestPt[Dim];
		value_type minDist = -1;
		NearestNeighbor(mpRoot, _pt, closestPt, minDist, 0);
		return closestPt;
	}

	template<typename T, size_t Dim>
	template<typename Func>
	void kd_tree<T, Dim>::iterate(Func _func) {
		IterateRec(_func, mpRoot);
	}

	template<typename T, size_t Dim>
	typename kd_tree<T, Dim>::node_type* 
		kd_tree<T, Dim>::InsertRec(
			typename kd_tree<T, Dim>::value_type _pt[Dim]
			, typename kd_tree<T, Dim>::node_type*& _pRoot
			, size_t _depth) {
		if (!_pRoot) return _pRoot = new node_type(_pt);
		
		size_t curDim = (_depth) % Dim;
		/*if (_pt[curDim] < _pRoot->mPt[curDim])
			_pRoot->mpLeft = InsertRec(_pt, _pRoot->mpLeft, _depth + 1);
		else
			_pRoot->mpRight = InsertRec(_pt, _pRoot->mpRight, _depth + 1);*/
		if (_pt[curDim] < _pRoot->mPt[curDim])
			InsertRec(_pt, _pRoot->mpLeft, _depth + 1);
		else
			InsertRec(_pt, _pRoot->mpRight, _depth + 1);

		return _pRoot;
	}

	template<typename T, size_t Dim>
	bool kd_tree<T, Dim>::SearchRec(
			typename kd_tree<T, Dim>::value_type _pt[Dim]
			, typename kd_tree<T, Dim>::node_type* _pRoot
			, size_t _depth)  const {
		if (!_pRoot) return false;
		if (Identical(_pRoot->mPt, _pt)) return true;

		size_t curDim = (_depth) % Dim;
		if (_pt[curDim] < _pRoot->mPt[curDim])
			return SearchRec(_pt, _pRoot->mpLeft, _depth + 1);
		else
			return SearchRec(_pt, _pRoot->mpRight, _depth + 1);

	}

	template<typename T, size_t Dim>
	bool kd_tree<T, Dim>::Identical(
		value_type _pt0[Dim]
		, value_type _pt1[Dim]) const {
		for (size_t i = 0; i < Dim; ++i)
			if (_pt0[i] != _pt1[i])
				return false;
		return true;
	}

	template<typename T, size_t Dim>
	template<typename Func>
	void kd_tree<T, Dim>::IterateRec(Func _func, node_type* _pRoot) {
		if (_pRoot) {
			_func(_pRoot->mPt);
			IterateRec(_func, _pRoot->mpLeft);
			IterateRec(_func, _pRoot->mpRight);
		}
	}

	template<typename T, size_t Dim>
	void kd_tree<T, Dim>::NearestNeighbor(
		node_type* _pRoot, value_type _pt[Dim], value_type(&_closestPt)[Dim], value_type& _minDist, size_t _depth) const {
		if (!_pRoot->mpLeft && !_pRoot->mpRight) {
			value_type dist = Distance(_pRoot->mPt, _pt);
			if (dist < _minDist) {
				_minDist = dist;
				AssignArray(_closestPt, _pRoot->mPt);
			}
		}
		else {
			size_t curDim = _depth % Dim;
			if (_pt[curDim] < _pRoot->mPt[curDim]) {
				NearestNeighbor(_pRoot->mpLeft, _pt, _closestPt, _minDist, _depth + 1);
				if (_pt[curDim] + _minDist >= _pRoot->mPt[curDim])
					NearestNeighbor(_pRoot->mpRight, _pt, _closestPt, _minDist, _depth + 1);
			}
			else {
				NearestNeighbor(_pRoot->mpRight, _pt, _closestPt, _minDist, _depth + 1);
				if (_pt[curDim] - _minDist <= _pRoot->mPt[curDim])
					NearestNeighbor(_pRoot->mpLeft, _pt, _closestPt, _minDist, _depth + 1);
			}
		}
	}

	template<typename T, size_t Dim>
	void kd_tree<T, Dim>::AssignArray(value_type(&_pt0)[Dim], value_type _pt1[Dim]) const {
		for (size_t i = 0; i < Dim; ++i)
			_pt0[i] = _pt1[i];
	}

	template<typename T, size_t Dim>
	typename kd_tree<T, Dim>::value_type 
		kd_tree<T, Dim>::Distance(value_type _pt0[Dim], value_type _pt1[Dim]) const {
		value_type distSqr = 0;
		for (size_t i = 0; i < Dim; ++i)
			distSqr += std::powf(_pt0[i] - _pt1[i], 2);
		return sqrtf(distSqr);
	}
}

#endif // !CCKIT_KD_TREE_H