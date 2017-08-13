// Compilation command
// g++ -g -O2 -std=c++11 -Wall -Wextra -pedantic train.cpp - -o train.o -I/usr/local/include/opencv2 -I/usr/local/include

#include <opencv.hpp>
#include <highgui.hpp>
#include <videoio.hpp>

using cv::VideoCapture;
using namespace cv;

int PORT1 = 1;              // Ports for the cameras
int PORT2 = 0;
std::string PATH = "";

void snap(size_t n, VideoCapture v1, VideoCapture v2){

    Mat save_img1; v1 >> save_img1;
    Mat save_img2; v2 >> save_img2;

    imwrite(PATH + "a" + std::to_string(n) + ".png", save_img1);
    imwrite(PATH + "b" + std::to_string(n) + ".png", save_img2);
    // printf("Saved img %d\n", n);
}
