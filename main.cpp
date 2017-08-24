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

using namespace CommProtocol;
using namespace CubeSolver;
using namespace cv;
//using namespace SerialComm;

using std::cout;
using std::endl;

#define PORT0 0
#define PORT1 1

float distance(Scalar s1, Scalar s2)
{
    return cv::norm(s1 - s2);
}

Color closestColor(Scalar rgb, ColorMap colorMap)
{
    float min_dist = std::numeric_limits<float>::max();
    Color min_color;

    for (auto pair : colorMap) {
        float this_distance = distance(rgb, pair.second);
        if (this_distance < min_dist) {
            min_dist = this_distance;
            min_color = pair.first;
        }
    }

    return min_color;
}

int main()
{
    readTurnTables();
    // initialize the cube solver

    std::cout << "creating color mappings" << std::endl;
    std::vector<std::vector<ColorMap>> colorMap = faceColorMap();

    std::cout << "now scramble the cube, press enter when done" << std::endl;

    // wait for user input
    std::string input;
    std::cin >> input;

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
        std::cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }

    // Take picture
    v0.read(img0);
    v1.read(img1);

    imwrite("scrambled0.png", img0);
    imwrite("scrambled1.png", img1);

    // hold the captured BGR values
    std::vector<std::vector<cv::Scalar>> faceColors = getFaceColors(img0, img1);
    // hold the actual colors
    std::vector<std::vector<Color>> cubeColors(6, std::vector<Color>(8));

    for (size_t face = 0; face < faceColors.size(); ++face) {
        for (size_t facelet = 0; facelet < 8; ++facelet) {
            // TODO: Austin sync EasyCube facelet order with getFaceColors facelet order
            Color color = closestColor(faceColors[face][facelet], colorMap[face][facelet]);
            cubeColors[face][facelet] = color;
            cout << "face " << face << " facelet " << facelet << " color " << color << endl;
        }
    }

    EasyCube easyCube(cubeColors);
    Cube cube = translate(easyCube);

    cout << "cube is" << endl;
    cout << cube << endl;

    std::cout << "initializing solver" << std::endl;
    Solver solver("ser/endMap_big");
    std::vector<Turn> turns = solver.solve(cube);

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

    return 0;
}
