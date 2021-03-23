#include <iostream>
#include<cmath>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
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
		//����HSV��ͨ��
		vector<Mat> hsvChannels;
		split(image_hsv, hsvChannels);
		Mat image_H;
		image_H = hsvChannels[0];  //ȡɫ��
	                                  
		//Extending image ����ͼƬ
		int m = getOptimalDFTSize(image_H.rows); //���ظ��������ߴ羭��DFT�任������ųߴ��С
		int n = getOptimalDFTSize(image_H.cols);
		copyMakeBorder(image_H, image_H, 0, m - image_H.rows, 0, n - image_H.cols, BORDER_CONSTANT, Scalar(0));//��������ֵ

		//Fourier transform
		Mat mFourier(image_H.rows + m, image_H.cols + n, CV_32FC2, Scalar(0, 0));
		Mat mForFourier[] = { Mat_<float>(image_H), Mat::zeros(image_H.size(), CV_32F) };
		Mat mSrc;
		merge(mForFourier, 2, mSrc);  //�ϲ�ͨ��
		dft(mSrc, mFourier);  //����Ҷ�任

		//channels[0] is the real part of Fourier transform,channels[1] is the imaginary part of Fourier transform 
		vector<Mat> channels;
		split(mFourier, channels);
		Mat mRe = channels[0];   //ʵ��
		Mat mIm = channels[1];   //�鲿

		//Calculate the amplitude  �����άʸ���ķ�ֵ
		Mat mAmplitude;
		magnitude(mRe, mIm, mAmplitude);

		//Logarithmic scale
		mAmplitude += Scalar(1);     //��ֵ 1
		log(mAmplitude, mAmplitude); //ת���������߶�

		Mat means, stddev;
		meanStdDev(mAmplitude, means, stddev); //�����ֵ�ͱ�׼��
		double men = means.at<double>(0);
		double std = stddev.at<double>(0);
		double max_v, min_v;
		minMaxLoc(mAmplitude, &min_v, &max_v);
		//������ֵ
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
			//cout << "�����쳣"  << endl;
			cout << "�����쳣, ������=" << stripe_rate << endl;
		}
		/*else {
			cout << "��������, ������=" << stripe_rate << endl;
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
