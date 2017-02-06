#ifndef LA_VECTOR_H
#define LA_VECTOR_H

#include <vector>

using namespace std;

template <typename T> class LAVector {
	private:
		vector<T > mat;
		unsigned rows;

	public:
		LAVector(unsigned _rows, const T& _initial);
		LAVector(const LAVector<T>& rhs);
    	LAVector(unsigned _rows, const char a, const T& min, const T& max); 
        virtual ~LAVector();
	
		//Operator overload
		LAVector<T>& operator=(const LAVector<T>& rhs);
	    LAVector<T>& operator=(const std::vector<T>& rhs);

		//Math operations
		LAVector<T> operator+(const LAVector<T>& rhs);
        LAVector<T>& operator+=(const LAVector<T>& rhs);
		//LAMatrix<T> operator+(const LAMatrix<T>& rhs);
		LAVector<T> operator-(const LAVector<T>& rhs);
        LAVector<T>& operator-=(const LAVector<T>& rhs);
		T operator*(const LAVector<T>& rhs);
		//vector<T> operator->*(const vector<T>& rhs);
	
		//Matrix scalar operations
		LAVector<T> operator+(const T& rhs);
		LAVector<T> operator-(const T& rhs);
		LAVector<T> operator*(const T& rhs);
		LAVector<T> operator/(const T& rhs);
		
		//element operators
		T& operator() (const unsigned& row);
		const T& operator() (const unsigned& row) const;
		T& get_element(const unsigned& row);
		
		unsigned get_size() const;
        void inverse(LAVector<T> v);
        void inverse(std::vector<T> v);
};

#include "vector.cpp"

#endif
