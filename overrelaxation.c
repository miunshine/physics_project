#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

void copyMatrices(double **matA, double **matB, int r, int c);
double func(double x, double y, double R);
double round(double d);

int main() 
{
	int iter, rows, cols, y_index;
	double value1, value2, threshold, diff, relax_param,tempy;
	ofstream output("relax_out.dat");
	value1 = 0; value2 = 50;
	iter = 0; 
	
	cout << "Give matrix dimensions (row , column)" << endl;
	cin >> rows >> cols;
	cout << "Give maximum threshold: " << endl;
	cin >> threshold;
	cout << "Give overrelaxation parameter: " << endl;
	cin >> relax_param;	


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
		}
	}
	
	for (int j=0; j<cols;j++){
		if ((10*10 - (j-5)*(j-5)) < 0) break;
		tempy = sqrt(10*10 - (j-5)*(j-5)) + 5;
		y_index = (int) round(tempy);
		temp[y_index][j] = 50;
		grid[y_index][j] = 50;
		flagmat[y_index][j] = 1;
	}
	for (int j=0; j<cols; j++){
		if (0*0 - j*j <0) break;
		tempy = sqrt(4*4 - j*j);
		y_index = (int) round(tempy);
		temp[y_index][j] = 0;
		grid[y_index][j] = 0;
		flagmat[y_index][j] = 1;
	}
	cout << "Applied potentials" << endl;
	
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
			if (i== (rows/2) && j == (cols/2)){
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
*/
	cout << " Grids created " << endl;
	diff = 1000000;
	while /*((iter <30) && */ (diff > threshold){
		diff = 0.;
		for (int i=1; i<rows-1; i++){
			for (int j=1; j<cols-1; j++){
				if (flagmat[i][j] == 0){ 
					grid[i][j] = temp[i][j] + relax_param*(0.25*(temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) - temp[i][j]);
			
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
	
	for (int i=0; i<rows; i++){
		delete[] grid[i];
		delete[] flagmat[i];
		delete[] temp[i];
	}
	delete[] grid;
	delete[] flagmat;
	delete[] temp;



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

double round(double d){
	return floor(d+0.5);
}
