/*
* 视频抖动异常检测
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <vector>

using namespace cv;

class ShakeDetection {
public:
	ShakeDetection() {

	}
	//行投影求和
	static void Row_sum(Mat src, std::vector<int>& rowsum, float& meanR)
	{
		float RS = 0;
		for (int i = 0; i < src.rows; i++)
		{
			uchar* ptr = src.ptr<uchar>(i);
			int Rsum = 0;
			for (int j = 0; j < src.cols; j++)
			{
				Rsum += ptr[j];
			}
			RS += Rsum;
			rowsum.push_back(Rsum);
		}
		meanR = RS / src.rows;
	}
	//列向投影求和
	static void Col_sum(Mat src, std::vector<int>& rowsum, float& meanC)
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
	static void Cal_min(std::vector<int> Crt, std::vector<int> Ref, int m, int& minL)
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
	static void ViewShake(Mat src, float& k, Mat baseimg)
	{
		if (baseimg.empty())
		{
			baseimg = src.clone();
			k = 0;
			return;
		}
		std::vector<int> Rsum_src, Csum_src, Rsum_base, Csum_base;
		float meanR_src = 0, meanC_src = 0, meanR_base = 0, meanC_base = 0;

		Row_sum(src, Rsum_src, meanR_src);
		Col_sum(src, Csum_src, meanC_src);
		Row_sum(baseimg, Rsum_base, meanR_base);
		Col_sum(baseimg, Csum_base, meanC_base);

		for (int i = 0; i < Rsum_src.size(); i++)
		{
			Rsum_src[i] = Rsum_src[i] - meanR_src;
			Rsum_base[i] = Rsum_base[i] - meanR_base;
		}
		for (int i = 0; i < Csum_src.size(); i++)
		{
			Csum_src[i] = Csum_src[i] - meanC_src;
			Csum_base[i] = Csum_base[i] - meanC_base;
		}

		int R_L = 0, C_L = 0; //行、列向的偏移像素
		Cal_min(Rsum_src, Rsum_base, 20, R_L);  //m初值设置为20
		Cal_min(Csum_src, Csum_base, 20, C_L);

		printf("行偏移像素值 R_L= %d\n", R_L);
		printf("列偏移像素值 C_L= %d\n", C_L);

		//设计阈值判断是否抖动
		if (abs(R_L) > 10 || abs(C_L) > 10) {
			printf("抖动异常\n");
		}
		else {
			printf("抖动正常\n");
		}

	}

	static void ShakeDetectionStart(String src, String base) {
		cv::Mat srcImage = cv::imread(src);	//读取
		cv::Mat baseimg = cv::imread(base);
		float k = 0;
		ViewShake(srcImage, k, baseimg);
	}
};

