#include "HttpClient.cpp"
#include <assert.h>
#include<iostream>
#include "url_code.h"
using namespace std;



int mainTest() {
    HttpClient httpcleint;
    //httpcleint.getRequest("http://localhost/v1/reportAlarm", "/1", "/san", "/dong", 8077);
    //string src = "http://localhost:8077/v1/reportAlarm/1/二号机、科研楼二层东";
    string src = "/二号机/科研楼二层东";
    string dst;
    UrlEncode(src, dst);
    cout << dst << endl;
    return 0;
}