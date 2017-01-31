#ifndef LA_MATRIX_H
#define LA_MATRIX_H

#include <vector>
#include "vector.h"

using namespace std;

template <typename T> class LAMatrix {
	private:
		vector<vector<T> > mat;
		unsigned rows;
		unsigned cols;

	public:
		LAMatrix(unsigned _rows, unsigned _cols, const T& _initial);
		LAMatrix(const LAMatrix<T>& rhs);
		virtual ~LAMatrix();
	
		//Operator overload
		LAMatrix<T>& operator=(const LAMatrix<T>& rhs);
		
		//Math operations
		LAMatrix<T> operator+(const LAMatrix<T>& rhs);
		//LAMatrix<T> operator+(const LAMatrix<T>& rhs);
		LAMatrix<T> operator-(const LAMatrix<T>& rhs);
		LAMatrix<T> operator*(const LAMatrix<T>& rhs);
		vector<T> operator->*(const vector<T>& rhs);
		LAVector<T> operator->*(const LAVector<T>& rhs);
		
		//Matrix scalar operations
		LAMatrix<T> operator+(const T& rhs);
		LAMatrix<T> operator-(const T& rhs);
		LAMatrix<T> operator*(const T& rhs);
		LAMatrix<T> operator/(const T& rhs);
		
		//element operators
		T& operator() (const unsigned& row, const unsigned& col);
		const T& operator() (const unsigned& row, const unsigned& col) const;
		T& get_element(const unsigned& row, const unsigned& col);
		
		unsigned get_rows() const;
		unsigned get_cols() const;

};

#include "matrix.cpp"

#endif
