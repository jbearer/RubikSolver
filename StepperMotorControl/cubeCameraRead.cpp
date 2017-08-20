/**
 * cubeCameraRead.cpp
 * 
 * Take a picture of the cube after moving a face
 */

#include <opencv.hpp>
#include <highgui.hpp> 
#include <unistd.h>
#include <stdio.h>
// #include <ncurses.h>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "turnCube.hpp"

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
    v1.open(PORT1);
    if(!v0.isOpened() || !v1.isOpened()){
        std::cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }

    // Read a single frame
    v0.read(cam0);
    v1.read(cam1);
    // Show pic
    imshow("Image0", cam0);
    imshow("Image1", cam1);

    // Setup motors
    pioInit();
    setup();
    digitalWrite(sleepPin,HIGH);    

    // // Setup ncurses
    // initscr();
    // timeout(0);

    waitKey(500);

    std::clock_t lastTime;
    std::clock_t curTime;
    int count = 0;

    // Turn cube and take pictures
    while(true){
        // Read a single frame
        v0.read(cam0);
        v1.read(cam1);
        // Show pic
        imshow("Image0", cam0);
        imshow("Image1", cam1);
        waitKey(5);
        
        // Step cube and take picture every 3 seconds
        curTime = std::clock();
        if(curTime - lastTime > 5.0){
            // Turn front face
            step(1,CW,1);
            sleep(1);

            // // Save pic
            // imwrite("a"+std::to_string(count)+".png", cam0);
            // imwrite("b"+std::to_string(count)+".png", cam1);

            lastTime = curTime;
            count++;
        }


        if(count == 8){
            break;
        }


    }

    // Turn off motors
    digitalWrite(sleepPin,LOW);

}