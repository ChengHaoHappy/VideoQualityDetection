#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件

using namespace cv;

class Occlusion {
public:
	static bool Cover2(Mat src)
	{
		bool IsCovered = false;
		Mat gray;
		cvtColor(src, gray, CV_BGR2GRAY); //灰度图

		Mat lapImg;
		Laplacian(gray, lapImg, gray.depth()); //边缘图

		int CoverBlock = 0;
		for (int i = 0; i < gray.rows - 2; i = i + gray.rows / 3)
		{
			for (int j = 0; j < gray.cols - 2; j = j + gray.cols / 3)
			{
				float sigma_g = 0;
				float sigma_l = 0;
				Rect r = Rect(j, i, gray.cols / 3, gray.rows / 3); //矩形，设置截取图片的位置和大小
				Mat subImg = gray(r);
				Mat subLap = lapImg(r);
				Mat means, stddev_g, stddev_l;
				meanStdDev(subImg, means, stddev_g);  //求均值和标准差
				meanStdDev(subLap, means, stddev_l);

				sigma_g = stddev_g.at<double>(0);
				sigma_l = stddev_l.at<double>(0);

				if (sigma_g < 25 && sigma_l < 10)
					CoverBlock++;
			}
		}
		IsCovered = CoverBlock > 0 ? true : false;

		return IsCovered;
	}

	static void OcclusionDetectionStart(std::string src) {
		cv::Mat imageData = cv::imread(src.c_str());
		bool IsCovered = Cover2(imageData);
		if (IsCovered) {
			std::cout << "视频被遮挡 " << std::endl;
		}
		else {
			std::cout << "视频未被遮挡 " << std::endl;
		}
	}

};