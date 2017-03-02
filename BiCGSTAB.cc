#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cassert>
#include <cmath>

#include "vector.h"
#include "BandMatrix.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{	
    double permittivity =  8.854188 * pow(10,-12);
	Mat image;
	double positive_val, negative_val, charge;
    double stencil[3][3] = {
	{  0.,  -1.,  0. },
	{  -1.,  4., -1. },
	{  0.,  -1.,  0. },
    };
    ofstream fout("output.dat");
    image = imread(argv[1], 1);
    // -- Input --
	cout << "Insert value of positive voltage: " << endl;
	cin >> positive_val;
	cout << "Insert absolute value of negative voltage: " << endl;
	cin >> negative_val;
	cout << "Give density charge value: " << endl;
    cin >> charge;

	if ( !image.data )
	{
        	printf("No image data \n");
        	return -1;
	}
	
    //   namedWindow("Display Image", WINDOW_AUTOSIZE );
	//    imshow("Display Image", image);
	
	//    waitKey(0);
	
    assert(image.cols == image.rows);
    int n = image.cols;
	
    // -- Image part --
    printf("Reading Image\n");
	unsigned char *input = (unsigned char*)(image.data);
	int red,green,blue;
	Vec3b intensity;
    
    LAVector<double> b(n*n, 0.0);
    LAVector<int>    flag(n*n, 1.0);
    LAVector<int>    periodic_edge(n*n, 0.0);

	for (int i = 0; i<n;i++){
		for (int j = 0; j < n;j++){
			intensity = image.at<Vec3b>(j,i);
			blue  = intensity.val[0];
        	green = intensity.val[1] ;
        	red   = intensity.val[2] ;

			if ((blue>150) && (red<150) && (green<150))
			{
                b(i*n+j) = -negative_val;
                flag(i*n+j) = 0.;
            }
			else if ((blue<150) && (red>150) && (green<150))
			{
                b(i*n+j) = positive_val;
			    flag(i*n+j) = 0.;
            }
			else if ((blue<150) && (red<150) && (green<150))
			{
                b(i*n+j) = 0;
			    flag(i*n+j) = 0.;
            }
			
            else if ((blue<150) && (red<150) && (green>150))
			{
				b(i*n+j) = charge / permittivity;
            }
            
            else if (blue>150 && red>150 && green>150)
            {
                if (j == 0) {
                    periodic_edge(i*n+j)= 1;
                }
                if (j == (n-1)) {
                    periodic_edge(i*n+j) = 1;
                }
                if (i == 0) {
                    periodic_edge(i*n+j) = 1;
                }
                if (i == (n-1)){
                    periodic_edge(i*n+j) = 1;
                }
            }

        }
	}	
    
    // BICONJUGATE GRADIENT STABILISED -- 
    
    printf("Starting Solver\n");
    BandMatrix A = createFromStencil(n, stencil, flag,periodic_edge);
    //std::vector<double> Mi = A.diag();
    //LAVector<double> M(n*n, 0.0);
    //M.inverse(Mi);

    LAVector<double> x = b;
    LAVector<double> r = b - A * x;
    LAVector<double> r_o = r;
    LAVector<double> h(n*n, 0.0);
    LAVector<double> p(n*n, 0.0);
    LAVector<double> v(n*n, 0.0);
    LAVector<double> s(n*n, 0.0);
    LAVector<double> t(n*n, 0.0);

    double rho_old = 1; double omega_old = 1; double alpha = 1;
    double rho; double omega=1; double beta; double res;
    double epsilon = 0.0001; 
    double res_o = r * r;
    int count = 0;
    while (count == 0 || res > (epsilon*epsilon*res_o))  {
        ++count;
        rho = r_o * r;
        beta = (rho / rho_old) * ( alpha / omega);
        p = r + ((p - (v * omega)) * beta);
        v = A * p;
        alpha = rho / ( r_o * v );
        h = x +  p * alpha;
        s = r - v * alpha;
        if ((s * s) < (epsilon*epsilon*res_o)) {
            x = h;
            break;
        }
        t = A*s;
        omega = (t * s) / (t * t);
        x = h + s * omega;
        r = s - t * omega;
        res = r * r;
        cout << "Res: " << res << endl;
        rho_old = rho;
    }
    
    printf("count is %d \n", count);
    printf("Saving Results\n");

    cout << sizeof(A) << endl;
    for (int i = 0; i< n; i++) {
        for (int j = 0; j <n ; j++)
            fout << i << " " << j << " " << x(i*n+(n-1-j)) << endl;
    }

	return 0;
}
