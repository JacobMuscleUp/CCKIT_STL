#ifndef CCKIT_MATH_H
#define CCKIT_MATH_H

namespace cckit
{
	float abs(float _arg) { return (_arg < 0.0f) ? -_arg : _arg; }
	double abs(double _arg) { return (_arg < 0) ? -_arg : _arg; }
}

#endif // !CCKIT_MATH_H