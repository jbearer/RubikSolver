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

#include "turnCube.hpp"

using namespace CommProtocol;
using namespace CubeSolver;
using namespace cv;
//using namespace SerialComm;

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

    // initialize the cube solver
    std::cout << "initializing solver" << std::endl;
    Solver solver("CubeSolver/ser/endMap_big.ser");

    std::cout << "creating color mappings" << std::endl;
    std::vector<std::vector<ColorMap>> colorMap = faceColorMap();

    std::cout << "now scramble the cube, press enter when done" << std::endl;

    // wait for user input
    std::string input;
    std::cin >> input;

    // TODO: Austin- take actual picture
    Mat img0, img1;
    // hold the captured BGR values
    std::vector<std::vector<cv::Scalar>> faceColors = getFaceColors(img0, img1);
    // hold the actual colors
    std::vector<std::vector<Color>> cubeColors(6, std::vector<Color>(8));

    for (size_t face = 0; face < faceColors.size(); ++face) {
        for (size_t facelet = 0; facelet < 8; ++facelet) {
            // TODO: Austin sync EasyCube facelet order with getFaceColors facelet order
            cubeColors[face][facelet] = closestColor(faceColors[face][facelet], colorMap[face][facelet]);
        }
    }

    EasyCube easyCube(cubeColors);
    Cube cube = translate(easyCube);

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
    digitalWrite(sleepPon,LOW);

    return 0;
}
