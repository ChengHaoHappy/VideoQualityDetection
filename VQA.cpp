#include<iostream>
#include <list>
#include "Action.h"
#include <opencv2/opencv.hpp>  //OpenCV头文件

using namespace std;

class VQA {
public:
	int sum = 0;
	int sum1 = 0;
	int frame = 50;
	int flag = 0;
	list<Detection*> queue;
public:
	VQA() {

		queue.push_back(new BrightDetect());
		queue.push_back(new ColorCastDetect());
		queue.push_back(new ContrastDetect());
		//queue.push_back(new FreezeDetect());
		queue.push_back(new LOSDetect());
		queue.push_back(new NoiseDetect());
		queue.push_back(new OcclusionDetect());
		//queue.push_back(new ShakeDetect());
		queue.push_back(new SharpnessDetect());
		queue.push_back(new StripeDetect());
	}


	bool detectStart(cv::Mat src) {
		list<Detection*>::iterator piter = queue.begin();
		for (; piter != queue.end(); ++piter) {
			int x = (*piter)->Detect(src);
			if (x > 0) {
				if (flag == x) {
					sum++;
				}
				else {
					flag = x;
					sum = 1;
					sum1 = 0;
				}

				if (sum >= 3) {
					sum = 0;
					if (frame > 1) {
						frame /= 2;
						//调整队列
						if (*piter != queue.front()) {
							Detection* temp = (*piter);
							piter = queue.erase(piter);
							queue.push_front(temp);

						}
					}
				}
				return false;
			}
		}
		flag = 0;
		sum = 0;
		sum1++;
		if (sum1 >= 3) {
			frame *= 2;
			sum1 = 0;
		}
		return true;
	}
};
