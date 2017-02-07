#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

#include "vector.h"
#include "BandMatrix.h"

using namespace std;

BandMatrix::BandMatrix(unsigned N) : m_N(N) { }        //initialised constructor
   
//Cannot use standard element access (this->mat[][]) because of band
//form. Use a proxy class that access the band matrix and returns
//element at correct position. Since the band matrix does not have
//zeros stored, the proxy needs to return a zero value if the
//requested element is not in the matrix. 

proxy BandMatrix::operator()(int i, int j)
{
    return proxy(*this, i - j, std::min(i, j)); //initialise proxy
}

double BandMatrix::operator()(int i, int j) const //const -> no change of members
{
    int diff = i - j;
    //iterator (pointer) to element pointed by key - the key in the
    //specific band matrix is the difference of the indexes.
    const auto& it = m_elements.find(diff);
    //if the iterator did not find the difference it reaches map::end
    //then the element is not included in band matrix - it is zero
    if (m_elements.end() == it)
    	return 0.f;
    //it->first is the key, it->second is the vector
    const auto& band = it->second; 
    return band[std::min(i, j)];  //return the element of the vector
    
}
std::vector<double> BandMatrix::get_row(int i)
{
    int n = this->size();
    std::vector<double> result;
    for (int j=0; j<n; j++){
        int diff = i - j;
        const auto& it = m_elements.find(diff);
        if (m_elements.end() == it)
            result.push_back(0.f);
        else
        {
            const auto& band = it->second;
            int minimum = std::min(i, j);
            result.push_back(band[minimum]);
        }
    }
    return result;
}

std::vector<double> BandMatrix::get_col(int j)
{
    int n = this->size();
    std::vector<double> result;
    for (int i=0; i<n; i++) {
        int diff = i - j;
        const auto& it = m_elements.find(diff);
        if (m_elements.end() == it)
            result.push_back(0.f);
        else
        {
            const auto& band = it->second;
            int minimum = std::min(i,j);
            result.push_back(band[minimum]);
        }
    }
    return result;
}

BandMatrix BandMatrix::operator+(const BandMatrix& v)
{
   	assert(this->m_elements[0].size() == v.size());               //check sizes
    BandMatrix Result(v.size());
    for (const auto& band: this->m_elements) {
        int diff = band.first; 
	    int min = 0;            
        for (const auto m: band.second) {
	        int i = (diff >= 0) ? min + diff :        min;
	        int j = (diff >= 0) ?        min : min - diff; 
	        if (m + v(i,j) != 0) Result(i,j) = m + v(i,j);
	        min = min+1;
	    }
    }
    return Result;
}

BandMatrix& BandMatrix::operator+=(const BandMatrix& v)
{
    assert(this->m_elements[0].size() == v.size());
    for (const auto& band: this->m_elements) {
        int diff = band.first; 
	    int min = 0;            
        for (const auto m: band.second) {
	        int i = (diff >= 0) ? min + diff :        min;
	        int j = (diff >= 0) ?        min : min - diff; 
	        double result = m + v(i,j);
            if (result != 0) 
                this->m_elements[diff][min] = result; 
	        min = min+1;
	    }
    }
    
    return *this;
}

BandMatrix BandMatrix::operator-(const BandMatrix& v)
{
   	assert(this->m_elements[0].size() == v.size());               //check sizes
    BandMatrix Result(v.size());
    for (const auto& band: this->m_elements) {
        int diff = band.first; 
	    int min = 0;            
        for (const auto m: band.second) {
	        int i = (diff >= 0) ? min + diff :        min;
	        int j = (diff >= 0) ?        min : min - diff; 
	        if (m - v(i,j) != 0) Result(i,j) = m - v(i,j);
	        min = min + 1;
	    }
    }
    return Result;
}

BandMatrix& BandMatrix::operator-=(const BandMatrix& v)
{
    assert(this->m_elements[0].size() == v.size());
    for (const auto& band: this->m_elements) {
        int diff = band.first; 
	    int min = 0;            
        for (const auto m: band.second) {
	        int i = (diff >= 0) ? min + diff :        min;
	        int j = (diff >= 0) ?        min : min - diff; 
	        double result = m - v(i,j);
            if (result != 0) 
                this->m_elements[diff][min] = result; 
	        min = min+1;
	    }
    }
    
    return *this;
}
std::vector<double> BandMatrix::operator*(const std::vector<double>& v)
{
   	assert(this->m_elements[0].size() == v.size());               //check sizes
    std::vector<double> retVal(v.size(), 0.f);
    for (const auto& band: this->m_elements) {
        int diff = band.first; 
	    int min = 0;            
        for (const auto m: band.second) {
	        int i = (diff >= 0) ? min + diff :        min;
	        int j = (diff >= 0) ?        min : min - diff; 
	        retVal[i] += m * v[j]; 
	        ++min;
	    }
    }
    return retVal;
}

LAVector<double> BandMatrix::operator*(const LAVector<double>& v)
{
   	assert(this->m_elements[0].size() == v.get_size());               //check sizes
    LAVector<double> retVal(v.get_size(), 0.f);
    //for the number of "pairs" in M - pair is key and vector
    for (const auto& band: this->m_elements) {
        int diff = band.first;   //band.first stores the key- ie the diff i,j
	    int min = 0;             //default min ie the diagonal
	    //for elements in band.second ie vector
        for (const auto m: band.second) {
            //if statement INSIDE int definition. 
	        int i = (diff >= 0) ? min + diff :        min;
	        int j = (diff >= 0) ?        min : min - diff; 
	        retVal(i) += m * v(j); // retVal_i += m_ij v_j
	        ++min;
	    }
    }
    return retVal;
}

