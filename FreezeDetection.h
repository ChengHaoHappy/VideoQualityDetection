/*
* 画面冻结异常
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <opencv2\imgproc\types_c.h>
#include <vector>

using namespace cv;
using namespace std;


class FreezeDetection {
public:
	//Mat baseimg;   //全局变量
	const static int T = 10;   //假设同一像素位置的变化范围为10                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	static void ViewFreeze2(Mat src, Mat baseimg, float& k)
	{
		if (baseimg.empty())
		{
			baseimg = src.clone();
			k = 0;
			return;
		}
		Mat src_gray, base_gray;
		cvtColor(src, src_gray, CV_BGR2GRAY);
		cvtColor(baseimg, base_gray, CV_BGR2GRAY);

		int NUM = 0; //变化大的像素数量
		for (int i = 0; i < src_gray.rows; i++)
		{
			uchar* ptr1 = src_gray.ptr<uchar>(i);
			uchar* ptr2 = base_gray.ptr<uchar>(i);

			for (int j = 0; j < src_gray.cols; j++)
			{
				if (abs(ptr1[j] - ptr2[j] > T))
					NUM++;
			}
		}
		k = (float)NUM / (src_gray.rows * src_gray.cols);
		//printf("NUM = %d，像素差异比 k=%f\n", NUM,k);
		if (k < 0.15) {
			cout << "视频冻结异常" << endl;
			//printf("视频冻结异常，NUM = %d，像素差异比 k=%f\n", NUM, k);
		}
		/*else {
			printf("视频未冻结，NUM = %d，像素差异比 k=%f\n", NUM, k);
		}*/
	}
	static void FreezeDetectionStart(String src, String base) {
		cv::Mat srcImage = cv::imread(src);	//读取
		cv::Mat baseimg = cv::imread(base);
		if (srcImage.data  == 0 || baseimg.data == 0)
		{
			cerr << "Image reading error" << endl;
			system("pause");
			return;
		}
		float k = 0;
		ViewFreeze2(srcImage, baseimg, k);
	}
};

