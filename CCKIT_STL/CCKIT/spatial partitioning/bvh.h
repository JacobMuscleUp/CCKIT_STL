#ifndef CCKIT_BVH_H
#define CCKIT_BVH_H

#include <vector>

namespace cckit
{
	template<typename Obj>
	float Extent(const Obj& _obj) {
		return -1.0f;
	}

	template<typename T, size_t Dim>
	class AABB
	{
	public:
		AABB(T _min[Dim], T _max[Dim]);
	private:
		T mMin[Dim];
		T mMax[Dim];
	};
	template<typename T, size_t Dim>
	AABB<T, Dim>::AABB(T _min[Dim], T _max[Dim]) {
		for (size_t i = 0; i < Dim; ++i) {
			mMin[i] = _min[i];
			mMax[i] = _max[i];
		}
	}

	template<typename Obj>
	class BvhNode
	{
	private:
		bool mLeaf;
		AABB* mpBox;
		//std::vector
	};
}

#endif // !CCKIT_BVH_H