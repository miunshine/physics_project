#include <cmath>
#include <iostream>
#include <fstream>

#include "vector.h"
#include "BandMatrix.h"

using namespace std;

int main() {
    double stencil[3][3] = {
	{  0.,  -1.,  0. },
	{  -1.,  4., -1. },
	{  0.,  -1.,  0. },
    };
    
    ofstream fout("output.dat");
    int n = 100;  //side of grid (or diagonal of grid matrix)
    BandMatrix A = createFromStencil(n, stencil);

    LAVector<double> b(n*n, 2.);
    LAVector<double> x(n*n, 0.0);
    LAVector<double> r = b - A * x;
    LAVector<double > Ap(n*n,0.0);
    LAVector<double> p = r;
    double rsold = r * r; double epsilon = 0.001; double rs_origin = rsold;
    double alpha; double beta; double rsnew; 
    int count = 0;

    while (count == 0 || rsnew > (epsilon*epsilon*rs_origin))  {
        ++count;
        Ap = A * p;
        alpha = rsold / (p * Ap);
        x += p * alpha;
        if (count % 50) 
            r = b - A * x;
        else
            r -= Ap * alpha;
        rsnew = r * r;
        //if (sqrt(rsnew) < epsilon) break;
        p = r + p * (rsnew / rsold);
        rsold = rsnew;
    }
    
    printf("count is %d \n", count);
    for (int i= 0; i<x.get_size(); i++)
        cout << x(i) << " ";
    cout << endl;
  
    for (int i = 0; i< n; i++) {
        for (int j = 0; j< n; j++)
            fout << i << " " << j << " " << x(i+j) << endl;
    }

    return 0;
}
