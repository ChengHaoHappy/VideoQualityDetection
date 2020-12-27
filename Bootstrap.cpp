#include <iostream>
#include "ShakeDetection.h"       //����
#include "FreezeDetection.h"      //����
#include "ColorCastDetection.h"   //ƫɫ
#include "BrightDetection.h"      //����
#include "SharpnessDetection.h"   //������
#include "OcclusionDetection.h"   //�ڵ�

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
	//const char* src = "E:/VQA/DatabaseBright/5.bmp";   //����
	const char* base = "E:/VQA/DatabaseViewShake/game/cam0/frame000.png"; //����
	const char* src = "E:/VQA/DatabaseViewShake/game/cam0/frame001.png";  //����
	//const char* src = "E:/VQA/DatabaseColorCast/t3.jpg";  //ƫɫ
	//const char* src = "E:/VQA/DatabaseBright/1.png";   //ƫ��
	//const char* src = "E:/VQA/DatabaseBright/2.png"; // ƫ��
	//const char* src = "E:/VQA/DatabaseSharpness/1.bmp";  //�������쳣
	//const char* src = "E:/VQA/DatabaseOcclusion/1.png";  //�ڵ�
	DetectionSchedule(src, base);
	return 0;
}
