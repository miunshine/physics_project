#ifndef LA_VECTOR_CPP
#define LA_VECTOR_CPP

#include "vector.h"


//Constructor using parameters
template<typename T>
LAVector<T>::LAVector(unsigned _rows, const T& _initial){
	mat.resize(_rows, _initial);
	rows = _rows;
}

//Constructor copying a LAMatrix to this
template<typename T>
LAVector<T>::LAVector(const LAVector<T>& rhs){
	mat = rhs.mat;
	rows = rhs.get_size();
}

//Destructor
template<typename T>
LAVector<T>::~LAVector() {} //no memory allocation so done by compiler

//Assignment Operator
template<typename T>
LAVector<T>& /*lhs of operator*/ LAVector<T>::operator=(const LAVector<T>& rhs /*righthand*/) {
	if (&rhs == this) //this object
		return *this;  //* - values pointed by this where this is current object

	//this is a pointer (thus dereferenced to return values) 
	//it is a pointer of the current object
	
	unsigned new_rows = rhs.get_size();

	mat.resize(new_rows); //same as this->mat.resize

	for (unsigned i=0; i<new_rows; i++){
			mat[i] = rhs(i);
	}
	
	rows = new_rows;

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
LAVector<T> LAVector<T>::operator+(const LAVector<T>& rhs) {
	LAVector total(rows,0.0);
	
	for (unsigned i=0; i<rows; i++) {
			total(i) = this->mat[i] + rhs(i);
	}
	return total;
}

template<typename T>
LAVector<T> LAVector<T>::operator-(const LAVector<T>& rhs){
	LAVector total(rows,0.0);
	
	for (unsigned i=0; i<rows; i++){
			total(i) = this->mat[i] + rhs(i);
	}
	return total;
}

//multiplication dot
template<typename T>
T& LAVector<T>::operator*(const LAVector<T>& rhs) {
	T& mult;
	for (unsigned i=0; i<this->get_rows(); i++) {
				mult =mult + this->mat[i] * rhs(i);
	}
	
	return mult;
}

//Row-Indexed scheme multiplication

//scalar operators -OVERLOADING-
template<typename T>
LAVector<T> LAVector<T>::operator+(const T& rhs) {
	LAVector total(rows,0.0);
	
	for (unsigned i=0; i<rows; i++) {
			total(i) = this->mat[i] + rhs;
	}
	return total;
}

template<typename T>
LAVector<T> LAVector<T>::operator-(const T& rhs) {
	LAVector total(rows,0.0);
	
	for (unsigned i=0; i<rows; i++) {
			total(i) = this->mat[i]- rhs;
	}
	return total;
}
template<typename T>
LAVector<T> LAVector<T>::operator*(const T& rhs) {
	LAVector total(rows,0.0);
	
	for (unsigned i=0; i<rows; i++) {
			total(i) = this->mat[i]* rhs;
	}
	return total;
}
template<typename T>
LAVector<T> LAVector<T>::operator/(const T& rhs) {
	LAVector total(rows,0.0);
	
	for (unsigned i=0; i<rows; i++) {
			total(i) = this->mat[i] / rhs;
	}

	return total;
}

//template<typename T>
//LAMatrix<T> LAMatrix<T>::RowScheme(const LAMatrix<T> array) {
//	LAMatrix rowscheme(


//Element access
template<typename T>
T& LAVector<T>::operator() (const unsigned& row){
	return this->mat[row];
}

template<typename T>
const T& LAVector<T>::operator() (const unsigned& row) const {
	return this->mat[row];
}

//Pointer access
template<typename T>
T& LAVector<T>::get_element(const unsigned& row) {
	return this->mat[row];
}

template<typename T>
unsigned LAVector<T>::get_size() const {
	return this->rows; //vector rows access
}


#endif
