#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

void LangrangeSolve(int rows, int cols, float **temp, int **flagmat, float threshold, float relax_param){
	
	float diff = 1000000;
	int iter = 0;
	float new_val;

	while /*((iter <30) && */ (diff > threshold){
		diff = 0.;
		for (int i=1; i<rows-1; i++){
			for (int j=1; j<cols-1; j++){
		
				if (flagmat[i][j] == 0){ 
					new_val = temp[i][j] + relax_param*(0.25*(temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) - temp[i][j]);
					if (i==1) temp[i-1][j] = temp[i][j];
					if (i==(rows-2)) temp[i+1][j] = temp[i][j];
					if (j==1) temp[i][j-1] = temp[i][j];
					if (j==(cols-2)) temp[i][j+1] = temp[i][j];
				
					//no need to change corners of grid now -> they are not used
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
	//change corners to avoid weird graphical effects on plots
	temp[0][0] = temp[0][1];
	temp[0][cols-1] = temp[1][cols-1];
	temp[rows-1][0] = temp[rows-1][1];
	temp[rows-1][cols-1] = temp[rows-2][cols-1];

	cout << "Iterations: " << iter << endl; 
}

