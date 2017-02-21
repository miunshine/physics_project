#include <iostream>
#include "BandMatrix.h"
#include "vector.h"

int main()
{
    float stencil[3][3] = {
	{  0.f,  1.f,  0.f },
	{  1.f, -4.f,  1.f },
	{  0.f,  1.f,  0.f },
    };
    int n = 3;
    BandMatrix M = createFromStencil(n, stencil);
    M(0,0) = 1.f;
    for (int i=1; i< M.size(); i++)
        M(0,i) = 0.f;
    
    LAVector<float> v1(n*n, 1.1);
    LAVector<float> v2(n*n, 0.0), v3(n*n, 0.0);
    v2  = M * v1; 
    v3  = M ->* v1;
    float test = v3 * v3;
    printf("Test: %f \n" , test);
    for (int i=0; i<v2.get_size(); ++i) {
        printf(" %g", v2(i));
    }
    printf("\n");

    for (int i=0; i<v3.get_size(); ++i) {
        printf(" %g", v3(i));
    }
    printf("\n");



/*    for (unsigned i = 0; i < 6; ++i) {
        for (unsigned j = 0; j< 6; ++j) {
            printf(" %g" , v2[i+j]);
        }
        printf("\n");
    }
    v1.clear();
    v2.clear();
    v3.clear();
*/
    for (int i = 0; i < M.size(); ++i) {
	    for (int j = 0; j < M.size(); ++j)
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


