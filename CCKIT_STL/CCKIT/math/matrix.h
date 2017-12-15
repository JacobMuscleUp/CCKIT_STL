#ifndef CCKIT_MATRIX_H
#define CCKIT_MATRIX_H

#include "../internal/config.h"
#include <initializer_list>
#include <cmath>
//#include "../math.h"

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
		matrix(const matrix<Row, Col>& _other) {
			for (int row = 0; row < Row; ++row)
				for (int col = 0; col < Col; ++col)
					mArray[row][col] = _other.mArray[row][col];
		}

		double at(int _row, int _col) const {
			return mArray[_row][_col];
		}
		double& at(int _row, int _col) {
			return mArray[_row][_col];
		}

		const double* operator[](int _row) const {
			return mArray[_row];
		}
		double* operator[](int _row) {
			return mArray[_row];
		}

		matrix<Col, Row> transpose() const {
			matrix<Col, Row> transposeMat;
			for (int row = 0; row < Row; ++row)
				for (int col = 0; col < Col; ++col)
					transposeMat.mArray[row][col] = mArray[col][row];
			return transposeMat;
		}

		template<typename = cckit::enable_if_t<(Row == Col)> >
		matrix<Row, Col> inverse() const {
			double det = determinant(*this);

			if (det != 0) {
				matrix<Row, Col> inverseMat;
				for (int row = 0; row < Row; ++row)
					for (int col = 0; col < Col; ++col)
						inverseMat.mArray[row][col] = transpose().cofactor(row, col) / det;
				return inverseMat;
			}
			return *this;
		}

		template<typename = cckit::enable_if_t<(Row == Col)> >
		matrix<Row, Col> inverse_gauss_jordan_elimination() const {
			matrix<Row, Col> inverseMat, auxMat = *this;

			for (int row = 0; row < Row; ++row)
				for (int col = 0; col < Col; ++col)
					inverseMat.mArray[row][col] = ((row != col) ? 0 : 1);
			
			for (int j = 0; j < Col; ++j) {
				int i = 0;

				double largestAbs = 0;
				for (int row = j; row < Row; ++row) {
					double absVal = cckit::abs(auxMat.mArray[row][j]);
					if (largestAbs < absVal) {
						largestAbs = absVal;
						i = row;
					}
				}
				if (largestAbs == 0)
					return *this;
				if (i != j) {
					for (int col = 0; col < Col; ++col) {
						cckit::swap(auxMat.mArray[i][col], auxMat.mArray[j][col]);
						cckit::swap(inverseMat.mArray[i][col], inverseMat.mArray[j][col]);
					}
				}

				for (int col = Col - 1; col >= 0; --col) {
					inverseMat.mArray[j][col] /= auxMat.mArray[j][j];
					if (col > j)
						auxMat.mArray[j][col] /= auxMat.mArray[j][j];
				}
				auxMat.mArray[j][j] = 1; // auxMat.mArray[j][j] /= auxMat.mArray[j][j];

				for (int r = 0; r < Row; ++r) {
					if (r == j) continue;

					for (int col = Col - 1; col >= 0; --col) {
						inverseMat.mArray[r][col] -= inverseMat.mArray[j][col] * auxMat.mArray[r][j];
						if (col > j)
							auxMat.mArray[r][col] -= auxMat.mArray[j][col] * auxMat.mArray[r][j];
					}
					auxMat.mArray[r][j] = 0; // auxMat.mArray[r][j] -= auxMat.mArray[j][j] * auxMat.mArray[r][j];
				}
			}
			
			return inverseMat;
		}

		double cofactor(int _row, int _col) const {
			return std::pow(-1, _row + _col) * determinant(minor(_row, _col));
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

	template<int i, int j, int k>
	matrix<i, k> multiply(const matrix<i, j>& _mat0, const matrix<j, k>& _mat1) {
		matrix<i, k> resultMat;

		for (int row = 0; row < i; ++row) {
			for (int col = 0; col < k; ++col) {
				resultMat.at(row, col) = 0;
				for (int jCurrent = 0; jCurrent < j; ++jCurrent)
					resultMat.at(row, col) += _mat0.at(row, jCurrent) * _mat1.at(jCurrent, col);
			}
		}
		return resultMat;
	}

	template<int Length, typename = cckit::enable_if_t<(Length > 0)> >
	double determinant(const matrix<Length, Length>& _mat) {
		double result = 0;
		for (int col = 0; col < _mat.COL; ++col) 
			result += _mat.cofactor(0, col) * _mat.at(0, col);
		return result;
	}
	double determinant(const matrix<1, 1>& _mat) {
		return _mat.at(0, 0);
	}
}

#endif // !CCKIT_MATRIX_H