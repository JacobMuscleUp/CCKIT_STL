#ifndef CCKIT_QUADTREE_H
#define CCKIT_QUADTREE_H

#include "../utility.h"
#include "../deque.h"
#include "../math.h"

namespace cckit 
{
	bool equal(float _lhs, float _rhs) {
		return cckit::abs(_lhs - _rhs) < FLT_EPSILON;
	}
	bool unequal(float _lhs, float _rhs) {
		return !cckit::equal(_lhs, _rhs);
	}

	template<typename T, size_t MaxNodeSize = 3>
	class quadtree
	{
	public:
		typedef quadtree<T, MaxNodeSize> this_type;
		typedef T value_type;
		typedef size_t size_type;

		struct point {
			float mX, mY;
			point(float _x = 0.0f, float _y = 0.0f) 
				: mX(_x), mY(_y) {}
			point(const point& _other)
				: point(_other.mX, _other.mY) {}
			point& operator=(const point& _other) { point tmp = _other; swap(tmp); return *this; }
			point& operator+=(const point& _other) { mX += _other.mX; mY += _other.mY; return *this; }
			point operator+(const point& _other) const { point tmp = *this; return tmp += _other; }
			bool operator==(const point& _other) const { return cckit::equal(mX, _other.mX) && cckit::equal(mY, _other.mY); }
			bool operator!=(const point& _other) const { return !(*this == _other); }
			void swap(const point& _other) { cckit::swap(mX, _other.mX); cckit::swap(mY, _other.mY); }
		};
		struct rect {
			point mTopLeft, mBottomRight;
			rect() : mTopLeft(), mBottomRight() {}
			rect(const point& _topLeft, const point& _bottomRight)
				: mTopLeft(_topLeft), mBottomRight(_bottomRight) {}
			rect(const rect& _other)
				: rect(_other.mTopLeft, _other.mBottomRight) {}
			bool contain(const point& _pt) const {
				return !((_pt.mX < mTopLeft.mX) || (_pt.mX > mBottomRight.mX) || cckit::equal(_pt.mX, mBottomRight.mX)
					|| (_pt.mY < mTopLeft.mY) || (_pt.mY > mBottomRight.mY) || cckit::equal(_pt.mY, mBottomRight.mY));
			}
			bool intersect(const rect& _other) const {
				return !((mTopLeft.mX > _other.mBottomRight.mX) || (mBottomRight.mX < _other.mTopLeft.mX)
						|| (mTopLeft.mY > _other.mBottomRight.mY) || (mBottomRight.mY < _other.mTopLeft.mY)
						|| cckit::equal(mTopLeft.mX, _other.mBottomRight.mX) || cckit::equal(mBottomRight.mX, _other.mTopLeft.mX)
						|| cckit::equal(mTopLeft.mY, _other.mBottomRight.mY) || cckit::equal(mBottomRight.mY, _other.mTopLeft.mY));
			}
		};

		static constexpr size_type MAX_NODE_SIZE = MaxNodeSize;
		typedef cckit::deque<cckit::pair<value_type, point> > elemlist_type;

	public:
		quadtree() : mpElems(new elemlist_type()) {}
		explicit quadtree(const rect& _bounds)
			: mpElems(new elemlist_type()), mBounds(_bounds) {}
		~quadtree();

		bool insert(const value_type& _val, const point& _pt);
		void query_range(const rect& _range, elemlist_type& _list) const;
		size_type size() const;

		template<typename UnaryFunction0, typename NullaryFunction0, typename NullaryFunction1, typename NullaryFunction2>
		void verify(UnaryFunction0 _func0, NullaryFunction0 _func1, NullaryFunction1 _func2, NullaryFunction2 _func3) {
			if (mpElems) {
				for (const auto& elem : *mpElems)
					_func0(elem.first);
				_func1();
			}
			else {
				_func2();
				for (size_type i = 0; i < 4; ++i)
					mpSubtrees[i]->verify(_func0, _func1, _func2, _func3);
				_func3();
			}
		}

	private:
		void Subdivide();

	private:
		this_type* mpSubtrees[4];
		rect mBounds;
		elemlist_type* mpElems;
	};
}

namespace cckit
{
	template<typename T, size_t MaxNodeSize>
	inline quadtree<T, MaxNodeSize>::~quadtree() {
		if (mpElems)
			delete mpElems;
		else 
			for (size_type i = 0; i < 4; ++i)
				delete mpSubtrees[i];
	}

	template<typename T, size_t MaxNodeSize>
	bool quadtree<T, MaxNodeSize>::insert(const value_type& _val, const point& _pt) {
		if (!mBounds.contain(_pt))
			return false;

		if (mpElems) {
			if (mpElems->size() != MAX_NODE_SIZE) {
				for (const auto& elem : *mpElems)
					if (elem.second == _pt) return false;
				mpElems->push_back(cckit::pair<value_type, point>(_val, _pt));
				goto Proc0;
			}
			else
				Subdivide();
		}
		for (size_type i = 0; i < 4; ++i)
			if (mpSubtrees[i]->insert(_val, _pt)) break;
	Proc0:
		return true;
	}

	template<typename T, size_t MaxNodeSize>
	void quadtree<T, MaxNodeSize>::query_range(const rect& _range, elemlist_type& _list) const {
		if (mBounds.intersect(_range)) {
			if (mpElems) {
				for (const auto& elem : *mpElems)
					if (_range.contain(elem.second))
						_list.push_back(elem);
			}
			else
				for (size_type i = 0; i < 4; mpSubtrees[i]->query_range(_range, _list), ++i) {}
		}
	}

	template<typename T, size_t MaxNodeSize>
	typename quadtree<T, MaxNodeSize>::size_type 
		quadtree<T, MaxNodeSize>::size() const {
		if (!mpElems) {
			size_type sz = 0;
			for (size_type i = 0; i < 4; ++i)
				sz += mpSubtrees[i]->size();
			return sz;
		}
		return mpElems->size();
	}

	template<typename T, size_t MaxNodeSize>
	void quadtree<T, MaxNodeSize>::Subdivide() {
		float x0 = mBounds.mTopLeft.mX, y0 = mBounds.mTopLeft.mY
			, x2 = mBounds.mBottomRight.mX, y2 = mBounds.mBottomRight.mY
			, x1 = (x0 + x2) / 2, y1 = (y0 + y2) / 2;

		mpSubtrees[0] = new this_type(rect(point(x1, y0), point(x2, y1)));
		mpSubtrees[1] = new this_type(rect(point(x0, y0), point(x1, y1)));
		mpSubtrees[2] = new this_type(rect(point(x0, y1), point(x1, y2)));
		mpSubtrees[3] = new this_type(rect(point(x1, y1), point(x2, y2)));

		while (!mpElems->empty()) {
			for (size_type i = 0; i < 4; ++i) {
				auto back = mpElems->back();
				if (mpSubtrees[i]->insert(back.first, back.second)) break;
			}
			mpElems->pop_back();
		}
		delete mpElems;
		mpElems = nullptr;
	}
}

#endif // !CCKIT_QUADTREE_H