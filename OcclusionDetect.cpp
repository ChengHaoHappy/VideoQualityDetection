#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
#include "Detection.h"

using namespace cv;
using namespace std;

class OcclusionDetect : public Detection {
public:
	bool Cover2(Mat src)
	{
		bool IsCovered = false;
		Mat gray;
		cvtColor(src, gray, CV_BGR2GRAY); //�Ҷ�ͼ

		Mat lapImg;
		Laplacian(gray, lapImg, gray.depth()); //��Եͼ

		int CoverBlock = 0;
		for (int i = 0; i < gray.rows - 2; i = i + gray.rows / 3)
		{
			for (int j = 0; j < gray.cols - 2; j = j + gray.cols / 3)
			{
				float sigma_g = 0;
				float sigma_l = 0;
				Rect r = Rect(j, i, gray.cols / 3, gray.rows / 3); //���Σ����ý�ȡͼƬ��λ�úʹ�С
				Mat subImg = gray(r);
				Mat subLap = lapImg(r);
				Mat means, stddev_g, stddev_l;
				meanStdDev(subImg, means, stddev_g);  //���ֵ�ͱ�׼��
				meanStdDev(subLap, means, stddev_l);

				sigma_g = stddev_g.at<double>(0);
				sigma_l = stddev_l.at<double>(0);
				//cout << "sigma_g=" << sigma_g << ", sigma_l = " << sigma_l << endl;
				if (sigma_g < 15 && sigma_l < 10)
					CoverBlock++;
			}
		}
		//printf("CoverBlock=%d\n", CoverBlock);
		IsCovered = CoverBlock > 1 ? true : false;

		return IsCovered;
	}

	int Detect(std::string src) {
		cv::Mat imageData = cv::imread(src.c_str());
		if (!imageData.data)
		{
			cout << "no picture!\n";
			return 7;
		}
		bool IsCovered = Cover2(imageData);
		if (IsCovered) {
			std::cout << "��Ƶ���ڵ� " << std::endl;
			return 7;
		}
		return 0;
	}

};