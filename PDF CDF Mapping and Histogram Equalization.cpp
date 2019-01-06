Mat FindPDF(Mat I) {

	Mat pdf(256, 1, CV_32FC1);
	int total = I.rows * I.cols;

	for (int i = 0; i < 256; i++) {
		pdf.at<float>(i) = 0;
	}

	for (int i = 0; i < I.rows; i++) {
		for (int j = 0; j < I.cols; j++) {
			// Fill in pdf array
			int value = I.at<uchar>(i, j);
			pdf.at<float>(value) += 1;
			

		}
	}

	for (int i = 0; i < 256; i++) {
		float temp = pdf.at<float>(i);
		pdf.at<float>(i) = temp / total;
	}
	// convert histogram to pdf

	return pdf;

}

Mat FindCDF(Mat pdf) {

	Mat cdf(256, 1, CV_32FC1);

	for (int i = 0; i < 256; i++) {
		cdf.at<float>(i) = 0;
	}
	cdf.at<float>(0) = pdf.at<float>(0);
	for (int i = 1; i < 256; i++) {
		cdf.at<float>(i) = cdf.at<float>(i-1) + pdf.at<float>(i);

		// Fill in cdf array
	}

	return cdf;
}

Mat FindEqualMapping(Mat cdf) {

	Mat target(256, 1, CV_32FC1);
	float temp = 0.00390625;

	for (int i = 0; i < 256; i++) {
		target.at<float>(i) = 0;
	}
	target.at<float>(0) = temp;

	for (int i = 1; i < 256; i++) {
		// Fill in target cdf
		target.at<float>(i) = target.at<float>(i - 1) + temp;
	}

	/*
	std::cout << "cdf" << endl;
	for (int i = 0; i < 256; i++) {
		std::cout << i << ": " << cdf.at<float>(i) << endl;
	}

	std::cout << "target" << endl;
	for (int i = 0; i < 256; i++) {
		std::cout << i << ": " << target.at<float>(i) << endl;
	}
	*/

	
	Mat mapping(256, 1, CV_8UC1);
	
	for (int i = 0; i < 256; i++) {
		float target_index = 0.00;

		// Find closet target[j] to cdf[i]
		// Find closest target[210] to cdf[191]
		// cdf[191] = target[210]
		float counter = cdf.at<float>(i);
		counter = counter / temp;
		target_index = counter - 1;
		target_index = round(target_index);

		mapping.at<uchar>(i) = (int)target_index;
		// mapping[i] = j
		// mapping[191] = 210;
	}
	
	/*
	for (int i = 0; i < 256; i++) {
		std::cout << i << ": "  << (int)mapping.at<uchar>(i) << endl;
	}
	*/

	return mapping;
}

Mat ApplyEqualization(Mat I, Mat mapping) {

	Mat Output(I.rows, I.cols, CV_8UC1);

	for (int i = 0; i < I.rows; i++) {

		for (int j = 0; j < I.cols; j++) {
			int i_value = I.at<uchar>(i, j);
			int map_value = mapping.at<uchar>(i_value);
			Output.at<uchar>(i, j) = map_value;


			// Set the output[i,j]
		}
	}

	return Output;

}

Mat HistogramEqualization(Mat I) {
	Mat currPDF = FindPDF(I);
	Mat currCDF = FindCDF(currPDF);
	Mat eqMap = FindEqualMapping(currCDF);
	Mat output = ApplyEqualization(I, eqMap);
	return output;
}