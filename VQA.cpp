#include<iostream>
#include <list>
#include "Action.h"
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
using namespace std;

class VQA {
public:
	//ͬ���쳣�������ֵĴ���
	int sum = 0;
	//���������Ĵ���
	int sum1 = 0;
	// ��ʼ���
	int frame = 50;
	//�쳣���ͱ�־Ϊ
	int flag = 0;
	list<Detection*> queue;
public:
	VQA() {

		
		queue.push_back(new ColorCastDetect());
		//queue.push_back(new FreezeDetect());
		queue.push_back(new LOSDetect());
		queue.push_back(new NoiseDetect());
		queue.push_back(new OcclusionDetect());
		queue.push_back(new BrightDetect());
		//queue.push_back(new ShakeDetect());
		queue.push_back(new SharpnessDetect());
		queue.push_back(new StripeDetect());
		queue.push_back(new ContrastDetect());
	}


	int detectStart(cv::Mat src) {
		list<Detection*>::iterator piter = queue.begin();
		//�쳣������
		for (; piter != queue.end(); ++piter) {
			//�쳣����
			int x = (*piter)->Detect(src);
			//�쳣
			if (x > 0) {
				sum1 = 0;
				if (flag == x) {
					sum++;
				}
				else {
					flag = x;
					sum = 1;
				}
				//ͬһ���쳣������������
				if (sum >= 3) {
					sum = 0;
					if (frame > 1) {
						//�����/2
						frame /= 2;
						//���쳣����������ͷ��
						if (*piter != queue.front()) {
							Detection* temp = (*piter);
							piter = queue.erase(piter);
							queue.push_front(temp);

						}
					}
				}
				return x;
			}
		}
		//���쳣
		flag = 0;
		//�����쳣����0
		sum = 0;
		//������������1
		sum1++;
		//�����������������3
		if (sum1 >= 3) {
			//�����*2
			frame *= 2;
			//��������������Ϊ0
			sum1 = 0;
		}
		return 0;
	}

	void orderDetect(cv::Mat src) {
		list<Detection*>::iterator piter = queue.begin();
		for (; piter != queue.end(); ++piter) {
			//�쳣����
			int x = (*piter)->Detect(src);
		}
	}
};
