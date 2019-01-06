#include "stdafx.h"
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;

Mat BoxFilterGray3(Mat I) {
	for (int i = 1; i < I.rows -1; i++) {
		for (int j = 1; j < I.cols-1; j++) {
			I.at<uchar>(i, j) = ((I.at<uchar>(i-1,j-1) + I.at<uchar>(i, j-1) + I.at<uchar>(i+1, j-1) +
								 I.at<uchar>(i-1,j) + I.at<uchar>(i+1, j) + 
								 I.at<uchar>(i-1, j+1) + I.at<uchar>(i, j+1) + I.at<uchar>(i+1, j+1))/8);

		}
	}
	return I;

}

int main(){
	Mat I = imread("text.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat J(I.rows, I.cols, CV_8UC1);
	
	J = BoxFilterGray3(I);
	imwrite("textBlur.jpg", J);
	return 0;
}

