#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "overrelaxation.h"

using namespace std;

void copyMatrices(double **matA, double **matB, int r, int c);
double func(double x, double y, double R);
double round(double d);

int main()
{
        int rows, cols, y_index;
        double value1, value2, threshold, diff, relax_param,tempy;
        ofstream output("relax_out.dat");
        value1 = 0; value2 = 50;

        cout << "Give matrix dimensions (row , column)" << endl;
        cin >> rows >> cols;
        cout << "Give maximum threshold: " << endl;
        cin >> threshold;
        cout << "Give overrelaxation parameter: " << endl;
        cin >> relax_param;


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
                }
        }

        for (int j=0; j<cols;j++){
                if ((10 *10 - (j-5)*(j-5)) < 0) break;
                tempy = sqrt(10*10 - (j-5)*(j-5)) + 5;
                y_index = (int) round(tempy);
                temp[y_index][j] = 50;
                flagmat[y_index][j] = 1;
        }
        for (int j=0; j<cols; j++){
                if (0*0 - j*j <0) break;
                tempy = sqrt(4*4 - j*j);
                y_index = (int) round(tempy);
                temp[y_index][j] = 0;
                flagmat[y_index][j] = 1;
        }

        LangrangeSolve(rows,cols,temp,flagmat,threshold,relax_param);

        for (int i=0; i<rows; i++){
                for (int j=0; j<cols; j++)
                        output << j << " " << i << " " << temp[i][j] << endl;
        }

        for (int i=0; i<rows; i++){
                delete[] flagmat[i];
                delete[] temp[i];
        }
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

