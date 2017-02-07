#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cassert>

#include "vector.h"
#include "BandMatrix.h"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Mat image;
	double positive_val, negative_val, charge;
    double stencil[3][3] = {
	{  0.,  -1.,  0. },
	{  -1.,  4., -1. },
	{  0.,  -1.,  0. },
    };
    ofstream fout("output.dat");
    image = imread(argv[1], 1);

	cout << "Insert value of positive voltage: " << endl;
	cin >> positive_val;
	cout << "Insert absolute value of negative voltage: " << endl;
	cin >> negative_val;
	negative_val = abs(negative_val);
	cout << "Give charge value: " << endl;
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
	    
	unsigned char *input = (unsigned char*)(image.data);
	int red,green,blue;
	Vec3b intensity;
    
    LAVector<double> b(n*n, 0.0);
    LAVector<int>    flag(n*n, 1.0);

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
				b(i*n+j) = charge;
            }

		}
	}	
    
    BandMatrix A = createFromStencil(n, stencil, flag);
    std::vector<double> Mi = A.diag();
    LAVector<double> M(n*n, 0.0);
    M.inverse(Mi);
    
    LAVector<double> x(n*n, 0.0);
    LAVector<double> r = b - A * x;
    LAVector<double> s(n*n, 0.0);
    LAVector<double > Ap(n*n,0.0);
    LAVector<double> p = r ->* M;

    double rsold = r * p; double epsilon = 0.001; double rs_origin = rsold;
    double alpha; double beta; double rsnew; 
    int count = 0;
    
    while (count == 0 || rsnew > (epsilon*epsilon*rs_origin))  {
        ++count;
        Ap = A * p;
        alpha = rsold / (p * Ap);
        x += p * alpha;
        if (count % 50) 
            r = b - A * x;
        else
            r -= Ap * alpha;
        s = r ->* M;  //elemntwise multiplication of vectors(defined in vector class)
        rsnew = r * s;
        //if (sqrt(rsnew) < epsilon) break;
        p = s + p * (rsnew / rsold);
        rsold = rsnew;
    }
    
    printf("count is %d \n", count);
//    for (int i= 0; i<x.get_size(); i++)
//       fout << x(i) << " ";
//    fout << endl;
  
    for (int i = 0; i< n; i++) {
        for (int j = 0; j< n; j++)
            fout << i << " " << j << " " << x(i*n+j) << endl;
    }

	return 0;
}
