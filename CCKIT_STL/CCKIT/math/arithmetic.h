#ifndef CCKIT_ARITHMETIC_H
#define CCKIT_ARITHMETIC_H

namespace cckit
{
	int add(int a, int b) {
		while (a != 0) {// until no more carriage exists
			int c = ((a & b) << 1);
			b = a ^ b;
			a = c;
		}
		return b;
	}

	int mult(int a, int b) {
		int c = 0;
		while (b != 0) {// until every bit of b has been used to multiply with a
			if ((b & 1) != 0)
				c = add(c, a);
			a <<= 1;
			b >>= 1;
		}
		return c;
	}
}

#endif