//Multiplication of transpose of band matrix with vector
std::vector<double> BandMatrix::operator->*(const std::vector<double>& v)
{   
    assert(this->m_elements[0].size() == v.size());
    std::vector<double> retVal(v.size(), 0.f);
    for (const auto& band: this->m_elements) {
        int diff = band.first;
        int min  = 0;
        for (const auto m: band.second) {
            int i = (diff >= 0) ?        min  : min - diff;
            int j = (diff >= 0) ? min + diff  :        min;
            retVal[i] += m * v[j];
            ++min;
        }
    }
    return retVal;
}

LAVector<double> BandMatrix::operator->*(const LAVector<double>& v)
{   
    assert(this->m_elements[0].size() == v.get_size());
    LAVector<double> retVal(v.get_size(), 0.f);
    for (const auto& band: this->m_elements) {
        int diff = band.first;
        int min  = 0;
        for (const auto m: band.second) {
            int i = (diff >= 0) ?        min  : min - diff;
            int j = (diff >= 0) ? min + diff  :        min;
            retVal(i) += m * v(j);
            ++min;
        }
    }
    return retVal;
}

std::vector<double> BandMatrix::diag()
{
    return this->m_elements[0];
}
/*
LAVector<double> BandMatrix::diag(BandMatrix M)
{
    LAVector<double> result = M.m_elements[0];
    return result;
}
*/
unsigned BandMatrix::size() const { return m_N; }
//map::end, map::begin definitions in case band.begin() is called
/*
void BandMatrix::ApplyBoundary(const unsigned r) 
{
    this->m_elements[0][r] = 1;
    for (const auto& band: this->m_elements) {
        int diff = band.first;
        if (diff == 0) {
            cout << diff << " changing to 1"<<endl;
            this->m_elements[diff][r] = 1;
        }
        else if (diff<0 && ((r-diff)>=0)){
            cout << chang
            this->m_elements[diff][r-diff] = 0;
        else
            if (r < m_elements[diff].size()) this->m_elements[diff][r] = 0;
    }
}
*/
BandMatrix::const_iterator BandMatrix::begin() const { return m_elements.begin(); }
BandMatrix::const_iterator BandMatrix::end() const { return m_elements.end(); }
BandMatrix::iterator BandMatrix::begin() { return m_elements.begin(); }
BandMatrix::iterator BandMatrix::end() { return m_elements.end(); }
	

//Access of an element is done by the the difference between i,j.
//This gives the key of the map object (the first int - can use map::find)
//Because of the band form the element of the vector (second type of map) is
//the minimum between i,j. (Drawing the band form and the actual confirms
//this)

//MEMBER FUNCTIONS OF PROXY CLASS
//These are used when user calls band(i,j).
//eg1 band(i,j) = 1; () operator calls BandMatrix class. This returns a proxy
//which is then equal to 1. = operator calls proxy class. Then normal element
//access of band matrix (as above) applies. !If band is empty then the key is
//to a currently non-existed band (previously zero off diagonal) which is
//changed. 
//eg2 a = band(i,j); This is just access to element. Again proxy is returned
//BUT variable a might be a double (or type T) which means that an operator
//double() is needed to return element in its specific value that is a float.
//So t

double& proxy::operator=(float el) {
	auto& band = m_mat.m_elements[m_diff]; //key to access map
	if (band.empty())
		//m_N - abs(m_diff) is the size of off diagonal vector
                band.resize(m_mat.m_N - std::abs(m_diff), 0.f);
	return (band[m_min] = el); //returns requested element
}

proxy::operator double() const {
	//similar to operator() in bandmatrix but returns a double.
        const auto& it = m_mat.m_elements.find(m_diff);
	if (m_mat.m_elements.end() == it)
		return 0.f;
	const auto& band = it->second;
	return band[m_min];
}

BandMatrix createFromStencil(unsigned N, double stencil[3][3],LAVector<int> flagv)
{
    //N is the grid SIDE size -> N*N is the diagonal size, N^4 is the matrix size 
    //Note that the band matrix is emtpy. When assigning a value the key is created.

    BandMatrix retVal(N * N);
    for (int i = 0; i < int(N); ++i) {
    	for (int j = 0; j < int(N); ++j) {
    	    unsigned idx = i * int(N) + j;
    	    if (i - 1 >= 0) {
    		//3 values along columns
    		if (j - 1 >= 0 && stencil[0][0] && flagv(idx)) retVal(idx, idx - int(N) - 1) = stencil[0][0];
    		if (stencil[0][1] && flagv(idx)) retVal(idx, idx - int(N)) = stencil[0][1];
    		if (j + 1 < int(N) && stencil[0][2] && flagv(idx)) retVal(idx, idx - int(N) + 1) = stencil[0][2];
    	    }
    	    if (j - 1 >= 0 && stencil[1][0] &&flagv(idx)) retVal(idx, idx - 1) = stencil[1][0];
    	    if (stencil[1][1] && flagv(idx)) retVal(idx, idx) = stencil[1][1]; //diagonal
            if (stencil[1][1] && flagv(idx) == 0) retVal(idx,idx) = 1.;
    	    if (j + 1 < int(N) && stencil[1][2] &&flagv(idx)) retVal(idx, idx + 1) = stencil[1][2];
    	    if (i + 1 < int(N)) {
    		//again 3 values
    		if (j - 1 >= 0 && stencil[2][0] && flagv(idx)) retVal(idx, idx + int(N) - 1) = stencil[2][0];
    		if (stencil[2][1] && flagv(idx)) retVal(idx, idx + int(N)) = stencil[2][1];
    		if (j + 1 < int(N) && stencil[2][2] && flagv(idx)) retVal(idx, idx + int(N) + 1) = stencil[2][2];
    	    }
    	}
    }
    return retVal;
}
// vim: sw=4:tw=78:ft=cpp:et:cindent
