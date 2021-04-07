#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "VQA.cpp"
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;
using namespace cv;

static std::atomic_bool isOpen;

static void cameraThreadFunc(int camId, int height, int width, cv::Mat* pFrame)
{
    cv::VideoCapture capture(camId);
    capture.set(cv::CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    capture.set(cv::CAP_PROP_FPS, 30);

    if (!capture.isOpened()) {
        isOpen = false;
        std::cout << "Failed to open camera with index " << camId << std::endl;
    }
    Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),
        (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    VideoWriter writer("D:/test.mp4", CV_FOURCC('M', 'P', '4', 'V'), 30, S, true);
    cv::Mat frame;
    while (isOpen) {
        capture >> frame;
        cv::imshow("video", frame);
        frame.copyTo(*pFrame);
        cv::waitKey(5);
    }
    capture.release();
}

int main() {
    VideoCapture capture;
    capture.open("E:/VQA/DatabaseViewShake/00006_1.avi");
    if (!capture.isOpened()) {
        printf("could not read this video file...\n");
        return -1;
    }
    Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),
        (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    int fps = capture.get(CV_CAP_PROP_FPS);
    printf("current fps : %d \n", fps);
    //VideoWriter writer("D:/test.mp4", CV_FOURCC('D', 'I', 'V', 'X'), fps, S, true);

    Mat frame;
    namedWindow("camera-demo", CV_WINDOW_AUTOSIZE);

    VQA vqa;
    int count = 0;

    while (capture.read(frame)) {
        imshow("camera-demo", frame);
        count++;
        cout << count << endl;
        if (count == vqa.frame) {
            cout << "frame " << vqa.frame << endl;
            vqa.detectStart(frame);
            cout << "sum1 " << vqa.sum1 << endl;
            count = 0;
        }
        //writer.write(frame);
        char c = waitKey(50);
        if (c == 27) {
            break;
        }
    }
    capture.release();
    //writer.release();
    waitKey(0);

	/*VQA vqa;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        count++;
        if (count == vqa.frame) {
            cout << i << endl;
            vqa.detectStart("E:/VQA/DatabaseColorCast/t3.jpg");
            count = 0;
        }
    }*/
    return 0;
}




