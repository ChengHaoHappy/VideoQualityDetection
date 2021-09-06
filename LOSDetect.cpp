#include<iostream>
#include <opencv2/opencv.hpp>  
#include <math.h>　　　　
#include "Detection.h"

using namespace std;
using namespace cv;

const double EPS = 1e-6;

class LOSDetect : public Detection {
public:
	int getHistImage(const MatND& hist)
	{

		double maxValue = 0;
		double minValue = 0;
		double sum = 0;
		double loss = 0;
		for (int i = 0; i < hist.rows; i++)
		{
			float b = hist.at<float>(i);
			if (i < 5) {
				loss += b;
			}
			sum += b;

		}
		double loss_rate = loss / sum;
		if (loss_rate + EPS > 0.9) {
			cout << "信号丢失异常" << endl;
			return 5;
		}
		return 0;
	}

	int Detect(Mat src)
	{
		//Mat src = imread(imagesrc.c_str(), 0);
		if (!src.data)
		{
			cout << "no picture!\n";
			return 5;
		}
		int a = src.channels();//a = 1通道
		int image_count = 1;//要计算直方图的图像的个数
		int channels[1] = { 0 };//图像的通道'

		Mat out;//计算所得直方图
		int dims = 1;//得到直方图的维数
		int histsize[1] = { 256 };//直方图横坐标的子区间数
		float hrange[2] = { 0, 255 };//区间的总范围
		const float* ranges[1] = { hrange };//指针数组

		calcHist(&src, image_count, channels, Mat(), out, dims, histsize, ranges); //计算灰度图的直方图

		return getHistImage(out);
	}
};


