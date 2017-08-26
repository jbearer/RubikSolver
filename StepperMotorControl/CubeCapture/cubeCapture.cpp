/**
 * cubeCapture.cpp
 * 
 * Display webcam (to allow focus), and snap and save frame when user 
 * input on keyboard
 * 
 */

#include <opencv.hpp>
#include <highgui.hpp> 
#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

using namespace cv;

int PORT0 = 0;              // Ports for the cameras
int PORT1 = 1;

std::string PATH = "";

int main(void)
{
    // Initialize video ports
    VideoCapture v0(PORT0);
    VideoCapture v1(PORT1);

    // Initialize matrices for current frame of videos for cameras
    Mat cam0;
    Mat cam1;

    // Open the port
    v0.open(PORT0);
    if(!v0.isOpened()){
        std::cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }
    v1.open(PORT1);
    if(!v1.isOpened()){
        std::cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }

    int i = 0;
    initscr();
    timeout(0);

    // double exposure;

    sleep(1);
    while(!i){
    // while(true){
        // Read a single frame
        v0.read(cam0);
        v1.read(cam1);

        imshow("Image0", cam0);
        imshow("Image1", cam1);

        // // v0.set(CV_CAP_PROP_AUTO_EXPOSURE, 0 ); 
        // exposure = v0.get(CV_CAP_PROP_EXPOSURE); 
        // std::cout << "Exposure: " << exposure << std::endl;

        i=getch();
        // std::cout << i << std::endl;
        if(i>0) i = 1;
        else i = 0;
// 
        waitKey(5);
    }

    endwin();

    imwrite("template0.jpg", cam0);
    imwrite("template1.jpg", cam1);

}