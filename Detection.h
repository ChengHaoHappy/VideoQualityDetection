#pragma once
#include <iostream>
#include <string>

using namespace std;

class Detection {
public:
	virtual int Detect(string path) {
		cout << "test" << endl;
		return 0;
	}
};