#ifndef LA_MATRIX_CPP
#define LA_MATRIX_CPP

#include "matrix.h"
#include "vector.h"

//Constructor using parameters
template<typename T>
LAMatrix<T>::LAMatrix(unsigned _rows, unsigned _cols, const T& _initial){
	mat.resize(_rows);
	for (unsigned i=0; i< mat.size(); i++) { //allowed since mat is already declared velctor
		mat[i].resize(_cols, _initial);
	}
	rows = _rows;
	cols = _cols; //assign given rows to private variables
}

//Constructor copying a LAMatrix to this
template<typename T>
LAMatrix<T>::LAMatrix(const LAMatrix<T>& rhs){
	mat = rhs.mat;
	rows = rhs.get_rows();
	cols = rhs.get_cols();
}

//Destructor
template<typename T>
LAMatrix<T>::~LAMatrix() {} //no memory allocation so done by compiler

//Assignment Operator
template<typename T>
LAMatrix<T>& /*lhs of operator*/ LAMatrix<T>::operator=(const LAMatrix<T>& rhs /*righthand*/) {
	if (&rhs == this) //this object
		return *this;  //* - values pointed by this where this is current object

	//this is a pointer (thus dereferenced to return values) 
	//it is a pointer of the current object
	
	unsigned new_rows = rhs.get_rows();
	unsigned new_cols = rhs.get_cols();

	mat.resize(new_rows); //same as this->mat.resize
	for (unsigned i=0; i<mat.size(); i++) {
		mat[i].resize(new_cols);
	}

	for (unsigned i=0; i<new_rows; i++){
		for (unsigned j=0; j<new_cols; j++){
			mat[i][j] = rhs(i,j);
		}
	}
	rows = new_rows;
	cols = new_cols;

	return *this;

}

/* Operators :
B + C
B calls the operator + giving as a "variable" C
B is -this- object so can access elements with this pointer
using the constructor "=" we can assign to another object 
So: A = //constructor// B + //B calls operator +// C //gives C as variable
*/

//addition of matrices
template<typename T>
//return by reference will create error
LAMatrix<T> LAMatrix<T>::operator+(const LAMatrix<T>& rhs) {
	LAMatrix total(rows,cols,0.0);
	
	for (unsigned i=0; i<rows; i++) {
		for (unsigned j=0; j<cols; j++) {
			total(i,j) = this->mat[i][j] + rhs(i,j);
		}
	}
	return total;
}

template<typename T>
LAMatrix<T> LAMatrix<T>::operator-(const LAMatrix<T>& rhs){
	LAMatrix total(rows,cols,0.0);
	
	for (unsigned i=0; i<rows; i++){
		for (unsigned j=0; j<cols; j++){
			total(i,j) = this->mat[i][j] + rhs(i,j);
		}
	}
	return total;
}

//multiplication
template<typename T>
LAMatrix<T> LAMatrix<T>::operator*(const LAMatrix<T>& rhs) {
	unsigned cols = rhs.get_cols();
	LAMatrix mult(this->rows,cols,0.0);
	
	for (unsigned i=0; i<this->get_rows(); i++) {
		for (unsigned j=0; j<cols; j++) {
			for (unsigned k=0; k<this->get_cols(); k++){
				mult(i,j) = mult(i,j) + this->mat[i][k] * rhs(k,j);
			}
		}
	}
	
	return mult;
}

//Row-Indexed scheme multiplication
template<typename T>
vector<T> LAMatrix<T>::operator->*(const vector<T>& rhs) {
	//check dimensions??
	vector<T> result(rhs.size());
	
	for (int i=0; i<rhs.size(); i++) {
		result[i] = this->mat[i][0]*rhs[i];
		for (int k = this->mat[i][1]; k<=this->mat[i+1][1]-1; k++) {
			result[i] = result[i] + this->mat[k][0] * rhs[this->mat[k][1]];
		}
	}
	
	return result;
}

template<typename T>
LAVector<T> LAMatrix<T>::operator->*(const LAVector<T>& rhs) {
	LAVector<T> result(rhs.get_size(),0.0);

	for (int i=0; i<rhs.get_size(); i++) {
		result(i) = this->mat[i][0]*rhs(i);
		for (int k = this->mat[i][1]; k<=this->mat[i+1][1]-1; k++) {
			result(i) = result(i) + this->mat[k][0] * rhs(this->mat[k][1]);
		}
	}
	return result;
}

//scalar operators -OVERLOADING-
template<typename T>
LAMatrix<T> LAMatrix<T>::operator+(const T& rhs) {
	LAMatrix total(rows,cols,0.0);
	
	for (unsigned i=0; i<rows; i++) {
		for (unsigned j=0; j<cols; j++) {
			total(i,j) = this->mat[i][j] + rhs;
		}
	}
	return total;
}

template<typename T>
LAMatrix<T> LAMatrix<T>::operator-(const T& rhs) {
	LAMatrix total(rows,cols,0.0);
	
	for (unsigned i=0; i<rows; i++) {
		for (unsigned j=0; j<cols; j++) {
			total(i,j) = this->mat[i][j] - rhs;
		}
	}
	return total;
}
template<typename T>
LAMatrix<T> LAMatrix<T>::operator*(const T& rhs) {
	LAMatrix total(rows,cols,0.0);
	
	for (unsigned i=0; i<rows; i++) {
		for (unsigned j=0; j<cols; j++) {
			total(i,j) = this->mat[i][j] * rhs;
		}
	}
	return total;
}
template<typename T>
LAMatrix<T> LAMatrix<T>::operator/(const T& rhs) {
	LAMatrix total(rows,cols,0.0);
	
	for (unsigned i=0; i<rows; i++) {
		for (unsigned j=0; j<cols; j++) {
			total(i,j) = this->mat[i][j] / rhs;
		}
	}
	return total;
}

//template<typename T>
//LAMatrix<T> LAMatrix<T>::RowScheme(const LAMatrix<T> array) {
//	LAMatrix rowscheme(


//Element access
template<typename T>
T& LAMatrix<T>::operator() (const unsigned& row, const unsigned& col){
	return this->mat[row][col];
}

template<typename T>
const T& LAMatrix<T>::operator() (const unsigned& row, const unsigned& col) const {
	return this->mat[row][col];
}

//Pointer access
template<typename T>
T& LAMatrix<T>::get_element(const unsigned& row, const unsigned& col) {
	return this->mat[row][col];
}

template<typename T>
unsigned LAMatrix<T>::get_rows() const {
	return this->rows; //vector rows access
}

template<typename T>
unsigned LAMatrix<T>::get_cols() const {
	return this->cols;
}

#endif
