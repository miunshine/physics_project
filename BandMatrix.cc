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
	typedef std::map<int, std::vector<float> > SparseBands;
	typedef SparseBands::iterator iterator;
	typedef SparseBands::const_iterator const_iterator;
	typedef SparseBands::value_type band_value_type;
	typedef SparseBands::reference band_reference;
	typedef SparseBands::const_reference band_const_reference;
    private:
	unsigned m_N;
	SparseBands m_elements;
	friend class proxy;
    public:
	BandMatrix(unsigned N) : m_N(N) { }
	proxy operator()(int i, int j)
	{
	    return proxy(*this, i - j, std::min(i, j));
	}
	float operator()(int i, int j) const
	{
	    int diff = i - j;
	    const auto& it = m_elements.find(diff);
	    if (m_elements.end() == it)
		return 0.f;
	    const auto& band = it->second;
	    return band[std::min(i, j)];
	}
	unsigned size() const { return m_N; }
	const_iterator begin() const { return m_elements.begin(); }
	const_iterator end() const { return m_elements.end(); }
	iterator begin() { return m_elements.begin(); }
	iterator end() { return m_elements.end(); }
	
};

float& proxy::operator=(float el) {
	auto& band = m_mat.m_elements[m_diff];
	if (band.empty())
		band.resize(m_mat.m_N - std::abs(m_diff), 0.f);
	return (band[m_min] = el);
}
proxy::operator float() const {
	const auto& it = m_mat.m_elements.find(m_diff);
	if (m_mat.m_elements.end() == it)
		return 0.f;
	const auto& band = it->second;
	return band[m_min];
}

std::vector<float> operator*(const BandMatrix& M, const std::vector<float>& v)
{
    assert(M.size() == v.size());
    std::vector<float> retVal(v.size(), 0.f);
    for (const auto& band: M) {
	int diff = band.first;
	int min = 0;
	for (const auto m: band.second) {
	    int i = (diff >= 0) ? min + diff :        min;
	    int j = (diff >= 0) ?        min : min - diff; 
	    retVal[i] += m * v[j]; // retVal_i += m_ij v_j
	    ++min;
	}
    }
    return retVal;
}

BandMatrix createFromStencil(unsigned N, float stencil[3][3])
{
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
    BandMatrix M = createFromStencil(10000, stencil);
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
