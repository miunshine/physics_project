#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

void LangrangeSolve(int rows, int cols, double **temp, int **flagmat, double threshold, double relax_param){
	
	double	diff = 1000000;
	int iter = 0;
	double new_val;

	while /*((iter <30) && */ (diff > threshold){
		diff = 0.;
		for (int i=1; i<rows-1; i++){
			for (int j=1; j<cols-1; j++){
				if (flagmat[i][j] == 0){ 
					new_val = temp[i][j] + relax_param*(0.25*(temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) - temp[i][j]);
			
				}
				else{
					new_val = temp[i][j];
				}
				diff = diff + abs(temp[i][j] - new_val);
				temp[i][j] = new_val; //copying here makes this different method
			}	
		}
		iter = iter+1;
	}
	
	cout << "Iterations: " << iter << endl; 
}

