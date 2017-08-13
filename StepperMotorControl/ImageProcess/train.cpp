// Compilation command
// g++ -g -O2 -std=c++11 -Wall -Wextra -pedantic train.cpp - -o train.o -I/usr/local/include/opencv2 -I/usr/local/include

#include <opencv.hpp>
#include <highgui.hpp>
#include <videoio.hpp>

using cv::VideoCapture;
using namespace cv;

int PORT1 = 1;              // Ports for the cameras
int PORT2 = 1;
std::string PATH = "";
size_t DELAY = 500;         // Delay in milliseconds

void snap(size_t n, VideoCapture v1/*, VideoCapture v2*/){

    Mat save_img1; v1 >> save_img1;
    Mat save_img2; v2 >> save_img2;

    imwrite(PATH + "a" + std::to_string(n) + ".png", save_img1);
    imwrite(PATH + "b" + std::to_string(n) + ".png", save_img2);
}


int main() {
    //train(3);
    //read();
    VideoCapture v1(PORT1);
    VideoCapture v2(PORT2);

    snap(3, v1, v2);

    // Funky test
    /*

    printf("Flushing camera\n");
    int i = 0;
    for(i = 0; i < 10; i++){
      printf("0.%d\n", i);
      delayMillis(500);
      snap(100+i, v1, v2);
      delayMillis(1500);
    }

    for(i = 0; i < 10; i++){
      printf("1.%d\n", i);
      delayMillis(100);
      snap(200+i, v1, v2);
      delayMillis(1500);
    }

    */
    return 0;
}
