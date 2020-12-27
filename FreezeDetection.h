/*
* ���涳���쳣
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
#include <vector>

using namespace cv;


class FreezeDetection {
public:
	//Mat baseimg;   //ȫ�ֱ���
	const static int T = 10;   //����ͬһ����λ�õı仯��ΧΪ10                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
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

		int NUM = 0; //�仯�����������
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
		printf("NUM = %d�����ز���� k=%f\n", NUM,k);
		if (k < 0.15) {
			printf("��Ƶ�����쳣\n");
		}
		else {
			printf("��Ƶδ����\n");
		}
	}
	static void FreezeDetectionStart(String src, String base) {
		cv::Mat srcImage = cv::imread(src);	//��ȡ
		cv::Mat baseimg = cv::imread(base);
		float k = 0;
		ViewFreeze2(srcImage, baseimg, k);
	}
};
