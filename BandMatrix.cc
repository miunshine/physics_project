#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

class BandMatrix;

class proxy {
    private:
	BandMatrix& m_mat;
	int m_diff, m_min;
    public:
	proxy(BandMatrix& mat, int diff, int min) : m_mat(mat), m_diff(diff), m_min(min) {}
	float& operator=(float el);
	operator float() const;
};

class BandMatrix{
    public:
        //definitions inside class
	typedef std::map<int, std::vector<float> > SparseBands;
	typedef SparseBands::iterator iterator;
	typedef SparseBands::const_iterator const_iterator;
	typedef SparseBands::value_type band_value_type;
	typedef SparseBands::reference band_reference;
	typedef SparseBands::const_reference band_const_reference;
    private:
	unsigned m_N;           //dimension of bandmatrix
	SparseBands m_elements; //BAND matrix definition
	friend class proxy;     //define another class to be used inside this one
    public:
        // : m_N(N) is a way to initialize
	BandMatrix(unsigned N) : m_N(N) { }        //initialised constructor

        //Cannot use standard element access (this->mat[][]) because of band
        //form. Use a proxy class that access the band matrix and returns
        //element at correct position. Since the band matrix does not have
        //zeros stored, the proxy needs to return a zero value if the
        //requested element is not in the matrix. 
	proxy operator()(int i, int j)
	{
	    return proxy(*this, i - j, std::min(i, j)); //initialise proxy
	}

	float operator()(int i, int j) const //const -> no change of members
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
	
        unsigned size() const { return m_N; }

        //map::end, map::begin definitions in case band.begin() is called
	const_iterator begin() const { return m_elements.begin(); }
	const_iterator end() const { return m_elements.end(); }
	iterator begin() { return m_elements.begin(); }
	iterator end() { return m_elements.end(); }
	
};

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
//BUT variable a might be a float (or type T) which means that an operator
//float() is needed to return element in its specific value that is a float.
//So t
float& proxy::operator=(float el) {
	auto& band = m_mat.m_elements[m_diff]; //key to access map
	if (band.empty())
		//m_N - abs(m_diff) is the size of off diagonal vector
                band.resize(m_mat.m_N - std::abs(m_diff), 0.f);
	return (band[m_min] = el); //returns requested element
}
proxy::operator float() const {
	//similar to operator() in bandmatrix but returns a float.
        const auto& it = m_mat.m_elements.find(m_diff);
	if (m_mat.m_elements.end() == it)
		return 0.f;
	const auto& band = it->second;
	return band[m_min];
}

std::vector<float> operator*(const BandMatrix& M, const std::vector<float>& v)
{
    assert(M.size() == v.size());               //check sizes
    std::vector<float> retVal(v.size(), 0.f);
    //for the number of "pairs" in M - pair is key and vector
    for (const auto& band: M) {
	int diff = band.first;   //band.first stores the key- ie the diff i,j
	int min = 0;             //default min ie the diagonal
	//for elements in band.second ie vector
        for (const auto m: band.second) {
            //if statement INSIDE int definition. 
	    int i = (diff >= 0) ? min + diff :        min;
	    int j = (diff >= 0) ?        min : min - diff; 
	    retVal[i] += m * v[j]; // retVal_i += m_ij v_j
	    ++min;
	}
    }
    return retVal;
}

//Band matrix creator from 2D stencil
BandMatrix createFromStencil(unsigned N, float stencil[3][3])
{
    //N is the grid SIDE size -> N*N is the diagonal size, N^4 is the matrix size 
    BandMatrix retVal(N * N);
    for (int i = 0; i < int(N); ++i) {
	for (int j = 0; j < int(N); ++j) {
	    unsigned idx = i * int(N) + j;
	    if (i - 1 >= 0) {
		if (j - 1 >= 0 && stencil[0][0]) retVal(idx, idx - int(N) - 1) = stencil[0][0];
		if (stencil[0][1]) retVal(idx, idx - int(N)) = stencil[0][1];
		if (j + 1 < int(N) && stencil[0][2]) retVal(idx, idx - int(N) + 1) = stencil[0][2];
	    }
	    if (j - 1 >= 0 && stencil[1][0]) retVal(idx, idx - 1) = stencil[1][0];
	    if (stencil[1][1]) retVal(idx, idx) = stencil[1][1];
	    if (j + 1 < int(N) && stencil[1][2]) retVal(idx, idx + 1) = stencil[1][2];
	    if (i + 1 < int(N)) {
		if (j - 1 >= 0 && stencil[2][0]) retVal(idx, idx + int(N) - 1) = stencil[2][0];
		if (stencil[2][1]) retVal(idx, idx + int(N)) = stencil[2][1];
		if (j + 1 < int(N) && stencil[2][2]) retVal(idx, idx + int(N) + 1) = stencil[2][2];
	    }
	}
    }
    return retVal;
}

int main()
{
    float stencil[3][3] = {
	{  0.f,  1.f,  0.f },
	{  1.f, -4.f,  1.f },
	{  0.f,  1.f,  0.f },
    };
    BandMatrix M = createFromStencil(6, stencil);
    for (unsigned i = 0; i < M.size(); ++i) {
	for (unsigned j = 0; j < M.size(); ++j)
	    std::printf(" %g", float(M(i, j)));
	std::printf("\n");
    }
    for (const auto& band: M) {
	int diff = band.first;
	std::printf("{diff = %d: [", diff);
	for (auto el: band.second) {
	    std::printf(" %g", el);
	}
	std::printf("] }\n");
    }
    return 0;
}
// vim: sw=4:tw=78:ft=cpp:et:cindent
