#include<iostream>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
#include <math.h>��������
using namespace std;
using namespace cv;
const double EPS = 1e-6;

class LOSDetection {
public:
	static void getHistImage(const MatND& hist)
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
		//	cout << "�źŶ�ʧ�쳣, ��ʧ��=" << loss_rate << endl;
		}
		/*else {
			cout << "�ź�����, ��ʧ��=" << loss_rate << endl;
		}*/
	}

	static void LOSDetectionStart(std::string imagesrc)
	{
		Mat src = imread(imagesrc.c_str(),0);
		if (!src.data)
		{
			cout << "no picture!\n";
			exit(1);
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

		getHistImage(out);
	}
};


