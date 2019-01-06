Mat CreateGaussianFilter() {

	// This is your empty kernel
	// Every entry is a float value
	Mat I(5, 5, CV_32FC1);

	// This is 1D Gaussian kernel values
	float g[] = { 0.05, 0.25, 0.4, 0.25, 0.05 };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {


			I.at<float>(i, j) = g[i] * g[j];
			
		}
	}
	
	return I;
}

Mat ApplyFilter(Mat input, Mat filter) {

	// This is your empty output Mat
	Mat result(input.rows, input.cols, CV_8UC1);
	Mat test = input;
	float temp = 0.00;
	float total = 0.00;
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			// no need to flip the values of filter horizontally + vertically as it's already symetrical.

			for (int a = 2; a <= filter.rows + 1; a++) {
				for (int b = 2; b <= filter.cols + 1; b++) {
					if (i >= 2 && i <= result.rows - 2 && j >= 2 && j <= result.cols - 2) {
						float input_pixel;
						input_pixel = (float)input.at<uchar>(i - a, j - b);
						temp = input_pixel * filter.at<float>(a - 2, b - 2);
						temp = floor(temp);
						total += temp;

					}
				}
			}
			total = (unsigned char)total;
			test.at<uchar>(i, j) = total;
				
			
			result.at<uchar>(i, j) = test.at<uchar>(i, j);

			/*

			// Perform convolution for this pixel
			for (...){
			for (...){

			// Checking boundary condition
			if (...){

			}
			}
			}

			// Assign the output value for pixel
			result.at<uchar>(i, j) = ...

			*/
		}
	}

	return result;
}

Mat Reduce(Mat input) {

	// THis is your empty output image
	Mat output(input.rows / 2, input.cols / 2, CV_8UC1);

	// Calculate each pixel of output image
	for (int i = 0; i < output.rows; i++) {
		for (int j = 0; j < output.cols; j++) {
			output.at<uchar>(i, j) = ((input.at<uchar>(i * 2, j * 2) + input.at<uchar>(i * 2, j * 2 + 1) + input.at<uchar>(i * 2 + 1, j * 2) + input.at<uchar>(i * 2 + 1, j * 2 + 1) / 4));

		}
	}
	return output;
}

Mat Deduct(Mat I, Mat J) {
	
	// Intermediate pixel to keep the differences
	// Each entry is int
	Mat intermediate(I.rows, I.cols, CV_32SC1);
	int minVal = 256;
	int maxVal = -256;
	for (int i = 0; i < intermediate.rows; i++) {
		for (int j = 0; j < intermediate.cols; j++) {
			/*
				Calculate the intermediate pixel values
			*/
		}
	}
	float dynamicRange = maxVal - minVal;

	// The output image of type unsigned char for each pixel
	Mat result(I.rows, I.cols, CV_8UC1);
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			/*
			// Calculate the output pixels
			result.at<uchar>(i, j) = ...
			*/
		}
	}

	return result;
}