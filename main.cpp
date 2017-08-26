/**
 * \file main.cpp
 * Uses the Cube class and the serial protocol to solve a scrambled cube and
 * send the solution to the serial port (where it should be received by an
 * Arduino).
 */

//#include "MotorControl/src/arduino-incl/comm-protocol.h"
//#include "SerialComm/src/serialstream.h"
//#include "Cube Solver 2/Cube Solver 2/cube.h"
#include <vector>
#include <iostream>
#include <limits>
#include <opencv.hpp>

#include "startup.hpp"
#include "colorFromTemplate.hpp"
#include "cubeSolver.h"

#include "StepperMotorControl/turnCube.hpp"
#include <unistd.h>
#include <stdio.h>
#include <exception>

using namespace CubeSolver;
using namespace cv;
//using namespace SerialComm;

using std::cout;
using std::endl;
using std::vector;

#define PORT0 0
#define PORT1 1

// int sortScalar(Scalar s)
// {
//     std::vector<int> place;
//     if(s[0] < s[1]){
//         if(s[0] < s[2]){
//             if(s[1] < s[2]) return 0;
//             else            return 1;
//         }
//         else return 4;
//     }
//     else{
//         if(s[0] > s[2]){
//             if(s[1] < s[2]) return 2;
//             else            return 3;
//         }
//         else{
//             return 5;
//         }
//     }
//     return -1;
// }

float distance(Scalar s1, Scalar s2)
{
    return cv::norm(s1, s2, NORM_L2);
}

vector<Color> closestColor(Scalar rgb, ColorMap colorMap)
{
    int threshold = 25;

    float min_dist = std::numeric_limits<float>::max();
    Color min_color;

    for (auto pair : colorMap) {
        float this_distance = distance(rgb, pair.second);
        // cout << "  " << pair.first << ": " << rgb << "," << pair.second << " = " << this_distance << endl;
        if (this_distance < min_dist) {
            min_dist = this_distance;
            min_color = pair.first;
        }
    }

    // find all the colors that are pretty close
    vector<Color> min_colors;
    for (auto pair : colorMap) {
        if (distance(rgb, pair.second) < min_dist + threshold) {
            min_colors.push_back(pair.first);
        }
    }

    // debug info
    if (min_colors.size() > 1) {
        cout << "  found " << min_colors.size() << " candidates" << endl;
    }

    return min_colors;
}

int main()
{
    cout << "\n---------------\nStarting Main\n---------------\n" << endl;

    readTurnTables();
    // initialize the cube solver

    std::cout << "creating color mappings" << std::endl;
    std::vector<std::vector<ColorMap>> colorMap = faceColorMap();

    std::cout << "initializing solver" << std::endl;
    Solver solver("ser/endMap_big");

    while(true){

        std::cout << "now scramble the cube, press enter when done" << std::endl;

        // wait for user input
        std::string input;
        std::cin >> input;
        cout << "Moving on" << endl;

        // TODO: Austin- take actual picture
        // Initialize video ports
        VideoCapture v0(PORT0);
        VideoCapture v1(PORT1);
        // Initialize matrices for current frame of videos for cameras
        Mat img0;
        Mat img1;
        // Open the port
        v0.open(PORT0);
        v1.open(PORT1);
        if(!v0.isOpened() || !v1.isOpened()){
            std::cout<<"ERROR ACQUIRING VIDEO FEED"<<std::endl;
            getchar();
            return -1;
        }
        cout << "Opened video ports successfully" << endl;

        // Take picture
        v0.read(img0);
        v1.read(img1);

        // auto startTime;

        // startTime = std::clock();
        // while( (std::clock()-startTime) / (double)CLOCKS_PER_SEC < 1.0){
        //     v0.read(img0);
        //     v1.read(img1);

        //     waitKey(1);
        // }


        imwrite("scrambled0.png", img0);
        imwrite("scrambled1.png", img1);
        cout << "Saved scambled pictures" << endl;


        // hold the captured BGR values
        std::vector<std::vector<cv::Scalar>> faceColors = getFaceColors(img0, img1, false);

        cout << "got face colors" << endl;
        // hold the actual color candidates
        vector<vector<vector<Color>>> cubeColors(6, vector<vector<Color>>(8, vector<Color>(1)));
        cout << "made cube colors" << endl;
        int tileSize = 60;
        Mat colorChoicePic(1*tileSize, 7*tileSize, CV_8UC3, Scalar(0,0,0));
        cout << "Init colorChoicePic" << endl;

        for (size_t face = 0; face < faceColors.size(); ++face) {
            for (size_t facelet = 0; facelet < 8; ++facelet) {
                // TODO: Austin sync EasyCube facelet order with getFaceColors facelet order
                vector<Color> close_colors = closestColor(faceColors[face][facelet], colorMap[face][facelet]);
                cubeColors[face][facelet] = close_colors;
                cout << "face " << face << " facelet " << facelet << " color ";
                for (auto color : close_colors) {
                    cout << color << " ";
                } cout << endl;
                // Print Pallet of "choices"
                rectangle(colorChoicePic,Point(0,0),Point(tileSize,tileSize),
                    faceColors[face][facelet], -1 );
                int c = 0;
                for (auto pair : colorMap[face][facelet]) {
                    rectangle(colorChoicePic,Point((c+1)*tileSize,0),Point((c+2)*tileSize, tileSize),
                        pair.second, -1 );
                    c++;
                }
                //imshow("Color Compare", colorChoicePic);
                // waitKey(500);

            }
        }
        Cube cube;
        try {
            cube = translate(cubeColors);
        }
        catch(...) {
            cout << "ERROR: could not translate to real cube, trying again" << endl;
            continue;
        }

        cout << "cube is" << endl;
        cout << cube << endl;

        // std::cout << "initializing solver" << std::endl;
        // Solver solver("ser/endMap_big");
        std::vector<Turn> turns = solver.solve(cube);

        // wait for user input
        std::cout << "Type SOLVE to solve the cube" << std::endl;
        std::cin >> input;


        // Setup motors
        pioInit(); // Initialize GPIOs
        setup(); // Setup pins
        digitalWrite(sleepPin,HIGH); // Wake up motors
        sleep(1);

        for (auto t : turns) {
            std::cout << t << " ";
            // TODO: actually turn the cube
            turn(t);
        }
        std::cout << std::endl;

        // Turn off motors
        digitalWrite(sleepPin,LOW);

        cout << "CONGRATULATIONS!!!!! THE CUBE IS SOLVED" << endl;
    }
    return 0;
}
