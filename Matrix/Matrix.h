#include<iostream>
#include<vector>
#include<cmath>
#include <stdexcept>
using namespace std;

template <typename T>
class Matrix
{
private:
	int n_row;
	int n_col;
	vector<vector<T>> element;

public:
	Matrix(int row, int col) : n_row(row), n_col(col), element(row, vector<T>(col)) {}
	int rows() const;
	int cols() const;
	bool is_square() const;
	T& operator()(int i, int j);
	const T& operator()(int i, int j) const;
	Matrix<T> operator+(const Matrix<T>& matB);
	Matrix<T> operator*(const T& scalar);
	Matrix<T> operator*(const Matrix<T>& matB);
	T det()const;
	Matrix<T> inv();

};

template <typename T>
int Matrix<T>::rows() const
{
	return n_row;
}

template <typename T>
int Matrix<T>::cols() const
{
	return n_col;
}
template <typename T>
bool Matrix<T>::is_square()const
{
	return n_row == n_col;
}

template <typename T>
T& Matrix<T>::operator()(int i, int j)
{
	if (i < 0 || i >= n_row || j < 0 || j >= n_col)
	{
		cerr << "Matrix index out of range " << endl;
	}
	return element[i][j];
}

template <typename T>
const T& Matrix<T> :: operator()(int i, int j) const
{
	if (i < 0 || i >= n_row || j < 0 || j >= n_col)
	{
		cerr << "Matrix index out of range " << endl;
	}
	return element[i][j];
}

template <typename T>
Matrix<T> Matrix<T>:: operator+(const Matrix<T>& matB)
{
	if (n_row != matB.n_row || n_col != matB.n_col)
	{
		cerr << "Matrix dimensions do not match for addition." << endl;
	}

	else
	{
		Matrix<T> matC(n_row, n_col);
		for (int i = 0; i < n_row; i++)
		{
			for (int j = 0; j < n_col; j++)
			{
				matC(i, j) = element[i][j] + matB(i, j);
			}
		}
		return matC;
	}
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& scalar)
{
	Matrix<T> matC(n_row, n_col);
	for (int i = 0; i < n_row; i++)
	{
		for (int j = 0; j < n_col; j++)
		{
			matC(i, j) = scalar * element[i][j];
		}
	}

	return matC;
}

template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& mat)
{
	Matrix<T> matC(mat.rows(), mat.cols());
	for (int i = 0; i < mat.rows(); i++)
	{
		for (int j = 0; j < mat.cols(); j++)
		{
			matC(i, j) = scalar * mat(i, j);
		}
	}
	return matC;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& matB)
{
	Matrix<T> matC(n_row, matB.cols());
	if (n_col != matB.rows())
	{
		cerr << "Matrix dimensions do not match for multiplication." << endl;
	}

	else
	{
		for (int i = 0; i < n_row; i++)
		{
			for (int j = 0; j < matB.n_col; j++)
			{
				double sum = 0;
				for (int k = 0; k < n_col; k++)
				{
					sum += element[i][k] * matB.element[k][j];
				}
				matC(i, j) = sum;
			}
		}
	}

	return matC;
}

template <typename T>
T Matrix<T>::det() const
{
	if (!is_square())
	{
		cerr << "Matrix is not square, determinant cannot be computed." << endl;
	}

	else
	{
		vector<vector<T>> matB = element;
		int n = n_row;
		T det = 1;

		for (int k = 0; k < n; k++)
		{
			int maxRow = k;
			for (int i = k + 1; i < n; i++)
			{
				if (abs(matB[i][k]) > abs(matB[maxRow][k]))
				{
					maxRow = i;
				}
			}

			if (maxRow != k)
			{
				swap(matB[k], matB[maxRow]);
				det = -det;
			}

			if (matB[k][k] == 0)
			{
				return 0;
			}

			for (int i = k + 1; i < n; i++)
			{
				T factor = matB[i][k] / matB[k][k];
				for (int j = k + 1; j < n; j++)
				{
					matB[i][j] = matB[i][j] - factor * matB[k][j];
				}
			}

			det = det * matB[k][k];
		}

		return det;
	}
}

template <typename T>
Matrix<T> Matrix<T>::inv()
{
	if (!is_square())
	{
		cerr << "Inverse is not defined since it's not a square matrix." << endl;
		return *this;
	}

	int n = n_row;
	Matrix<T> inv(n, n);
	Matrix<T> mat = *this;

	for (int i = 0; i < n; i++)
	{
		inv(i, i) = 1;
	}

	for (int k = 0; k < n; k++)
	{
		T pivot = mat(k, k);
		if (pivot == 0)
		{
			cerr << "Matrix is singular." << endl;
			return *this;
		}

		for (int j = 0; j < n; j++)
		{
			mat(k, j) /= pivot;
			inv(k, j) /= pivot;
		}

		for (int i = 0; i < n; i++)
		{
			if (i == k)
				continue;

			T factor = mat(i, k);
			for (int j = 0; j < n; j++)
			{
				mat(i, j) -= factor * mat(k, j);
				inv(i, j) -= factor * inv(k, j);
			}
		}
	}

	return inv;
}
