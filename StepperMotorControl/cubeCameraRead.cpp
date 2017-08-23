/**
 * cubeCameraRead.cpp
 * 
 * Take a picture of the cube after moving a face
 */

#include <opencv.hpp>
#include <highgui.hpp> 
#include <unistd.h>
#include <stdio.h>


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

    // Wait a sec for to buffer
    waitKey(1000);
    printf("Waited 1 seconds\n");
    sleep(1);

    std::clock_t lastTime, curTime;
    int count = 0;
    bool takePic = false;

    // Turn cube and take pictures
    while(true){
        // Read a single frame
        v0.read(cam0);
        v1.read(cam1);
        // Show pic
        imshow("Image0", cam0);
        imshow("Image1", cam1);
        waitKey(1);
        
        
        // Step cube and take picture every 3 seconds
        curTime = std::clock();
        printf("%f\n", (curTime-lastTime) / (double) CLOCKS_PER_SEC);

        // Snap picture right before making another move
        if ((curTime-lastTime)/(double)CLOCKS_PER_SEC > 0.9 && takePic){
            imwrite("a"+std::to_string(count)+".png", cam0);
            imwrite("b"+std::to_string(count)+".png", cam1);
            takePic = false;

            printf("COUNT: %d\n", count);
            count++;
        }
        
        // Move face every 1 second
        if( (curTime - lastTime) /(double) CLOCKS_PER_SEC > 1.0){
            // Turn front face
            step(rand()%6,CW,1);

            // printf("%f\n", (curTime - lastTime)/ (double) CLOCKS_PER_SEC);
            lastTime = curTime;
            
            takePic = true;
        }




        if(count == 50){
            break;
        }


    }

    // Turn off motors
    digitalWrite(sleepPin,LOW);

}