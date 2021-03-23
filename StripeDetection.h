#include <iostream>
#include<cmath>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class StripeDetection {
public:
    static int StripeException(Mat mImage) {
		//e1:0.00895833  e2:   e3:0.00807774  e4:0.00710118  e5:0.00887214  e6:0.00887214
		//Mat mImage = imread("E:/VQA/DatabaseStripe/e6.jpg");
		if (mImage.data == 0)
		{
			cerr << "Image reading error" << endl;
			return -1;
		}

		Mat image_hsv;
		//bgr to hsv
		cvtColor(mImage, image_hsv, CV_BGR2HSV);
		//分离HSV三通道
		vector<Mat> hsvChannels;
		split(image_hsv, hsvChannels);
		Mat image_H;
		image_H = hsvChannels[0];  //取色度
	                                  
		//Extending image 扩充图片
		int m = getOptimalDFTSize(image_H.rows); //返回给定向量尺寸经过DFT变换后的最优尺寸大小
		int n = getOptimalDFTSize(image_H.cols);
		copyMakeBorder(image_H, image_H, 0, m - image_H.rows, 0, n - image_H.cols, BORDER_CONSTANT, Scalar(0));//扩充像素值

		//Fourier transform
		Mat mFourier(image_H.rows + m, image_H.cols + n, CV_32FC2, Scalar(0, 0));
		Mat mForFourier[] = { Mat_<float>(image_H), Mat::zeros(image_H.size(), CV_32F) };
		Mat mSrc;
		merge(mForFourier, 2, mSrc);  //合并通道
		dft(mSrc, mFourier);  //傅里叶变换

		//channels[0] is the real part of Fourier transform,channels[1] is the imaginary part of Fourier transform 
		vector<Mat> channels;
		split(mFourier, channels);
		Mat mRe = channels[0];   //实部
		Mat mIm = channels[1];   //虚部

		//Calculate the amplitude  计算二维矢量的幅值
		Mat mAmplitude;
		magnitude(mRe, mIm, mAmplitude);

		//Logarithmic scale
		mAmplitude += Scalar(1);     //赋值 1
		log(mAmplitude, mAmplitude); //转换到对数尺度

		Mat means, stddev;
		meanStdDev(mAmplitude, means, stddev); //计算均值和标准差
		double men = means.at<double>(0);
		double std = stddev.at<double>(0);
		double max_v, min_v;
		minMaxLoc(mAmplitude, &min_v, &max_v);
		//计算阈值
		double T = max(men + 3 * std, max_v / 2);
		double count = 0;
		int height = mAmplitude.rows;
		int width = mAmplitude.cols;
		//cout << mAmplitude << endl;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				//cout << mAmplitude.at<float>(i, j) <<endl;
				if (mAmplitude.at<float>(i, j) > T) {
					count++;
				}
			}
		}
		//printf("count = %d\n", count);
		double stripe_rate = count / (height * width);
		if (stripe_rate > 0.008) {
			//cout << "条纹异常"  << endl;
			cout << "条纹异常, 条纹率=" << stripe_rate << endl;
		}
		/*else {
			cout << "条纹正常, 条纹率=" << stripe_rate << endl;
		}*/
		return  0;
    }

	static void StripeExceptionStart(std::string src) {

		cv::Mat srcImage = cv::imread(src.c_str());
		if (!srcImage.data)
		{
			cout << "no picture!\n";
			exit(1);
		}
        StripeException(srcImage);

	}
};
