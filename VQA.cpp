#include<iostream>
#include <list>
#include "Action.h"
#include <opencv2/opencv.hpp>  //OpenCV头文件
using namespace std;

class VQA {
public:
	//同种异常连续出现的次数
	int sum = 0;
	//连续正常的次数
	int sum1 = 0;
	// 初始间隔
	int frame = 50;
	//异常类型标志为
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
		//异常检测队列
		for (; piter != queue.end(); ++piter) {
			//异常类型
			int x = (*piter)->Detect(src);
			//异常
			if (x > 0) {
				sum1 = 0;
				if (flag == x) {
					sum++;
				}
				else {
					flag = x;
					sum = 1;
				}
				//同一种异常连续出现三次
				if (sum >= 3) {
					sum = 0;
					if (frame > 1) {
						//检测间隔/2
						frame /= 2;
						//该异常调整到队列头部
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
		//无异常
		flag = 0;
		//连续异常数置0
		sum = 0;
		//连续正常数加1
		sum1++;
		//如果连续正常数等于3
		if (sum1 >= 3) {
			//检测间隔*2
			frame *= 2;
			//连续正常数重置为0
			sum1 = 0;
		}
		return 0;
	}

	void orderDetect(cv::Mat src) {
		list<Detection*>::iterator piter = queue.begin();
		for (; piter != queue.end(); ++piter) {
			//异常类型
			int x = (*piter)->Detect(src);
		}
	}
};
