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
    if (!capture.isOpened()) {
        printf("could not read this video file...\n");
        return false;
    }
    cv::Mat frame;
    VQA vqa;
    int count = 0;
    while (capture.read(frame)) {
        //imshow("camera-demo", frame);
        count++;
		//printf("ID:%d,第%d帧\n", camera.getId(), count);
        if (count == vqa.frame) {
			printf("ID:%d,检测间隔:%d\n", camera.getId(), vqa.frame);
			cout<<"摄像头编号:"<< camera.getName()<<endl;
			cout << "摄像头位置:" << camera.getLocation() << endl;
            vqa.detectStart(frame);
			time_t result = time(NULL);
			char str[26];
			ctime_s(str, sizeof str, &result);
			printf("时间:%s\n", str);
            //cout << "连续正常的帧数:" << vqa.sum1 << endl;
            //cout << "连续异常的帧数:" << vqa.sum << endl;
            count = 0;
        }
        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();
    waitKey(0);
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
		mysql_query(&myCont, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文
		res = mysql_query(&myCont, "select * from camera");//查询    //database下有相应的表才能成功
		if (!res)
		{
			result = mysql_store_result(&myCont);//保存查询到的数据到result
			if (result)
			{
				cout << "number of result: " << (unsigned long)mysql_num_rows(result) << endl;
				while (sql_row = mysql_fetch_row(result))//获取具体的数据
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
	if (result != NULL) mysql_free_result(result);//释放结果资源
	mysql_close(&myCont);//断开连接

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
    //t2.join();
	
    return 0;
}





