#include<iostream>
#include <opencv2/opencv.hpp>  
#include <math.h>��������
#include "Detection.h"

using namespace std;
using namespace cv;

const double EPS = 1e-6;

class LOSDetect : public Detection {
public:
	int getHistImage(const MatND& hist)
	{

		double maxValue = 0;
		double minValue = 0;
		double sum = 0;
		double loss = 0;
		for (int i = 0; i < hist.rows; i++)
		{
			float b = hist.at<float>(i);
			if (i < 5) {
				loss += b;
			}
			sum += b;

		}
		double loss_rate = loss / sum;
		if (loss_rate + EPS > 0.9) {
			cout << "�źŶ�ʧ�쳣" << endl;
			return 5;
		}
		return 0;
	}

	int Detect(Mat src)
	{
		//Mat src = imread(imagesrc.c_str(), 0);
		if (!src.data)
		{
			cout << "no picture!\n";
			return 5;
		}
		int a = src.channels();//a = 1ͨ��
		int image_count = 1;//Ҫ����ֱ��ͼ��ͼ��ĸ���
		int channels[1] = { 0 };//ͼ���ͨ��'

		Mat out;//��������ֱ��ͼ
		int dims = 1;//�õ�ֱ��ͼ��ά��
		int histsize[1] = { 256 };//ֱ��ͼ���������������
		float hrange[2] = { 0, 255 };//������ܷ�Χ
		const float* ranges[1] = { hrange };//ָ������

		calcHist(&src, image_count, channels, Mat(), out, dims, histsize, ranges); //����Ҷ�ͼ��ֱ��ͼ

		return getHistImage(out);
	}
};


