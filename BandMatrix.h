#include <vector>
#include <map>
#include <algorithm>

#include "vector.h"

class BandMatrix;

class proxy {
    private:
	    BandMatrix& m_mat;
	    int m_diff, m_min;
    public:
	    proxy(BandMatrix& mat, int diff, int min) : m_mat(mat), m_diff(diff), m_min(min) {}
	    double& operator=(float el);
	    operator double() const;
};

class BandMatrix{
    public:
        //definitions inside class
    	typedef std::map<int, std::vector<double> > SparseBands;
    	typedef SparseBands::iterator iterator;
    	typedef SparseBands::const_iterator const_iterator;
    	typedef SparseBands::value_type band_value_type;
    	typedef SparseBands::reference band_reference;
    	typedef SparseBands::const_reference band_const_reference;
        //typedef SparseBands::transpose transpose;
    private:
	    unsigned m_N;           //dimension of bandmatrix
	    SparseBands m_elements; //BAND matrix definition
    	friend class proxy;     //define another class to be used inside this one
    public:
    	BandMatrix(unsigned N);       //initialised constructor
    	proxy operator()(int i, int j);

    	double operator()(int i, int j) const; //const -> no change of members
        std::vector<double> get_row(int i);
        std::vector<double> get_col(int j);
        BandMatrix operator+(const BandMatrix& v);
    	BandMatrix& operator+=(const BandMatrix& v);
        BandMatrix operator-(const BandMatrix& v);
        BandMatrix& operator-=(const BandMatrix& v);
        std::vector<double> operator*(const std::vector<double>& v);
        LAVector<double> operator*(const LAVector<double>& v);
        std::vector<double> operator->*(const std::vector<double>& v);
        LAVector<double> operator->*(const LAVector<double>& v);

        std::vector<double> diag();
        //LAVector<double>    diag(const BandMatrix M);
        unsigned size() const;
//	    void ApplyBoundary(const unsigned r);


    	const_iterator begin() const; 
    	const_iterator end() const; 
    	iterator begin();
    	iterator end();
	
};
BandMatrix createFromStencil(unsigned N, double stencil[3][3],LAVector<int> flagv);
