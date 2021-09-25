#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <mysql.h>
#include <list>
#include <ctime>
#include "Camera.h"
#include "VQA.cpp"
#include "HttpClient.cpp"

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

using namespace std;

const char user[] = "root";         
const char pswd[] = "123456";        
const char host[] = "localhost";    
const char table[] = "vqa";    
unsigned int port = 3306;
list<Camera> cameraList;

bool videoCaptureFun(Camera camera) {
    cv::VideoCapture capture;
    capture.open(camera.getIp());
	//long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);//��ȡ��Ƶ����֡��
	//cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;
	//��ȡ֡��  
	//double rate = capture.get(CV_CAP_PROP_FPS);
	//cout << "֡��Ϊ:" << rate << endl;
    if (!capture.isOpened()) {
        printf("could not read this video file...\n");
        return false;
    }
    cv::Mat frame;
    VQA vqa;
	//�����
    int count = 0;
	//�����쳣����
	int failureCount = 0;
	//�澯����
	int alarmCount = 0;
	time_t result1 = time(NULL);
	char startTime[26];
	//��ʼʱ��
	ctime_s(startTime, sizeof startTime, &result1);
	printf("startTime:%s\n", startTime);

	HttpClient httpcleint;

    while (capture.read(frame)) {
        //imshow("camera-demo", frame);
        count++;
		//printf("ID:%d,��%d֡\n", camera.getId(), count);
		//ÿ��count֡���һ��
        if (count == vqa.frame) {
			int flag = vqa.detectStart(frame);
			//�쳣
			if (flag != 0) {
				//printf("ID:%d,�����:%d\n", camera.getId(), vqa.frame);
				//printf("����ͷID:%d\n", camera.getId());
				string type = to_string(flag);
				string name = camera.getName();
				string location = camera.getLocation();
				cout<<"����ͷ���:"<< name <<endl;
				cout << "����ͷλ��:" << location << endl;
				time_t result = time(NULL);
				char str[26];
				ctime_s(str, sizeof str, &result);
				printf("�쳣����ʱ��:%s\n", str);
				failureCount++;
				if (failureCount == 3) {
					failureCount = 0;
					//�澯
					alarmCount++;
					cout << "����澯" << endl;
					//httpcleint.getRequest("http://localhost/v1/reportAlarm", type, name, location, 8077);
				}
				//�����澯���Σ����쳣�������־ŴΣ����ڼ�⣬ֹͣ�澯���ȴ�ά��
				if (alarmCount == 3) {
					cout << "gameover" << endl;
					break;
				}
			}
            //cout << "����������֡��:" << vqa.sum1 << endl;
            //cout << "�����쳣��֡��:" << vqa.sum << endl;
            count = 0;
        }
		//��30ms��ʾ��һ֡
		//waitKey(10);
    }
	time_t result2 = time(NULL);
	char endTime[26];
	ctime_s(endTime, sizeof endTime, &result2);
	printf("endTime:%s\n", endTime);
    capture.release();
    return true;
}

void connectDatabaseFun() {
	MYSQL myCont;
	MYSQL_RES* result = nullptr;
	MYSQL_ROW sql_row;
	MYSQL_FIELD* fd;
	char column[32][32];
	int res;
	mysql_init(&myCont);
	//auto ret = mysql_real_connect(&myCont, host, user, pswd, table, port, NULL, 0);
	if (mysql_real_connect(&myCont, host, user, pswd, table, port, NULL, 0))
	{
		cout << "connect succeed!" << endl;
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		res = mysql_query(&myCont, "select * from camera where is_delete = 0 and area_id = 1");//��ѯ    //database������Ӧ�ı���ܳɹ�
		if (!res)
		{
			result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if (result)
			{
				cout << "number of result: " << (unsigned long)mysql_num_rows(result) << endl;
				while (sql_row = mysql_fetch_row(result))//��ȡ���������
				{
					Camera camera(stoi(sql_row[0]), (string)sql_row[1], stoi(sql_row[2]), stoi(sql_row[3]), (string)sql_row[4], (string)sql_row[5],
						stoi(sql_row[6]), (string)sql_row[7], stoi(sql_row[8]), (string)sql_row[9], (string)sql_row[10]);
					cameraList.push_back(camera);
				}
			}
		}
		else
		{
			cout << "query sql failed!" << endl;
		}
	}
	else
	{
		cout << "connect failed!" << endl;
	}
	if (result != NULL) mysql_free_result(result);//�ͷŽ����Դ
	mysql_close(&myCont);//�Ͽ�����

}


int main() {
    connectDatabaseFun();
	int numthread = cameraList.size();
	thread *mythread = new thread[numthread];
	list<Camera>::iterator piter = cameraList.begin();
	for (int i = 0; i < numthread; i++)
	{
		mythread[i] = thread(videoCaptureFun, *piter);
		piter++;
	}
	for (int i = 0; i < numthread; i++)
	{	mythread[i].join();
	}
    //thread t1(videoCaptureFun,"E:/VQA/DatabaseViewShake/0073YC.avi");
    //t1.join();
   // thread t2(videoCaptureFun, "E:/VQA/DatabaseViewShake/00006_1.avi");
   // t2.join();
	
    return 0;
}





//�Ա�ʵ��

