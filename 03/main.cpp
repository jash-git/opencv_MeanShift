#include <iostream>
#include <opencv2/opencv.hpp>
 
using namespace cv;
 
int main()
{
	Mat srcImg1 = imread("1.jpg");
	Mat srcImg2 = imread("2.jpg");
 
	Mat srcHSV1, srcHSV2;
	cvtColor(srcImg1, srcHSV1, COLOR_RGB2HSV);
	cvtColor(srcImg2, srcHSV2, COLOR_RGB2HSV);
 
	Rect rect(130, 120, 55, 55);
	Mat imgROI = srcHSV1(rect);
	rectangle(srcImg1, rect, Scalar(0, 0, 255));
	rectangle(srcImg2, rect, Scalar(0, 0, 255));
 
	imshow("srcImg1", srcImg1);
	imwrite("srcImg1.png", srcImg1);
 
	int histSize[1] = {256};
	float hranges[2] = {0, 255};
	const float* ranges[1] = {hranges};
	int channels[1] = {0};
 
	Mat dstHist;
	calcHist(&imgROI, 1, channels, Mat(), dstHist, 1, histSize, ranges);
	normalize(dstHist, dstHist, 0.0, 1.0, NORM_MINMAX);
 
	Mat backprojection;	
	calcBackProject(&srcHSV2, 1, channels, dstHist, backprojection, ranges, 255.0);
	imshow("backprojection", backprojection);
	imwrite("backproj.png", backprojection);
 
	TermCriteria criteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 10, 0.01);
	meanShift(backprojection, rect, criteria);//Meanshift会自动更新这个rect
	rectangle(srcImg2, rect, Scalar(0, 255, 0));
	imshow("srcImg2", srcImg2);
	imwrite("srcImg2.png", srcImg2);
 
	waitKey(0);
 
	return 0;
}