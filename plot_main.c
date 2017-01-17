#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

void copyMatrices(double **matA, double **matB, int r, int c);
double func(double x, double y, double R);

int main() 
{
	int iter, rows, cols;
	double value1, value2, threshold, diff;
	ofstream output("out.dat");
	value1 = 0; value2 = 50;
	iter = 0; 

	cout << "Give matrix dimensions (row , column)" << endl;
	cin >> rows >> cols;
	cout << "Give maximum threshold: " << endl;
	cin >> threshold;

	double **grid = new double*[rows];
	for (int i=0; i<rows; i++)
		grid[i] = new double[cols];
	double **temp = new double*[rows];
	for (int i=0; i<rows; i++)
		temp[i] = new double[cols];
	int **flagmat = new int*[rows];
	for (int i=0; i<rows; i++)
		flagmat[i] = new int[cols];
	
	

	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			flagmat[i][j] = 0;
			temp[i][j] = 0;
			grid[i][j] = 0;
/*			if ((func(i,j, 2) < 0.5) && (func(i,j,2) >-0.5 )) {
				grid[i][j] = value1;
				temp[i][j] = value1;
				flagmat[i][j] = 1;  
			}
			if ((func(i,j,8) < 0.5) && (func(i,j,8) > -0.5)){
				grid[i][j] = value2;
				temp[i][j] = value2;
				flagmat[i][j] = 1;
			}
*/			if (i== (rows/2) && j == (cols/2)){
				flagmat[i][j] =1;
				 grid[i][j] = 0;
				temp[i][j] = 0;
			}
			if (i==(rows-1) || (i==0) || (j==(cols-1)) || (j==0)) {
				grid[i][j]=50;
				flagmat[i][j] = 1;
				temp[i][j] = 50;
			}
		}
	}
	cout << " Grids created " << endl;
	diff = 1000000;
	while /*((iter <30) && */ (diff > threshold){
		diff = 0.;
		for (int i=1; i<rows-1; i++){
			for (int j=1; j<cols-1; j++){
				if (flagmat[i][j] == 0){ 
					grid[i][j] = 0.25*(temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]);
			
				}
				diff = diff + abs(temp[i][j] - grid[i][j]);
				temp[i][j] = grid[i][j]; //copying here makes this different method
			}
		}
		iter = iter + 1;
	}
	 
	cout << endl << "Potential Matrix generated " << endl;
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++)
			output << j << " " << i << " " << temp[i][j] << endl;
	}
	cout << "Iterations: " << iter << endl;
	
	return 0;
}
		
void copyMatrices(double **matA, double **matB, int r, int c){
	for (int i=0; i<r; i++)
		for (int j=0; j<c; j++)
			matA[i][j] = matB[i][j];
}

double func(double x, double y, double R){
	return (x*x + y*y - R*R);
}


