#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;


int main() 
{				//inner matrix dims input	radius
	int iter, rows, cols,  rows_inp, cols_inp ,		 r;
	double threshold, diff, relax_param;
	ofstream output("relax_out.dat");
	iter = 0; 
	float new_val;
	float p = 9999 , p2 = 0; //plate potential , blob potential 
	
	cout << "Give matrix dimensions (row , column)" << endl;
	cin >> rows_inp >> cols_inp;
	rows = rows_inp +2; //additional rows and cols for boundaries
	cols = cols_inp +2;
	cout << "Give radius: " << endl;
	cin >> r;
	cout << "Give maximum threshold: " << endl;
	cin >> threshold;
	cout << "Give overrelaxation parameter interval(1,2): " << endl;
	cin >> relax_param;	

	
	
	double **temp = new double*[rows]; //for actual values
	for (int i=0; i<rows; i++)
		temp[i] = new double[cols];
	int **flagmat = new int*[rows]; //for places to solve
	for (int i=0; i<rows; i++)
		flagmat[i] = new int[cols];
		
	
int jmid = cols/2; //middle markers
int imid = rows/2;


//INITIAL
	for (int j=1; j<cols-1; j++){  //	j-col index, 	i-row index
		for (int i=0; i<rows; i++){
		//SIDES
			flagmat[i][0] = 1;
			flagmat[i][cols-1] = 1; //marker values
			temp[i][0] = p;
			temp[i][cols-1] = -p;
		//MID
			if ( pow(j-jmid,2) < pow(r,2) - pow(i-imid,2) ){
				flagmat[i][j] = 1;
				temp[i][j] = p2;			
			}

		//NONE
			else { 			
				flagmat[i][j] = 0;
				temp[i][j] = 0;
			}
		}
	}

cout << "Applied potentials" << endl;

/*
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			cout << j << " " << i << " temp " << temp[i][j] << endl;
			cout << j << " " << i << " flag " << flagmat[i][j] << endl;
		}
	}
*/
	
//SOLVER
	diff = 1000000;
	while /*((iter <30) && */ (diff > threshold){
		diff = 0.;

		for (int j=1; j<cols-1; j++){ //col
			for (int i=1; i<rows-1; i++){ //row

				if (flagmat[i][j] == 0){ 
					new_val = temp[i][j] + relax_param*(0.25*(temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1]) - temp[i][j]);
			
				}
				else{
					new_val = temp[i][j];
				}

				diff = diff + abs(temp[i][j] - new_val);
				temp[i][j] = new_val; 				
			}

			temp[0][j] = temp[3][j]; //boundaries most efficient
			temp[rows-1][j] = temp[rows-4][j];
			
		}
		iter = iter + 1;
	}

//OUTPUT	 
	cout << endl << "Potential Matrix generated " << endl;
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			output << j << " " << i << " " << temp[i][j] << endl;
			//cout << j << " " << i << " temp " << temp[i][j] << endl;
			//cout << j << " " << i << " flag " << flagmat[i][j] << endl;
			//output << j << " " << i << " " << flagmat[i][j] << endl;
		}
	}
	cout << "Iterations: " << iter << endl;
	

	for (int i=0; i<rows; i++){
		delete[] flagmat[i];
		delete[] temp[i];
	}
	delete[] flagmat;
	delete[] temp;



	return 0;
}
		

