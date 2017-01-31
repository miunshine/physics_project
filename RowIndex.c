#include "matrix.h"
#include <cmath>
using namespace std;

void RowIndex(int n, LAMatrix<double>& laplacian, LAMatrix<int>& index, LAMatrix<double>& stencil, LAMatrix<double>& eye, LAMatrix<double>& zeros) {
	int n2 = n*n;
	LAMatrix<double> *block;
	laplacian(0,1) = n2 + 1; 
	long k = n2; 
	long diag_k = 0;
	for (int i_index = 0; i_index < index.get_rows(); i_index++) {
		for (int row =0; row < stencil.get_rows(); row++) {

			for (int j_index = 0; j_index < index.get_cols() ; j_index++) {
				if (index(i_index,j_index) == 2) block = &stencil;
				if (index(i_index,j_index) == 1) block = &eye;
				//if (index(i_index,j_index) ==-1) block = &eyeDOWN;
				if (index(i_index,j_index) == 0) block = &zeros;
				for (int col=0; col< block->get_cols(); col++) {
				//	cout << (i_index)*n + row << ","  << (j_index)*n + col << "  "; //INDEXES
				//	laplacian((i_index)*n + row, (j_index)*n + col);
					if (((i_index*n) + row) == ((j_index)*n+col)) {
						laplacian(diag_k,0) = block->get_element(row,col);
						diag_k = diag_k + 1;
						continue;
					}
				
					if ((fabs(block->get_element(row,col)) > 0) && (((i_index*n) + row) != ((j_index)*n + col))){
						k = k+1;
						//if (k >= laplacian.get_rows()) return -1;
						laplacian(k,0) = block->get_element(row,col);
						laplacian(k,1) = (j_index)*n + col;
						//cout << laplacian(k,0) << " ";
					}
				}
			}
			laplacian((i_index*n)+row+1,1) = k+1;
		}
	}
}
