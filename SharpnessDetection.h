#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <vector>

using namespace cv;

class SharpnessDetection {
public:
	/********************************************************************************
*函数描述： DefRto 计算并返回一幅图像的清晰度
*函数参数： frame  彩色帧图
*函数返回值：double 清晰度表示值，针对该视频，当清晰度小于10为模糊，大于14为清楚
*********************************************************************************/
	static double DefRto(Mat frame)
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

	static void SharpnessDetectionStart(std::string src) {
		cv::Mat imageData = cv::imread(src.c_str());
		double DR = DefRto(imageData);
		if (DR < 10) {
			std::cout << "清晰度异常，DR= " << DR << std::endl;
		}
		else {
			std::cout << "清晰度正常, DR= " <<DR << std::endl;
		}
	}

};
