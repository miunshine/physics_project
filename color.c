#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <vector>
#include "overrelaxation.h"
#include <string>

using namespace cv;
using namespace std;

int main()
{
	string image_name;
	float positive_val, negative_val;
	Mat image;
	ofstream output("relax_out.dat");
	
	cout << "Insert name of picture" << endl;
	cin >> image_name;
	image = imread(image_name, 1);
    	
	cout << "Insert value of positive voltage: " << endl;
	cin >> positive_val;
	cout << "Insert absolute value of negative voltage: " << endl;
	cin >> negative_val;
	negative_val = abs(negative_val);
	
	if ( !image.data )
	{
        	printf("No image data \n");
        	return -1;
	}
	//   namedWindow("Display Image", WINDOW_AUTOSIZE );
	//    imshow("Display Image", image);
	
	//    waitKey(0);
	int rows = image.rows;
	int cols = image.cols;
	
	cout << " rows " << image.rows;
	cout << "cols " << image.cols;
	
	float **array = new float*[rows];
	for (int i=0; i<image.rows; i++) {
		array[i] = new float[cols];
	}
	int **flagmat = new int*[rows];
	for (int i=0; i<image.rows; i++) 
		flagmat[i] = new int[cols];
	
	unsigned char *input = (unsigned char*)(image.data);
	int r,g,b;
	Vec3b intensity;
	
	for (int i = 0; i<rows;i++){
		for (int j = 0; j < cols;j++){
			intensity = image.at<Vec3b>(j,i);
			b = intensity.val[0];
        		g = intensity.val[1] ;
        		r = intensity.val[2] ;
			if ((b>150) && (r<150) && (g<150))
			{
				array[i][j] = -negative_val;
				flagmat[i][j] = 1;
			}
			else if ((b<150) && (r>150) && (g<150))
			{
				array[i][j] = positive_val;
				flagmat[i][j] = 1;
			}
			else if ((b<150) && (r<150) && (g<150))
			{
				array[i][j] = 0;
				flagmat[i][j] = 1;
			}
			else 
			{
				array[i][j] = 0;
				flagmat[i][j] = 0;
			}
		}
	}	
	for (int j=0; j<cols; j++){
		for (int i=0; i<rows; i++){
			cout << flagmat[i][j] << " ";
		}
		cout << endl;
	}
	LangrangeSolve(image.rows, image.cols, array, flagmat, 0.1, 1.5);
	
	for (int j=0; j<cols; j++)
	{
		for (int i=0; i<rows; i++){
			output << j << " " << i << " " << array[i][j] << endl;
		}
	}
	
	for (int i=0; i<image.rows; i++) 
		delete[] array[i];
	delete[] array;
	for (int i=0; i<image.rows; i++)
		delete[] flagmat[i];
	delete[] flagmat;
	return 0;
}
