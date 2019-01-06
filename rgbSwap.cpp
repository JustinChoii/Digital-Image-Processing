#include "stdafx.h"
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
	Mat I = imread("nature.jpg", CV_LOAD_IMAGE_COLOR);
	Mat J(I.rows, I.cols, CV_8UC3);
	for (int i = 0; i < I.rows; i++) {
		for (int j = 0; j < I.cols; j++) {
			Vec3b pixel = I.at<Vec3b>(i, j);
			int b = pixel[0];
			int g = pixel[1];
			int r = pixel[2];
			J.at<Vec3b>(i, j)[0] = r;
			J.at<Vec3b>(i, j)[1] = b;
			J.at<Vec3b>(i, j)[2] = g;
		}
	}
	imwrite("natureSwap.jpg", J);
	return 0;
}