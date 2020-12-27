#include <iostream>
#include "ShakeDetection.h"       //抖动
#include "FreezeDetection.h"      //冻结
#include "ColorCastDetection.h"   //偏色
#include "BrightDetection.h"      //亮度
#include "SharpnessDetection.h"   //清晰度
#include "OcclusionDetection.h"   //遮挡

using namespace std;

void DetectionSchedule(const char* src, const char* base) {
	//ShakeDetection::ShakeDetectionStart(src, base);
	FreezeDetection::FreezeDetectionStart(src, base);
	//ColorCastDetection::ColorCastDetectionStart(src);
	//BrightDetection::BrightDetectionStart(src);
	//SharpnessDetection::SharpnessDetectionStart(src);
	//Occlusion::OcclusionDetectionStart(src);
}

int main() {
	//const char* src = "E:/VQA/DatabaseBright/5.bmp";   //正常
	const char* base = "E:/VQA/DatabaseViewShake/game/cam0/frame000.png"; //抖动
	const char* src = "E:/VQA/DatabaseViewShake/game/cam0/frame001.png";  //抖动
	//const char* src = "E:/VQA/DatabaseColorCast/t3.jpg";  //偏色
	//const char* src = "E:/VQA/DatabaseBright/1.png";   //偏暗
	//const char* src = "E:/VQA/DatabaseBright/2.png"; // 偏亮
	//const char* src = "E:/VQA/DatabaseSharpness/1.bmp";  //清晰度异常
	//const char* src = "E:/VQA/DatabaseOcclusion/1.png";  //遮挡
	DetectionSchedule(src, base);
	return 0;
}
