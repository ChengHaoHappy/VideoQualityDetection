#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
#include <vector>

using namespace cv;

class SharpnessDetection {
public:
	/********************************************************************************
*���������� DefRto ���㲢����һ��ͼ���������
*���������� frame  ��ɫ֡ͼ
*��������ֵ��double �����ȱ�ʾֵ����Ը���Ƶ����������С��10Ϊģ��������14Ϊ���
*********************************************************************************/
	static double DefRto(Mat frame)
	{

		cv::Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);

		IplImage tmp = cvIplImage(gray);
		IplImage* img = &tmp;
		double temp = 0;
		double DR = 0;
		int i, j;//ѭ������
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
			std::cout << "�������쳣��DR= " << DR << std::endl;
		}
		else {
			std::cout << "����������, DR= " <<DR << std::endl;
		}
	}

};