
#include<iostream>
#include "ShakeDetection.h"       //抖动
#include "FreezeDetection.h"      //冻结
#include "ColorCastDetection.h"   //偏色，K > 1.4
#include "BrightDetection.h"      //亮度  brightcast > 1
#include "SharpnessDetection.h"   //清晰度  DR < 10
#include "OcclusionDetection.h"   //遮挡   CoverBlock > 1
#include "StripeDetection.h"      //条纹   stripe_rate > 0.006
#include "ContrastDetection.h"    //对比度 brightcast > 1
#include "LOSDetection.h"         //信号丢失  loss_rate > 0.9
#include "NoiseDetection.h"       //噪声   噪声指数


using namespace std;

void DetectionSchedule(const char* src, const char* base) {
	//ShakeDetection::ShakeDetectionStart(src, base);
	//FreezeDetection::FreezeDetectionStart(src, base);
	ColorCastDetection::ColorCastDetectionStart(src);
	BrightDetection::BrightDetectionStart(src);
	SharpnessDetection::SharpnessDetectionStart(src);
	Occlusion::OcclusionDetectionStart(src);
	StripeDetection::StripeExceptionStart(src);
	ContrastDetection::ContrastDetectionStart(src);
	LOSDetection::LOSDetectionStart(src);
	NoiseDetection().NoiseDetectionStart(src);
	int td;
	int sum = 0;//同一种异常异常连续出现的次数
	int sum1 = 0;
	int tag = 0;  //每一种方法的标志位
	bool failure;
	for (int i = 0; i < 10; i++) {
		//循环检测
		if (failure && tag == i) {
			sum1 = 0;
			if (tag == i) {
				sum++;
			}
			else {
				sum = 1;
				tag = i;
			}
			if (sum > 3) {
				td = td / 2;
				//调整队列
			}
		}
		else {
			sum = 0;
			sum1++;
			if (sum1 > 3) {
				td = td * 2;
			}
		}
		//
	}
}

int main() {

	//const char* src = "E:/VQA/DatabaseBright/5.bmp";   //正常
	//const char* base = "E:/VQA/DatabaseBright/5.bmp";   //正常

	//const char* base = "E:/VQA/DatabaseViewShake/game/cam0/frame088.png"; //抖动
	//const char* src = "E:/VQA/DatabaseViewShake/game/cam0/frame199.png";  //抖动

	//const char* src = "E:/VQA/DatabaseColorCast/t3.jpg";  //偏色
	//const char* base = "E:/VQA/DatabaseColorCast/t3.jpg";  //偏色

	//const char* base = "E:/VQA/DatabaseBright/1.png";   //偏暗
	//const char* src = "E:/VQA/DatabaseBright/1.png";   //偏暗

	//const char* base = "E:/VQA/DatabaseBright/2.png"; // 偏亮
	//const char* src = "E:/VQA/DatabaseBright/2.png"; // 偏亮

	//const char* base = "E:/VQA/DatabaseSharpness/1.bmp";  //清晰度异常
	//const char* src = "E:/VQA/DatabaseSharpness/1.bmp";  //清晰度异常

	//const char* base = "E:/VQA/DatabaseOcclusion/1.png";  //遮挡
	//const char* src = "E:/VQA/DatabaseOcclusion/1.png";  //遮挡

	//const char* src = "E:/VQA/DatabaseStripe/e6.jpg";  //条纹
	//const char* base = "E:/VQA/DatabaseStripe/e6.jpg";  //条纹

	//const char* base = "E:/VQA/DatabaseContrast/1.png"; // 对比度异常
	//const char* src = "E:/VQA/DatabaseContrast/1.png"; // 对比度异常

	const char* src = "E:/VQA/DatabaseLOS/3.png";  //信号丢失
	const char* base = "E:/VQA/DatabaseLOS/3.png";  //信号丢失

	//const char* src = "E:/VQA/DatabaseNoise/Databaserelease2/wn/img4.bmp";  //噪声
	//const char* base = "E:/VQA/DatabaseNoise/Databaserelease2/wn/img4.bmp";  //噪声
	DetectionSchedule(src, base);

	return 0;
}

