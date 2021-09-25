/*
* �Աȶ��쳣
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
#include <vector>

using namespace cv;
using namespace std;

class ContrastDetection {
public:
	static void ContrastException(Mat InputImg, float& cast, float& da)
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
				a += float(GRAYimg.at<uchar>(i, j) - 128);//�ڼ�������У�����128Ϊ���Ⱦ�ֵ�㣬ͳ��ƫ�������
				int x = GRAYimg.at<uchar>(i, j);
				Hist[x]++; //ͳ��ÿ�����ȵĴ���
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
	static void ContrastDetectionStart(std::string src) {
		float brightcast, brightda;
		cv::Mat imageData = cv::imread(src.c_str());
		if (imageData.data == 0)
		{
			cerr << "Image reading error" << endl;
			system("pause");
			return;
		}
		ContrastException(imageData, brightcast, brightda);
		//std::cout << "brightcast: " << brightcast << std::endl;
		if (brightcast > 1) {
			std::string brightDes = (brightda > 0) ? "ƫ��" : "ƫ��";
			std::cout << "�Աȶ��쳣��" << brightDes << std::endl;
		}
		/*else {
			std::cout << "�Աȶ����� " << std::endl;
		}*/
	}
};

