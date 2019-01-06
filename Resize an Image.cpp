Mat Resize(Mat I, float s) {

	int orig_x = I.cols;
	int orig_y = I.rows;

	int tar_x = int(orig_x * s);
	int tar_y = int(orig_y * s);
	float temp = float((orig_x - 1) / (tar_x - 1));
	int x1 = 0;
	int x2 = orig_x;
	int y1 = 0;
	int y2 = orig_y;

	// Query points
	Mat X(tar_y, tar_x, CV_32FC1);
	Mat Y(tar_y, tar_x, CV_32FC1);
	for (int i = 0; i < tar_x; i++) {
		for (int j = 0; j < tar_y; j++) { 
			X.at<float>(i, j) = 0.00;
			Y.at<float>(i, j) = 0.00;
		}
	}


	// Setting the query points
	for (int i = 0; i < tar_y; i++) {

		for (int j = 0; j < tar_x; j++) {
			Y.at<float>(i, j) = (float)j * temp;
			X.at<float>(i, j) = (float)i * temp;
			// Set X[i,j] and Y[i,j]

		}
	}
	
	int v1 = I.at<uchar>(0, 0);
	int v2 = I.at<uchar>(orig_x - 1, 0);
	int v3 = I.at<uchar>(0, orig_y - 1);
	int v4 = I.at<uchar>(orig_x - 1, orig_y - 1);


	// Output image
	Mat Output(tar_y, tar_x, CV_8UC1);
	Output.at<uchar>(0, 0) = I.at<uchar>(0,0);
	Output.at<uchar>(639, 0) = I.at<uchar>(255, 0);
	Output.at<uchar>(0, 639) = I.at<uchar>(0, 255);
	Output.at<uchar>(639, 639) = I.at<uchar>(255, 255);

	
	// Performing the interpolation
	for(int i = 1; i < tar_y - 1; i++) {
		for (int j = 1; j < tar_x - 1; j++) {

			int hx1 = (int)round(X.at<float>(i, j));
			int hy1 = (int)round(Y.at<float>(i, j));
			int hx2 = (int)round(X.at<float>(i + 1, j));
			int hy2 = (int)round(Y.at<float>(i, j + 1));
			
			float vq1 = (X.at<float>(i, j) - hx1) * I.at<uchar>(hx2, hy1) + 
					  (hx2 - X.at<float>(i, j)) * I.at<uchar>(hx1, hy1);
			float vq2 = (X.at<float>(i, j) - hx1) * I.at<uchar>(hx2, hy2) +
					  (hx2 - X.at<float>(i, j)) * I.at<uchar>(hx1, hy2);
			float vq = (Y.at<float>(i, j) - hy1) * vq2 +
				     (hy2 - Y.at<float>(i, j)) * vq1;

			Output.at<uchar>(i, j) = (int)round(vq);

			// Set Output[i,j] using X[i,j] and Y[i,j]
		}
	}
	
	return Output;
}
