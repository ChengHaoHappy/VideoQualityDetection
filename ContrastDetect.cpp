#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <vector>
#include "Detection.h"

using namespace cv;
using namespace std;


class ContrastDetect : public Detection {
public:
	void ContrastException(Mat InputImg, float& cast, float& da)
	{
		Mat GRAYimg;
		cvtColor(InputImg, GRAYimg, CV_BGR2GRAY);
		float a = 0;
		int Hist[256];
		for (int i = 0; i < 256; i++)
			Hist[i] = 0;
		for (int i = 0; i < GRAYimg.rows; i++)
		{
			for (int j = 0; j < GRAYimg.cols; j++)
			{
				a += float(GRAYimg.at<uchar>(i, j) - 128);//在计算过程中，考虑128为亮度均值点，统计偏离的总数
				int x = GRAYimg.at<uchar>(i, j);
				Hist[x]++; //统计每个亮度的次数
			}
		}
		da = a / float(GRAYimg.rows * InputImg.cols);  //
		float D = abs(da);
		float Ma = 0;
		for (int i = 0; i < 256; i++)
		{
			Ma += abs(i - 128 - da) * Hist[i];
		}
		Ma /= float((GRAYimg.rows * GRAYimg.cols));
		float M = abs(Ma);
		float K = D / M;
		cast = K;
		return;
	}
	int Detect(Mat imageData) {
		float brightcast, brightda;
		//cv::Mat imageData = cv::imread(src.c_str());
		if (imageData.data == 0)
		{
			cerr << "Image reading error" << endl;
			system("pause");
			return 3;
		}
		ContrastException(imageData, brightcast, brightda);
		if (brightcast > 1) {
			std::string brightDes = (brightda > 0) ? "偏亮" : "偏暗";
			std::cout << "对比度异常：" << brightDes << std::endl;
			return 3;
		}
		return 0;
	}
};