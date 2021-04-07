
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include "time.h"
#include "Detection.h"

using namespace std;
using namespace cv;

Mat my_convmxt_h(Mat H);
Mat my_convmxt_v(Mat H);
Mat my_im2col(Mat im, Mat patchsize);
Mat my_matchange(Mat A, Mat B);
class NoiseDetect : public Detection {
public:
	void Matout(Mat A)    //输出矩阵Mat
	{
		for (int i = 0; i < A.rows; i++)
		{
			for (int j = 0; j < A.cols; j++)
				cout << A.at<double>(i, j) << " ";
			cout << endl;
		}
		cout << endl;
	}
	Mat my_im2col(Mat im, Mat patchsize)
	{
		int ma = im.rows;
		int na = im.cols;
		int m = patchsize.at<int>(0, 0);
		int n = patchsize.at<int>(0, 1);
		int mc = m;
		int nc = ma - m + 1;
		int nn = na - n + 1;
		Mat cidx = Mat(mc, nc, CV_64F);
		Mat ridx = Mat(mc, nc, CV_64F);
		Mat t = Mat(mc, nc, CV_64F);
		Mat vv = Mat(mc, nc, CV_64F);
		//ofstream imgdec_out("temp.txt", ios_base::out);
		for (int i = 0; i < nc; i++)
		{
			for (int j = 0; j < mc; j++)
			{
				cidx.at<double>(j, i) = j;
			}
		}
		//Matout(cidx);

		for (int i = 0; i < mc; i++)
		{
			for (int j = 0; j < nc; j++)
			{
				ridx.at<double>(i, j) = j + 1;
			}
		}
		//Matout(ridx);

		t = cidx + ridx;
		//t = t - 1;
		//Matout(t);

		Mat hh = Mat(mc, nc, CV_64F);
		Mat tt = Mat::zeros(mc * n, nc, CV_64F);
		int aa, bb;
		for (int i = 0; i < n; i++)
		{
			hh = t + ma * i;
			//Matout(hh);
			aa = i * mc;
			bb = i * mc + mc;
			hh.copyTo(tt.rowRange(aa, bb));
		}
		//Matout(tt);
		Mat ttt = Mat::zeros(mc * n, nc * nn, CV_64F);
		Mat gg = Mat(mc * n, nc, CV_64F);
		int cc, dd;
		for (int j = 0; j < nn; j++)
		{
			gg = tt + ma * j;
			//Matout(gg);
			cc = j * nc;
			dd = j * nc + nc;
			gg.copyTo(ttt.colRange(cc, dd));
		}


		Mat CC = my_matchange(im, ttt);
		//Mat DD;
		//CC.colRange(0, 5).copyTo(DD);
		//Matout(DD);

		return CC;
	}
	Mat my_matchange(Mat A, Mat B)
	{
		int m1 = A.rows;
		int m2 = A.cols;
		int n1 = B.rows;
		int n2 = B.cols;
		int temp;
		int p, q;
		Mat C = Mat(n1, n2, CV_64F);

		double* C3_data = (double*)C.data;
		double* A_data = (double*)A.data;
		double* B_data = (double*)B.data;
		//double* data2 = (double*)C.data;
		for (int k = 0; k < n1 * n2; k++)
		{
			temp = (int)B_data[k] - 1;
			C3_data[k] = A_data[temp];
		}

		Mat C3(n1, n2, CV_64F, C3_data);

		return C;
	}
	Mat my_convmxt_h(Mat H)
	{
		int m = 7;
		int n = 7;
		int s1 = H.rows;
		int s2 = H.cols;
		int II = m - s1 + 1;
		int JJ = n - s2 + 1;
		int Tw = II * JJ;
		int Th = m * n;
		Mat T = Mat::zeros(Tw, Th, CV_64F);
		int k = 1;
		int t;
		//int hh[3][1];

		for (int i = 1; i < II + 1; i++)
		{
			for (int j = 1; j < JJ + 1; j++)
			{
				for (int p = 1; p < s2 + 1; p++)
				{
					t = (i - 1) * n + (j - 1) + p;
					//hh[p-1][0] = t;
					T.at<double>(k - 1, t - 1) = H.at<double>(0, p - 1);
				}
				k = k + 1;
			}

		}
		return T;
	}
	Mat my_convmxt_v(Mat H)
	{
		int m = 7;
		int n = 7;
		int s1 = H.rows;
		int s2 = H.cols;
		int II = m - s1 + 1;
		int JJ = n - s2 + 1;
		int Tw = II * JJ;
		int Th = m * n;
		Mat T = Mat::zeros(Tw, Th, CV_64F);
		int k = 1;
		int h;

		for (int i = 1; i < II + 1; i++)
		{
			for (int j = 1; j < JJ + 1; j++)
			{
				for (int p = 1; p < s1 + 1; p++)
				{
					//for (int q = 1; p < s2 + 1; p++)
					//{
					h = (i - 1 + p - 1) * n + (j - 1) + 1 + s2 - 1;
					double w = H.at<double>(p - 1, 0);
					T.at<double>(k - 1, h - 1) = w;
					//}
				}
				k = k + 1;
			}

		}
		return T;
	}
	int Detect(Mat img) {

		//cv::Mat img = cv::imread(src.c_str());
		if (!img.data)
		{
			cout << "no picture!\n";
			return 6;
		}
		//Mat img = imread("E:/VQA/DatabaseNoise/Databaserelease2/wn/img1.bmp");
		//Mat img = imread("E:/VQA/DatabaseColorCast/t3.jpg");
		resize(img, img, Size(), 0.3, 0.3);
		//namedWindow("img", WINDOW_AUTOSIZE);//创建窗口
		//imshow("img", img);
		cvtColor(img, img, CV_BGR2GRAY);
		img.convertTo(img, CV_64F);
		Mat img2 = img / 255;
		//namedWindow("img2", WINDOW_AUTOSIZE);//创建窗口
		//imshow("img2", img2);
		//cvtColor(img, img, CV_BGR2GRAY);
		double start0 = clock();  //开始时间

		Mat imgh0, imgv0;
		Mat imgh1, imgv1;
		Mat imgh, imgv;
		Mat imgh2, imgv2;

		Mat kh = (Mat_<double>(1, 3) << -0.5, 0, 0.5);
		filter2D(img, imgh0, img.depth(), kh);
		imgh0.colRange(1, imgh0.cols - 1).copyTo(imgh1);

		imgh = imgh1.mul(imgh1);
		imgh2 = imgh / 255;
		//namedWindow("imgh", WINDOW_AUTOSIZE);//创建窗口
		//imshow("imgh", imgh2);

		Mat kv = (Mat_<double>(3, 1) << -0.5,
			0,
			0.5);
		filter2D(img, imgv0, img.depth(), kv);
		imgv0.rowRange(1, imgv0.rows - 1).copyTo(imgv1);

		imgv = imgv1.mul(imgv1);
		imgv2 = imgv / 255;
		//namedWindow("imgv", WINDOW_AUTOSIZE);//创建窗口
		//imshow("imgv", imgv2);

		Mat Dh = my_convmxt_h(kh);
		Mat Dh_t = Dh.t();
		Mat DHT = Dh_t * Dh;
		//Matout(DHT);

		Mat Dv = my_convmxt_v(kv);
		Mat Dv_t = Dv.t();
		Mat DVT = Dv_t * Dv;
		//Matout(DVT);

		Mat DD = DHT + DVT;
		//Matout(DD);
		Scalar scalar;
		scalar = trace(DD);
		double  Dtr = scalar.val[0];
		int r = 45;//要计算的
		//int Dtr = 35;//要计算的
		double tau0 = 81.8208;//要计算的

		Mat patchsize = (Mat_<int>(1, 2) << 7, 7);
		Mat patchsize_h = (Mat_<int>(1, 2) << 7, 5);
		Mat patchsize_v = (Mat_<int>(1, 2) << 5, 7);
		Mat X, Xh, Xv;

		X = my_im2col(img, patchsize);
		Xh = my_im2col(imgh, patchsize_h);
		Xv = my_im2col(imgv, patchsize_v);

		Mat Xt = Mat(Xh.rows * 2, Xh.cols, CV_64F);
		Xh.copyTo(Xt.rowRange(0, Xt.rows / 2));
		Xv.copyTo(Xt.rowRange(Xt.rows / 2, Xt.rows));

		Mat Xtr = Mat(1, Xt.cols, CV_64F);
		reduce(Xt, Xtr, 0, CV_REDUCE_SUM);


		Mat cov = X * X.t() / (X.cols - 1);
		//Matout(cov);
		Mat eValuesMat, eVectorsMat;
		eigen(cov, eValuesMat, eVectorsMat);


		double sig2;
		sig2 = eValuesMat.at<double>(eValuesMat.rows - 1, 0);
		//cout << sig2 << endl;
		double tau;
		Mat cov2;
		Mat eVal2, eVec2;
		Mat Xtr2 = Xtr.clone();
		Mat X2 = X.clone();
		for (int k = 0; k < 1; k++)
		{
			tau = sig2 * tau0;

			Mat Xtrout;

			Mat Xout = Mat(size(X2), CV_64F) /*= X.clone()*/;
			double gg;
			Mat hh;
			int w = 0;
			for (int i = 0; i < Xtr2.cols; i++)
			{
				gg = Xtr2.at<double>(0, i);
				hh = X2.col(i);
				if (gg < tau)
				{
					w = w + 1;
					Xtrout.push_back(gg);

					hh.copyTo(Xout.colRange(w - 1, w));
				}
			}
			Xtrout = Xtrout.t();
			Xout = Xout.colRange(0, w);

			cov2 = Xout * Xout.t() / (Xout.cols - 1);
			eigen(cov2, eVal2, eVec2);
			//Matout(eVal2);

			sig2 = eVal2.at<double>(eVal2.rows - 1, 0);
			//cout << "噪声指数= " << (int)sig2 << endl;
			if (sig2 > 200) {
				cout << "噪声异常" << " sig=" << sig2 << endl;
				return 6;
			}

			Xtr2 = Xtrout.clone();
			X2 = Xout.clone();
		}
		//waitKey();
		return 0;
	}



};
