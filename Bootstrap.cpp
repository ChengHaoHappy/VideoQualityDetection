
#include<iostream>
#include "ShakeDetection.h"       //����
#include "FreezeDetection.h"      //����
#include "ColorCastDetection.h"   //ƫɫ��K > 1.4
#include "BrightDetection.h"      //����  brightcast > 1
#include "SharpnessDetection.h"   //������  DR < 10
#include "OcclusionDetection.h"   //�ڵ�   CoverBlock > 1
#include "StripeDetection.h"      //����   stripe_rate > 0.006
#include "ContrastDetection.h"    //�Աȶ� brightcast > 1
#include "LOSDetection.h"         //�źŶ�ʧ  loss_rate > 0.9
#include "NoiseDetection.h"       //����   ����ָ��


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
	int sum = 0;//ͬһ���쳣�쳣�������ֵĴ���
	int sum1 = 0;
	int tag = 0;  //ÿһ�ַ����ı�־λ
	bool failure;
	for (int i = 0; i < 10; i++) {
		//ѭ�����
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
				//��������
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

	//const char* src = "E:/VQA/DatabaseBright/5.bmp";   //����
	//const char* base = "E:/VQA/DatabaseBright/5.bmp";   //����

	//const char* base = "E:/VQA/DatabaseViewShake/game/cam0/frame088.png"; //����
	//const char* src = "E:/VQA/DatabaseViewShake/game/cam0/frame199.png";  //����

	//const char* src = "E:/VQA/DatabaseColorCast/t3.jpg";  //ƫɫ
	//const char* base = "E:/VQA/DatabaseColorCast/t3.jpg";  //ƫɫ

	//const char* base = "E:/VQA/DatabaseBright/1.png";   //ƫ��
	//const char* src = "E:/VQA/DatabaseBright/1.png";   //ƫ��

	//const char* base = "E:/VQA/DatabaseBright/2.png"; // ƫ��
	//const char* src = "E:/VQA/DatabaseBright/2.png"; // ƫ��

	//const char* base = "E:/VQA/DatabaseSharpness/1.bmp";  //�������쳣
	//const char* src = "E:/VQA/DatabaseSharpness/1.bmp";  //�������쳣

	//const char* base = "E:/VQA/DatabaseOcclusion/1.png";  //�ڵ�
	//const char* src = "E:/VQA/DatabaseOcclusion/1.png";  //�ڵ�

	//const char* src = "E:/VQA/DatabaseStripe/e6.jpg";  //����
	//const char* base = "E:/VQA/DatabaseStripe/e6.jpg";  //����

	//const char* base = "E:/VQA/DatabaseContrast/1.png"; // �Աȶ��쳣
	//const char* src = "E:/VQA/DatabaseContrast/1.png"; // �Աȶ��쳣

	const char* src = "E:/VQA/DatabaseLOS/3.png";  //�źŶ�ʧ
	const char* base = "E:/VQA/DatabaseLOS/3.png";  //�źŶ�ʧ

	//const char* src = "E:/VQA/DatabaseNoise/Databaserelease2/wn/img4.bmp";  //����
	//const char* base = "E:/VQA/DatabaseNoise/Databaserelease2/wn/img4.bmp";  //����
	DetectionSchedule(src, base);

	return 0;
}

