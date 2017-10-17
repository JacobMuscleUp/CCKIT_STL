#ifndef CCKIT_MATRIX_H
#define CCKIT_MATRIX_H

#include "../internal/config.h"
#include <initializer_list>
#include <cmath>

namespace cckit 
{
	template<int Row, int Col>
	struct matrix
	{
		double mArray[Row][Col];
		const static int ROW = Row;
		const static int COL = Col;

		matrix() {}
		matrix(std::initializer_list<std::initializer_list<double> > _ilist) 
		{
			auto rowIter = _ilist.begin();
			for (int row = 0; row < Row; ++row, ++rowIter) {
				auto colIter = (*rowIter).begin();
				for (int col = 0; col < Col; ++col, ++colIter) {
					mArray[row][col] = *colIter;
				}
			}
		}

		double at(int _row, int _col) const {
			return mArray[_row][_col];
		}
		double& at(int _row, int _col) {
			return mArray[_row][_col];
		}

		template<typename = cckit::enable_if_t<(Row > 1 && Col > 1)> >
		matrix<Row - 1, Col - 1> minor(int _row, int _col) const {
			matrix<Row - 1, Col - 1> minorMat;
			for (int row = 0; row < _row; ++row) {
				for (int col = 0; col < _col; ++col)
					minorMat.mArray[row][col] = mArray[row][col];
				for (int col = Col - 2; col >= _col; --col)
					minorMat.mArray[row][col] = mArray[row][col + 1];
			}
			for (int row = Row - 2; row >= _row; --row) {
				for (int col = 0; col < _col; ++col)
					minorMat.mArray[row][col] = mArray[row + 1][col];
				for (int col = Col - 2; col >= _col; --col)
					minorMat.mArray[row][col] = mArray[row + 1][col + 1];
			}
			return minorMat;
		}
	};

	template<int Length, typename = cckit::enable_if_t<(Length > 0)> >
	double determinant(const matrix<Length, Length>& _mat) {
		double result = 0;
		for (int col = 0; col < _mat.COL; ++col) 
			result += std::pow(-1, col) * determinant(_mat.minor(0, col)) * _mat.at(0, col);
		return result;
	}
	double determinant(const matrix<1, 1>& _mat) {
		return _mat.at(0, 0);
	}
}

#endif // !CCKIT_MATRIX_H