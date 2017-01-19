#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

void LangrangeSolve(int rows, int cols, double **grid, double **temp, int **flagmat, double threshold, double relax_param){
	
	double	diff = 1000000;
	int iter = 0;
	
	while /*((iter <30) && */ (diff > threshold){
		diff = 0.;
		for (int i=1; i<rows-1; i++){
			for (int j=1; j<cols-1; j++){
				if (flagmat[i][j] == 0){ 
					grid[i][j] = temp[i][j] + relax_param*(0.25*(temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) - temp[i][j]);
			
				}
				diff = diff + abs(temp[i][j] - grid[i][j]);
				temp[i][j] = grid[i][j]; //copying here makes this different method
				if (i==1) {
					temp[0][j] = temp[i][j];
					grid[0][j] = grid[i][j];
				}
				if (i==(rows-2)) {
					temp[rows-1][j] = temp[i][j];
					grid[rows-1][j] = grid[i][j];
				}
				if (j==1) {
					temp[i][0] = temp[i][j];
					grid[1][0] = grid[i][j];
				}
				if (j==(cols-2)) {
					temp[i][cols-1] = temp[i][j];
					grid[i][cols-1] = grid[i][j];
				}
			}	
		}
		iter = iter + 1;
		grid[0][0] = grid[1][1];
		temp[0][0] = temp[1][1];
		grid[rows-1][cols-1] = grid[rows-2][cols-2];
		temp[rows-1][cols-1] = temp[rows-2][cols-2];
		grid[0][cols-1] = grid[1][cols-2];
		temp[0][cols-1] = temp[1][cols-2];
		grid[rows-1][0] = grid[rows-2][1];
		temp[rows-1][0] = temp[rows-2][1]; 
	}
	
	cout << "Iterations: " << iter << endl; 
}

