//Histogram类，计算彩色图像的灰度值
#ifndef HISTOGRAM
#define HISTOGRAM
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
class Histogram {

  private:

    int histSize[3];
	float hranges[2];
    const float* ranges[3];
    int channels[3];
    public:
	Histogram() {
		histSize[0]= histSize[1]= histSize[2]= 256;
		hranges[0]= 0.0;    // 灰度值区域0到255
		hranges[1]= 255.0;
		ranges[0]= hranges; //三个通道的灰度值的范围
		ranges[1]= hranges; 
		ranges[2]= hranges; 
		channels[0]= 0;		// 三个通道
		channels[1]= 1; 
		channels[2]= 2; 
	}
	//计算机hsv图像色调通道直方图，并去除低饱和的像素点
	cv::MatND getHueHistogram(const cv::Mat &image, int minSaturation = 0)
	{
	    //直方图
		cv::MatND hist;
		//HSV空间
		cv::Mat hsv;
		//转换到HSV空间
		cv::cvtColor(image, hsv, CV_BGR2HSV);
		//掩码，只处理非零点
		cv::Mat mask;
		//剔除低于设置饱和度的点
		if (minSaturation > 0)
		{
			std::vector<cv::Mat>v;
			cv::split(hsv, v);
			cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
		}
		//色调值的范围
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		//只处理0通道
		channels[0] = 0;
		//计算直方图
		cv::calcHist(&hsv,
			1,
			channels,
			mask,
			hist,
			1,
			histSize,
			ranges
			);
		return hist;
	}
};
#endif
