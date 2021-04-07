#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <vector>
#include "Detection.h"

using namespace cv;

class SharpnessDetect : public Detection {
public:
	double DefRto(Mat frame)
	{

		cv::Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);

		IplImage tmp = cvIplImage(gray);
		IplImage* img = &tmp;
		double temp = 0;
		double DR = 0;
		int i, j;//循环变量
		int height = img->height;
		int width = img->width;
		int step = img->widthStep / sizeof(uchar);
		uchar* data = (uchar*)img->imageData;
		double num = width * height;

		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				temp += sqrt((pow((double)(data[(i + 1) * step + j] - data[i * step + j]), 2) + pow((double)(data[i * step + j + 1] - data[i * step + j]), 2)));
				temp += abs(data[(i + 1) * step + j] - data[i * step + j]) + abs(data[i * step + j + 1] - data[i * step + j]);
			}
		}
		DR = temp / num;
		return DR;
	}

	int Detect(Mat imageData) {
		//cv::Mat imageData = cv::imread(src.c_str());
		if (!imageData.data)
		{
			cout << "no picture!\n";
			return 9;
		}
		double DR = DefRto(imageData);
		if (DR < 10) {
			std::cout << "清晰度异常" << std::endl;
			return 9;
		}
		return 0;
	}

};
