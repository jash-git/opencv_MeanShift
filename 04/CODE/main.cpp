//不显示CMD窗口，或者关闭CMD窗口，程序不退出
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#include"histogram.h"
#include"contentFinder.h"
//需要包含此文件才能调用Meanshift函数
#include<opencv2\video\tracking.hpp>
using namespace cv;
int main()
{
	//读取第一张图片
	Mat image = imread("0001.jpg");
	//定义感兴趣区域
	Mat imaRoi = image(Rect(213, 121, 21, 95));
	//设置最小饱和度
	int minSat = 65;
	Histogram h;
	//获得imaRoi的直方图特征
	MatND hist = h.getHueHistogram(imaRoi, minSat);
	ContentFinder finder;
	finder.setHistogram(hist);
	//读取第二张图片
	Mat findimage = imread("0024.jpg");
	Mat hsv;
	//将第二张图片转换为ＨＳＶ格式
	cvtColor(findimage, hsv, CV_BGR2HSV);
	std::vector<Mat>v;
	split(hsv, v);
	//将低于 最小饱和度的像素点 设置为0
	cv::threshold(v[1], v[1], minSat, 255, THRESH_BINARY);
	int ch[1] = { 0 };
	cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch, 1);
	//剔除低饱和的点
	bitwise_and(result, v[1], result);
	//在第一张图片上画出感兴趣区域的位置
	rectangle(image, Rect(213, 121, 21, 95), Scalar(255, 0, 0));
	//设置迭代停止条件
	TermCriteria criteria(TermCriteria::MAX_ITER, 100, 0.01);
	//预定义初始矩形区域
	Rect rect(213, 121, 21, 95);
	//调用meanshift算法，最终得到rect，相似区域的位置
    meanShift(result, rect, criteria);
	//在第二张图片上，显示相似区域的位置
	rectangle(findimage, rect, Scalar(255, 0, 0));
	imshow("第一张", image);
	imshow("第二张", findimage);
	cv::waitKey(0);
}
