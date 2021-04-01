#include<iostream>
#include "VQA.cpp"


int main() {
    VQA vqa;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        count++;
        if (count == vqa.frame) {
            cout << i << endl;
            vqa.detectStart("E:/VQA/DatabaseColorCast/t3.jpg");
            count = 0;
        }
    }
    cout << "over" << endl;
    return 0;
}