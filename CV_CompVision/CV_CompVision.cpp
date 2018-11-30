#include <iostream>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <cmath>

using namespace cv;
using namespace std;

int clamp(int val, int minVal, int maxVal) {
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;
	return val;
}

void norm7(Mat &src, Mat &D, Mat &out, double k) {
	out = Mat::zeros(src.size(), CV_8U);
	Mat I;
	integral(src, I);
	imshow("intgralnoe", I);
	int filtSize = 0; //radius of matrix
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			filtSize = ceil(k / D.at<uchar>(i, j));
			//cout << filtSize << endl;
			out.at<uchar>(i, j) = (uchar)((double)(I.at<uchar>(clamp(i + filtSize, 0, src.rows), clamp(j + filtSize, 0, src.cols)) 
									+ I.at<uchar>(clamp(i - filtSize, 0, src.rows), clamp(j - filtSize, 0 , src.cols))
									- I.at<uchar>(clamp(i + filtSize, 0, src.rows), clamp(j - filtSize, 0, src.cols)) 
									- I.at<uchar>(clamp(i - filtSize, 0, src.rows), clamp(j + filtSize, 0, src.cols))) / (double)pow((filtSize+1),2));
		}
	
	}
}

int main()
{	
	double k = 255;
	vector<Point2f> corners;
	Mat source, gray, histNorm, edges, distance, integ;
	// [[ 1 ]]
	source = imread("source4.png");
	imshow("source", source);
	// [[ 2 ]]
	cvtColor(source, gray, cv::COLOR_RGB2GRAY);
	imshow("gray", gray);
	// [[ 3 ]]
	equalizeHist(gray, histNorm);
	imshow("histNorm", histNorm);
	// [[ 4 ]]
	Canny(gray, edges, 60, 120);
	imshow("Canny", edges);
	// [[ 5 ]]
	goodFeaturesToTrack(gray, corners, 200, 0.5, 3);
	for (int i = 0; i < corners.size(); ++i) {
		circle(edges, corners[i], 2, Scalar(255), 2 ,8, 0);
	}
	imshow("corners + edges", edges);
	// [[ 6 ]]
	Mat bw;
	gray.copyTo(bw);
	threshold(bw, bw, 40, 255, CV_THRESH_BINARY);
	distanceTransform(bw, distance, CV_DIST_L2, 3);
	normalize(distance, distance, 0, 1, NORM_MINMAX);
	imshow("distance transform", distance);
	// [[ 7 ]]
	norm7(gray, distance, integ, k);
	//normalize(integ, integ, 0, 1, NORM_MINMAX);
	imshow("7", integ);
	waitKey(0);
	return 0;
}