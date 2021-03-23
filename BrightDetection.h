/*
* 亮度异常
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <vector>

using namespace cv;
using namespace std;

class BrightDetection {
public:
	static void JudgeBright1(Mat src, float& da)
	{
		Mat gray;
		cvtColor(src, gray, CV_BGR2GRAY); // 将图像转换为灰度图

		Mat means, stddev;
		meanStdDev(gray, means, stddev); //计算均值和标准差
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
*函数描述： brightnessException     
*函数参数： InputImg    需要计算的图片，BGR存放格式，彩色（3通道），灰度图无效
*           cast        计算出的偏差值，小于1表示比较正常，大于1表示存在亮度异常；当cast异常时，da大于0表示过亮，da小于0表示过暗
*函数返回值：   返回值通过cast、da两个引用返回，无显式返回值
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
			string brightDes = (brightda > 0) ? "偏亮" : "偏暗";
			cout << "亮度异常:" << brightDes  << endl;
			return false;
			//std::cout << "亮度异常, brightda=" << brightda << " " << brightDes << std::endl;
		}
		/*else {
			std::cout << "亮度正常，brightcast="<< brightcast << std::endl;
		}*/
		return true;
	}
};
