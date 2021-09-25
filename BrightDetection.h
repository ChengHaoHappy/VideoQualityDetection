/*
* �����쳣
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
#include <vector>

using namespace cv;
using namespace std;

class BrightDetection {
public:
	static void JudgeBright1(Mat src, float& da)
	{
		Mat gray;
		cvtColor(src, gray, CV_BGR2GRAY); // ��ͼ��ת��Ϊ�Ҷ�ͼ

		Mat means, stddev;
		meanStdDev(gray, means, stddev); //�����ֵ�ͱ�׼��
		da = means.at<double>(0);
	}
	static void JudgeBright2(Mat src, float& da, float& k)
	{
		Mat gray;
		cvtColor(src, gray, CV_BGR2GRAY);
		float Sum_a = 0;
		int Hist[256] = { 0 };
		double meanV = 0;
		for (int i = 0; i < gray.rows; i++)
		{
			uchar* ptr = gray.ptr<uchar>(i);
			for (int j = 0; j < gray.cols; j++)
			{
				Sum_a = Sum_a + (ptr[j] - 100);
				Hist[ptr[j]]++;
			}
		}
		da = Sum_a / (gray.rows * gray.cols);

		float D = abs(da);
		float ma = 0, sum_m = 0;
		for (int i = 0; i < 256; i++)
		{
			sum_m = sum_m + abs((i - 100) - da) * Hist[i];
		}
		ma = sum_m / (gray.rows * gray.cols);
		float M = abs(ma);

		k = D / M;
	}

	/*********************************************************************************************************************************************************
*���������� brightnessException     
*���������� InputImg    ��Ҫ�����ͼƬ��BGR��Ÿ�ʽ����ɫ��3ͨ�������Ҷ�ͼ��Ч
*           cast        �������ƫ��ֵ��С��1��ʾ�Ƚ�����������1��ʾ���������쳣����cast�쳣ʱ��da����0��ʾ������daС��0��ʾ����
*��������ֵ��   ����ֵͨ��cast��da�������÷��أ�����ʽ����ֵ
**********************************************************************************************************************************************************/
	static void brightnessException(Mat InputImg, float& cast, float& da)
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
	static bool BrightDetectionStart(std::string src) {
		float brightcast, brightda;
		cv::Mat imageData = cv::imread(src.c_str());
		if (imageData.data == 0)
		{
			cerr << "Image reading error" << endl;
			system("pause");
			return false;
		}
		brightnessException(imageData, brightcast, brightda);
		//std::cout << "brightcast: " << brightcast << std::endl;
		if (brightcast > 1) {
			string brightDes = (brightda > 0) ? "ƫ��" : "ƫ��";
			cout << "�����쳣:" << brightDes  << endl;
			return false;
			//std::cout << "�����쳣, brightda=" << brightda << " " << brightDes << std::endl;
		}
		/*else {
			std::cout << "����������brightcast="<< brightcast << std::endl;
		}*/
		return true;
	}
};
