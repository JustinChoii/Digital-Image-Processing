#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>
#include <math.h>


using namespace std;
using namespace cv;

void DFTShift(Mat& I) {

	int cx = I.cols / 2;
	int cy = I.rows / 2;

	// Define quadrants
	Mat q0(I, Rect(0, 0, cx, cy));   // Top-Left 
	Mat q1(I, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(I, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(I, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat temp;                           // swap quadrants (q0 and q2)
	q0.copyTo(temp);
	q3.copyTo(q0);
	temp.copyTo(q3);

	q1.copyTo(temp);                    // swap quadrant (q1 and q3)
	q2.copyTo(q1);
	temp.copyTo(q2);

}

void DFT(Mat Input, Mat& Real, Mat& Imag) {
	// I:    [In]   Gray Image
	// Real: [Out]  Real part of DFT
	// Imag: [Out]  Imaginary part of DFT

	// Converting input image to type float 
	Mat I, II;
	Input.convertTo(I, CV_32FC1);

	// Creating two channel input to represent
	Mat channels[] = { I, Mat::zeros(I.size(), CV_32FC1) };
	merge(channels, 2, II);

	// Calculate DFT
	dft(II, II);

	// Returning results
	split(II, channels);
	Real = channels[0];
	Imag = channels[1];
}

void IDFT(Mat& I, Mat Real, Mat Imag) {
	// I:    [Out] Gray Image
	// Real: [In]  Real part of DFT
	// Imag: [In]  Imaginary part of DFT

	// Merging Real and Imag
	Mat II, J;
	Mat channels[] = { Real, Imag };
	merge(channels, 2, II);

	// Calculate IDFT
	dft(II, II, DFT_INVERSE);

	// Returning results
	split(II, channels);
	magnitude(channels[0], channels[1], J);
	normalize(J, J, 0, 255, NORM_MINMAX);
	J.convertTo(I, CV_8UC1);
}

Mat NotchFilter(int s, int lowerCutOff, int upperCutOff) {
	// s: size of filter (e.g. 512 for 512x512)
	// lowerCutOff: radius of smaller circle (e.g. 40)
	// upperCutOff: radius of bigger circle (e.g. 60)

	// Sanity check
	assert(lowerCutOff < upperCutOff);

	// Creating the filter
	Mat I = Mat::ones(Size(s, s), CV_32FC1);
	int cx = I.cols / 2;
	int cy = I.rows / 2;

	// Drawing
	circle(I, Point(cx, cy), upperCutOff, 0, -1);
	circle(I, Point(cx, cy), lowerCutOff, 1, -1);

	// Return result
	return I;
}

int main() {
	Mat I = imread("uci.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat J(I.rows, I.cols, CV_32FC1);
	Mat Re;
	Mat Im;
	Mat M;
	Mat P;

	J = I;

	DFT(J, Re, Im);
	magnitude(Re, Im, M);
	Mat mag = M.clone();
	phase(Re, Im, P);
	Mat MM = M.clone();
	MM.at<float>(0, 0) = 0;
	DFTShift(M);
	DFTShift(MM);

	Mat output, output2, image, image2;
	normalize(M, image, 0, 255, NORM_MINMAX);
	normalize(MM, image2, 0, 255, NORM_MINMAX);
	
	image.convertTo(output, CV_8UC1);
	image2.convertTo(output2, CV_8UC1);

	imwrite("uci_magnitude.jpg", output);
	imwrite("MM.jpg", output2);

	Mat notch = NotchFilter(512, 30, 100);
	
	Mat N(512, 512, CV_8UC1);
	notch.convertTo(N, CV_8UC1);
	imwrite("lol.png", N);

	Mat Mask;
	Mat J_Output;

	GaussianBlur(notch, Mask, Size(9,9), 5);

	Mat C = Mask.mul(mag);

	Mat K(J.rows, J.cols, CV_8UC1);
	C.convertTo(K, CV_8UC1);
	imwrite("Mask.png", K);


	DFTShift(mag);
	polarToCart(mag, P, Re, Im);
	Mat temp;
	IDFT(temp, Re, Im);

	Mat K2(J.rows, J.cols, CV_8UC1);
	temp.convertTo(K2, CV_8UC1);
	imwrite("uci_denoised.jpg", K2);

	




	/*
	Mat I = imread("I1.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat J(I.rows, I.cols, CV_32FC1);
	Mat Re;
	Mat Im;
	Mat M;
	Mat P;

	J = I;
	DFT(J, Re, Im);
	magnitude(Re, Im, M);
	phase(Re, Im, P);

	// polarToCart(M, P, Re, Im);
	DFTShift(M);
	Mat input, output;

	normalize(M, output, 0, 255, NORM_MINMAX);
	output.convertTo(input, CV_8UC1);

	*/



	//Mat K(J.rows, J.cols, CV_8UC1);
	//I.convertTo(K, CV_8UC1);

	// imwrite("mag1_.png", input);;
	/*
	Mat I = imread("uci.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat J(I.rows, I.cols, CV_32FC1);
	J = I;
	
	for (int x = 0; x < J.rows; x++) {
		for (int y = 0; y < J.cols; y++) {
			J.at<uchar>(x, y) = (1 + sin(0.35 * pi * sqrt((x^2 + y^2))));
			//J.at<uchar>(x, y) = (1 + cos(0.2 * pi * y));
		}
	}

	normalize(J, I, 0, 255, NORM_MINMAX);
	Mat K(J.rows, J.cols, CV_8UC1);
	I.convertTo(K, CV_8UC1);

	imwrite("I5.png", K);
	*/
	return 0;
}

