#ifndef CCKIT_RANDOM_H
#define CCKIT_RANDOM_H

#include <cassert>
#include <cstdlib>
#include <ctime>
#include "internal/config.h"

namespace cckit
{
	int rand_int(int _min, int _max)
	{
#ifdef CCKIT_DEBUG
		assert(_min < _max);
#endif 
		static int seed = 0;
		if (seed == 0) {
			seed = static_cast<int>(time(0));
			srand(seed);
		}

		return _min + rand() % (_max - _min);
	}
}// namespace cckit

#endif // !CCKIT_RANDOM_H


