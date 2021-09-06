#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <opencv2/highgui/highgui_c.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "Detection.h"

using namespace std;
using namespace cv;

/**
https://github.com/verusx/ColorCastDetection
*/

class ColorCastDetect : public Detection {
public:
     double CastDetection(IplImage* src)
	{
		IplImage* lab = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
		cvCvtColor(src, lab, CV_BGR2Lab);
		IplImage* l = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		IplImage* a = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		IplImage* b = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
		IplImage* fa = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
		IplImage* fb = cvCreateImage(cvGetSize(src), IPL_DEPTH_32F, 1);
		cvSplit(lab, l, a, b, NULL);
		cvConvertScale(a, fa, 1.0, -128);
		cvConvertScale(b, fb, 1.0, -128);
		CvScalar aMean = cvAvg(fa);
		CvScalar bMean = cvAvg(fb);
		double D = sqrt(aMean.val[0] * aMean.val[0] + bMean.val[0] * bMean.val[0]);
		cvConvertScale(fa, fa, 1.0, -aMean.val[0]);
		cvConvertScale(fb, fb, 1.0, -bMean.val[0]);
		cvPow(fa, fa, 2.0);
		cvPow(fb, fb, 2.0);
		CvScalar aSquareMean = cvAvg(fa);
		CvScalar bSquareMean = cvAvg(fb);
		double M = sqrt(aSquareMean.val[0] + bSquareMean.val[0]);
		double K = D / M;
		cvReleaseImage(&lab);
		cvReleaseImage(&l);
		cvReleaseImage(&a);
		cvReleaseImage(&b);
		cvReleaseImage(&fa);
		cvReleaseImage(&fb);
		return K;
	}

	int Detect(Mat mat) {
		//const char* src = path.data();
		//IplImage* image = cvLoadImage(src, 1);
		IplImage image = cvIplImage(mat);	
		if (&image == 0)
		{
			cout << "Image reading error" << endl;
			return 2;
		}
		double K = CastDetection(&image);
		if (K > 1.4)
		{
			cout << "偏色异常" << endl;
			return 2;
		}
		return 0;

	}
};