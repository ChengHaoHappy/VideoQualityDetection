#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>  //OpenCVͷ�ļ�
using namespace std;
using namespace cv;

class Detection {
public:
	virtual int Detect(cv::Mat imageData) {
		cout << "test" << endl;
		return 0;
	}
};