#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;


/*
take square matrix where side length is D , with quarter sphere in left up corner - zeros
divide D in dx parts , skip the sphere, start evaluating from where j > R, next row start earlier to go around circle
how about the boundary p at the end? relaxation method
maybe start from y = inf, where v=px/D, though infinity may not be the best place to start

input D - distance from middle to plate, R - spheres radius , N number of partitions from 0 to D
  
*/


double solver(int N, float D, float R){
float p = 999; //plate potential
float step = float(D)/float(N) ;
int xmid =  N/2+2; //put GND in middle
int ymid = N/2+1;
float xval, yval, xmidval, ymidval; //for blob

xmidval = xmid*step;
ymidval = ymid*step;


//OUTPUT FILE
ofstream outfile1 ;
outfile1.open("result.dat", std::ios::app); // open to append text




//MATRIX OF ZEROS between two plates
float M[N+4][N] ; // two elements in x dir on each side for p-+ plates



for (int i=2 ;i<N+2; i++ ){ //middle elements, first and last column are plate potentials
	for (int j=1 ;j<N+4; j++ ){
			M[0][j] = p;
			M[1][j] = p;
			M[i][j] = 0;
			M[N+2][j] = -p;
			M[N+3][j] = -p;
	}
}



for (int x=2 ;x<N+2; x++ ){ //index in x , from second column
 	for (int y=1 ;y<N+1; y++ ){  // index y

//skip middle blob
xval = x*step; //+- convert from index
yval = y*step;
if ( pow(xval-xmidval,2) < pow(R,2) - pow(yval-ymidval,2) ){
//cout << "skipping elm " << x << ' ' << y << endl;
outfile1 << x << ' ' << y << ' '<<  0 <<  endl; 
continue; 
}


		M[x][y] = 0.25*(M[x-1][y]+M[x-1][y-1]+M[x-1][y+1]+M[x-2][y]); //calculate new elm
		
if(isinf(M[x][y]) || isnan(M[x][y])){
	outfile1 << x << ' ' << y << ' '<<  0 <<  endl;
	continue;
	}
		outfile1 << x << ' ' << y << ' '<<  M[x][y] <<  endl; 
		//cout << "M" << x<< '.' << y << ' ' << M[x][y] << endl;	
	}

	M[x][0] = M[x][1]; //we need values aboveand below space, fill additional rows
	M[x][N+1] = M[x][N];
}


return 0;
}


int main(){
	int N;
	float D,R;

	N = 50;
	R = 10;
	D = 50;
	//cout << "enter number of partitions, size and radius" << endl;
	//cin >> N;
	//cin >> D;
	//cin >> R;
	solver(N,D,R);


	return 0; 
}

