/*
* 视频抖动异常检测
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <vector>
#include "Detection.h"

using namespace cv;
using namespace std;

class ShakeDetect : public Detection{
public:
	ShakeDetect() {

	}
	//行投影求和
	void Row_sum(Mat src, std::vector<int>& rowsum, float& meanR)
	{
		float RS = 0;
		for (int i = 0; i < src.rows; i++)
		{
			uchar* ptr = src.ptr<uchar>(i);
			// Rsum 为第k帧图像第i行的灰度值
			int Rsum = 0;
			for (int j = 0; j < src.cols; j++)
			{
				Rsum += ptr[j];
			}
			RS += Rsum;
			rowsum.push_back(Rsum);
		}
		meanR = RS / src.rows;  //行像素和均值
	}
	//列向投影求和
	void Col_sum(Mat src, std::vector<int>& rowsum, float& meanC)
	{
		float CS = 0;
		for (int i = 0; i < src.cols; i++)
		{
			int Csum = 0;
			for (int j = 0; j < src.rows; j++)
			{
				uchar* ptr = src.ptr<uchar>(j);
				Csum += ptr[i];
			}
			CS += Csum;
			rowsum.push_back(Csum);
		}
		meanC = CS / src.cols;
	}
	//计算互相关最小值
	void Cal_min(std::vector<int> Crt, std::vector<int> Ref, int m, int& minL)
	{
		int Idx = 0;
		float minV = 999999999999;
		for (int w = 1; w <= 2 * m + 1; w++)
		{
			float sum_Diff = 0;
			for (int i = 0; i < Crt.size() - 2 * m; i++)
			{
				sum_Diff += pow((Crt[i + w - 1] - Ref[i + m]), 2);
			}
			if (sum_Diff < minV)
			{
				minV = sum_Diff;
				Idx = w;
			}
		}
		minL = m + 1 - Idx;
	}
	//功能函数-画面抖动检测
	int ViewShake(Mat src, float& k, Mat baseimg)
	{
		if (baseimg.empty())
		{
			baseimg = src.clone();
			k = 0;
			return 8;
		}
		std::vector<int> Rsum_src, Csum_src, Rsum_base, Csum_base;
		float meanR_src = 0, meanC_src = 0, meanR_base = 0, meanC_base = 0;

		Row_sum(src, Rsum_src, meanR_src);
		Col_sum(src, Csum_src, meanC_src);
		Row_sum(baseimg, Rsum_base, meanR_base);
		Col_sum(baseimg, Csum_base, meanC_base);

		//计算行方向的投影：说白了，行灰度投影就是将每行的像素替换为了该行所有像素和与像素均值的差值，列灰度投影同理。
		for (int i = 0; i < Rsum_src.size(); i++)
		{
			Rsum_src[i] = Rsum_src[i] - meanR_src;
			Rsum_base[i] = Rsum_base[i] - meanR_base;
		}
		//
		for (int i = 0; i < Csum_src.size(); i++)
		{
			Csum_src[i] = Csum_src[i] - meanC_src;
			Csum_base[i] = Csum_base[i] - meanC_base;
		}

		int R_L = 0, C_L = 0; //行、列向的偏移像素
		Cal_min(Rsum_src, Rsum_base, 20, R_L);  //m初值设置为20
		Cal_min(Csum_src, Csum_base, 20, C_L);

		

		//设计阈值判断是否抖动
		if (abs(R_L) > 10 || abs(C_L) > 10) {
			printf("抖动异常\n");
			return 8;
		}
		return 0;

	}

	int  Detect(Mat srcImage, Mat baseimg) {
		//cv::Mat srcImage = cv::imread(src);	//读取
		//cv::Mat baseimg = cv::imread(base);
		if (!srcImage.data || !baseimg.data)
		{
			cout << "no picture!\n";
			return 8;
		}
		float k = 0;
		return ViewShake(srcImage, k, baseimg);
	}
};