//˳����ͬһ���쳣
int main1() {
	const char filename[] = "E:/VQA/DatabaseBright/2.png";
	Mat frame = imread(filename);
	VQA vqa;
	time_t result1 = time(NULL);
	char startTime[26];
	ctime_s(startTime, sizeof startTime, &result1);
	printf("startTime:%s\n", startTime);
	for (int i = 0; i < 1000; i++) {
		//˳����ͬһ���쳣
		//vqa.orderDetect(frame);
		//�����㷨
		vqa.detectStart(frame);
	}
	time_t result2 = time(NULL);
	char endTime[26];
	ctime_s(endTime, sizeof endTime, &result2);
	printf("endTime:%s\n", endTime);
	return 0;
}
//˳��������쳣
int main2() {

	const char filename[] = "E:/VQA/DatabaseBright/2.png";
	const char filename2[] = "E:/VQA/DatabaseStripe/e6.jpg";
	const char filename3[] = "E:/VQA/DatabaseColorCast/t3.jpg";
	const char filename4[] = "E:/VQA/DatabaseSharpness/1.bmp";
	const char filename5[] = "E:/VQA/DatabaseOcclusion/1.png";
	const char filename6[] = "E:/VQA/DatabaseLOS/3.png";
	const char filename7[] = "E:/VQA/DatabaseContrast/1.png";

	//const char* src = "E:/VQA/DatabaseStripe/e6.jpg";  //����
	//const char* base = "E:/VQA/DatabaseBright/1.png";   //ƫ��
	//const char* src = "E:/VQA/DatabaseColorCast/t3.jpg";  //ƫɫ
	//const char* base = "E:/VQA/DatabaseSharpness/1.bmp";  //�������쳣
	//const char* base = "E:/VQA/DatabaseOcclusion/1.png";  //�ڵ�
	//const char* src = "E:/VQA/DatabaseLOS/3.png";  //�źŶ�ʧ
	//const char* base = "E:/VQA/DatabaseContrast/1.png"; // �Աȶ��쳣
	Mat frame = imread(filename);
	Mat frame2 = imread(filename2);
	Mat frame3 = imread(filename3);
	Mat frame4 = imread(filename4);
	Mat frame5 = imread(filename5);
	Mat frame6 = imread(filename6);
	Mat frame7 = imread(filename7);
	VQA vqa;
	time_t result1 = time(NULL);
	char startTime[26];
	ctime_s(startTime, sizeof startTime, &result1);
	printf("startTime:%s\n", startTime);
	for (int i = 0; i < 100; i++) {
		vqa.orderDetect(frame);
		vqa.orderDetect(frame2);
		vqa.orderDetect(frame3);
		vqa.orderDetect(frame4);
		vqa.orderDetect(frame5);
		vqa.orderDetect(frame6);
		vqa.orderDetect(frame7);
	}
	time_t result2 = time(NULL);
	char endTime[26];
	ctime_s(endTime, sizeof endTime, &result2);
	printf("endTime:%s\n", endTime);
	return 0;
}
//˳��������쳣
int main3() {

	const char filename[] = "E:/VQA/DatabaseBright/2.png";
	const char filename2[] = "E:/VQA/DatabaseStripe/e6.jpg";
	const char filename3[] = "E:/VQA/DatabaseColorCast/t3.jpg";
	const char filename4[] = "E:/VQA/DatabaseSharpness/1.bmp";
	const char filename5[] = "E:/VQA/DatabaseOcclusion/1.png";
	const char filename6[] = "E:/VQA/DatabaseLOS/3.png";
	const char filename7[] = "E:/VQA/DatabaseContrast/1.png";

	//const char* src = "E:/VQA/DatabaseStripe/e6.jpg";  //����
	//const char* base = "E:/VQA/DatabaseBright/1.png";   //ƫ��
	//const char* src = "E:/VQA/DatabaseColorCast/t3.jpg";  //ƫɫ
	//const char* base = "E:/VQA/DatabaseSharpness/1.bmp";  //�������쳣
	//const char* base = "E:/VQA/DatabaseOcclusion/1.png";  //�ڵ�
	//const char* src = "E:/VQA/DatabaseLOS/3.png";  //�źŶ�ʧ
	//const char* base = "E:/VQA/DatabaseContrast/1.png"; // �Աȶ��쳣
	Mat frame = imread(filename);
	Mat frame2 = imread(filename2);
	Mat frame3 = imread(filename3);
	Mat frame4 = imread(filename4);
	Mat frame5 = imread(filename5);
	Mat frame6 = imread(filename6);
	Mat frame7 = imread(filename7);
	VQA vqa;
	time_t result1 = time(NULL);
	char startTime[26];
	ctime_s(startTime, sizeof startTime, &result1);
	printf("startTime:%s\n", startTime);
	for (int i = 0; i < 1000000; i++) {
		vqa.detectStart(frame);
		vqa.detectStart(frame2);
		vqa.detectStart(frame3);
		vqa.detectStart(frame4);
		vqa.detectStart(frame5);
		vqa.detectStart(frame6);
		vqa.detectStart(frame7);
	}
	time_t result2 = time(NULL);
	char endTime[26];
	ctime_s(endTime, sizeof endTime, &result2);
	printf("endTime:%s\n", endTime);
	return 0;
}

//TODO����̬�������쳣
