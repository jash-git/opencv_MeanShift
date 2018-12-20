#ifndef OFINDER
#define OFINDER

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
class ContentFinder {

  private:

	float hranges[2];//像素值的范围
    const float* ranges[3];//指向三个通道像素值范围的指针
    int channels[3];//通道

	float threshold;//阈值
	cv::MatND histogram;//直方图

  public:
	  //初始化
	  ContentFinder() : threshold(-1.0f){

		ranges[0]= hranges; // 所有通道有相同的范围
		ranges[1]= hranges; 
		ranges[2]= hranges; 
	}
   
	// 设置阈值[0~1]
	void setThreshold(float t) {

		threshold= t;
	}

	// 得到阈值
	float getThreshold() {

		return threshold;
	}

	// 设置直方图
	void setHistogram(const cv::MatND& h) {

		histogram= h;
		//直方图归一化
		cv::normalize(histogram,histogram,1.0);
	}

	// 反投影直方图
	cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int *channels, int dim) {

		cv::Mat result;

		hranges[0]= minValue;
		hranges[1]= maxValue;

		for (int i=0; i<dim; i++)
			this->channels[i]= channels[i];

	
		   cv::calcBackProject(&image,
                      1,            //1张图片
                      this->channels,     //通道
                      histogram,    // 直方图
                      result,       // 结果
                      ranges,       // 每个维度的灰度值范围
                      255.0         // 缩放因子 
		   );	
        // 阈值化
		if (threshold>0.0)
			cv::threshold(result, result, 255*threshold, 255, cv::THRESH_BINARY);
		return result;
	}

};


#endif
