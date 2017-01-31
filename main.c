#include "matrix.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include "RowIndex.h"
#include "vector.h"
#include <time.h>
using namespace std;

int main(int argc, char **argv) {
	int n;
	clock_t t1,t2,t3,t4;
	cout << "give n" << endl;
	cin >> n;
	int n2 = n*n; long k; long diag_k;
	ofstream fout("sparse.dat");
	LAMatrix<int> index(n, n, 0);
	LAMatrix<double> eye(n,n,0.0);
	LAMatrix<double> stencil(n, n, 0.0);
	LAMatrix<double> zeros(n,n, 0.0);
	LAMatrix<double> laplacian((4+(n-2)*3)*n+2*(n-1)*(n) + 1, 2, 0.0); //function of size of RowIndexed for Laplacian first part is stencil second is eye
	cout << "Elements: " <<(4+(n-2)*3)*n+2*(n-1)*(n) + 1 << endl;
	LAVector<double> x(n2,10.0);
	LAVector<double> r(n2,0.0); 
	t1 = clock();
	for (int i=0; i<index.get_rows(); i++){
		for (int j=0; j<index.get_cols(); j++){
			if (i==j) { 
				index(i,j) = 2;
				eye(i,j) = -1;
				stencil(i,j) = 4.;
			}
			if (fabs(i-j)==1) {
				index(i,j) = 1; 
				stencil(i,j) = -1.;
			}
		}
	}
	t2 = clock();
	cout << "Matrix Blocks: " << (float) t2 - (float) t1 << endl;
	RowIndex(n,laplacian,index,stencil,eye,zeros);
	t3 = clock();
	cout << "Row Index transformation: " << (float) t3 - (float) t2 << endl;
	r = laplacian ->* x;
	t4 = clock();
	cout << "Matrix Product: " << (float) t4 - (float) t3 << endl;
		
//	for (int i = 0; i< r.get_size() ; i++ )
//		cout << r(i) << " ";
//	cout << endl;
	fout << laplacian.get_rows() << endl;
	for (int i=0; i<laplacian.get_rows(); i++)
		fout << laplacian(i,0) << " ";
	fout << endl;
	for (int i=0; i< laplacian.get_rows(); i++)
		fout << laplacian(i,1) << " ";
	fout << endl;
  return 0;
}
