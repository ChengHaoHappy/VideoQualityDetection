/*
* 视频偏色异常检测
*/
#include <iostream>
#include <opencv2/opencv.hpp>  //OpenCV头文件
#include <opencv2/highgui/highgui_c.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

class ColorCastDetection {
public:
	ColorCastDetection() {}

	static double CastDetection(IplImage* src)
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

	static void ColorCorrect(IplImage* src, IplImage* dst)
	{
		float u, v;
		double srcMax, srcMin, dstMax, dstMin, squareMax, squareMin;
		CvScalar cvSrcSum, cvDstSum, cvSquareSum;
		IplImage* square = cvCreateImage(cvGetSize(dst), IPL_DEPTH_32F, 1);

		cvPow(dst, square, 2.0);

		cvSrcSum = cvSum(src);
		cvDstSum = cvSum(dst);
		cvSquareSum = cvSum(square);

		cvMinMaxLoc(src, &srcMin, &srcMax);

		cvMinMaxLoc(dst, &dstMin, &dstMax);
		cvMinMaxLoc(square, &squareMin, &squareMax);

		u = (srcMax * cvDstSum.val[0] - dstMax * cvSrcSum.val[0]) / (squareMax * cvDstSum.val[0] - dstMax * cvSquareSum.val[0]);
		v = (squareMax * cvSrcSum.val[0] - srcMax * cvSquareSum.val[0]) / (squareMax * cvDstSum.val[0] - dstMax * cvSquareSum.val[0]);

		cvConvertScale(square, square, u);
		cvConvertScale(dst, dst, v);
		cvAdd(dst, square, dst);
		cvReleaseImage(&square);
	}

	static void AdjustCast(IplImage* img, IplImage* dst)
	{

		float ur, vr, ub, vb;
		CvScalar gSum, rSum, bSum, rSqrSum, bSqrSum;
		double rMax, rMin, rSqrMax, rSqrMin, gMax,
			gMin, bMax, bMin, bSqrMax, bSqrMin;
		IplImage* fImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 3);
		IplImage* fRed = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
		IplImage* fGreen = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);
		IplImage* fBlue = cvCreateImage(cvGetSize(img), IPL_DEPTH_32F, 1);

		cvConvertScale(img, fImg, 1.0 / 255, 0);
		cvSplit(fImg, fBlue, fGreen, fRed, NULL);

		ColorCorrect(fRed, fGreen);
		ColorCorrect(fRed, fBlue);
		cvThreshold(fGreen, fGreen, 1.0, 1.0, CV_THRESH_TRUNC);
		cvThreshold(fBlue, fBlue, 1.0, 1.0, CV_THRESH_TRUNC);
		cvMerge(fBlue, fGreen, fRed, NULL, fImg);

		cvConvertScale(fImg, dst, 255);

		cvReleaseImage(&fImg);
		cvReleaseImage(&fRed);
		cvReleaseImage(&fGreen);
		cvReleaseImage(&fBlue);

	}
	/*static void RepairCast(const char* src)
	{
		IplImage* image = cvLoadImage(src, 1);
		IplImage* dst = cvCloneImage(image);
		double K = CastDetection(image);
		//printf("偏色检测K值：%lf\n", K);
		if (K > 1.4)
		{
			printf("偏色异常, 偏色检测值K=%lf\n", K);
			AdjustCast(image, dst);
		}
		cvNamedWindow("src", 1);
		cvNamedWindow("dst", 1);
		cvShowImage("src", image);
		cvShowImage("dst", dst);
		cvWaitKey(0);
		cvReleaseImage(&dst);
	}*/

	static void ColorCastDetectionStart(const char* src) {
		IplImage* image = cvLoadImage(src, 1);
		//cv::Mat image = cv::imread(src.c_str());

		if (image == 0)
		{
			cerr << "Image reading error" << endl;
			system("pause");
			return ;
		}
		
	
		double K = CastDetection(image);
		if (K > 1.4)
		{
			cout << "偏色异常" << endl;
			//printf("偏色异常, K=%lf\n", K);
		}
		/*else {
			printf("偏色正常, K=%lf\n", K);
		}*/

	}

	/********************************************************************************
*函数描述： ColorfulNess 计算并返回一幅图像的颜色丰富度
*函数参数： frame  彩色帧图
*函数返回值：double 颜色丰富度表示值
*********************************************************************************/
	double ColorfulNess(cv::Mat frame)
	{
		// split image to 3 channels (B,G,R)
		cv::Mat channelsBGR[3];
		cv::split(frame, channelsBGR);

		// rg = R - G
		// yb = 0.5*(R + G) - B
		cv::Mat rg, yb;
		cv::absdiff(channelsBGR[2], channelsBGR[1], rg);
		cv::absdiff(0.5 * (channelsBGR[2] + channelsBGR[1]), channelsBGR[0], yb);

		// calculate the mean and std for rg and yb
		cv::Mat rgMean, rgStd; // 1*1矩阵
		meanStdDev(rg, rgMean, rgStd);
		cv::Mat ybMean, ybStd; // 1*1矩阵
		meanStdDev(yb, ybMean, ybStd);

		// calculate the mean and std for rgyb
		double stdRoot, meanRoot;
		stdRoot = sqrt(pow(rgStd.at<double>(0, 0), 2)
			+ pow(ybStd.at<double>(0, 0), 2));
		meanRoot = sqrt(pow(rgMean.at<double>(0, 0), 2)
			+ pow(ybMean.at<double>(0, 0), 2));

		// return colorfulNess
		return stdRoot + (0.3 * meanRoot);
	}
};

