/*
 * At start up, create a map of average color values from each location
 *
 */
#include "colorFromTemplate.hpp"
#include "startup.hpp"
#include "../CubeSolver/src/cubeSolver.h"
#include <iostream>
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "turnCube.hpp"

using namespace CommProtocol;
using namespace CubeSolver;
using namespace cv;

#define PORT0 0
#define PORT1 1

Cube makeCube(std::vector<Color> colors) {
    EasyCube c(
        std::vector<Color>(8, colors[0]),
        std::vector<Color>(8, colors[1]),
        std::vector<Color>(8, colors[2]),
        std::vector<Color>(8, colors[3]),
        std::vector<Color>(8, colors[4]),
        std::vector<Color>(8, colors[5])
        );
    return translate(c);
}

static const std::vector<std::vector<Color>> colorPatterns
{
    {Red, Yellow, Blue, Green, White, Orange},
    {Orange, Yellow, Green, Blue, White, Red},
    {Yellow, Green, Orange, Red, Blue, White},
    {Green, Red, White, Yellow, Orange, Blue},
    {Blue, Red, Yellow, White, Orange, Green},
    {White, Green, Red, Orange, Blue, Yellow},
    {Red, White, Green, Blue, Yellow, Orange},
    {Green, Orange, Yellow, White, Red, Blue},
    {Yellow, Blue, Red, Orange, Green, White}
};

// Get instructions to turn cube such that each color is visible at least
// once from each facelet
std::vector<std::vector<Turn>> colorTour()
{
    Cube start;
    std::vector<std::vector<Turn>> allTurns;

    Solver solver("ser/endMap_big");

    std::vector<Cube> patternCubes;
    for (auto& v : colorPatterns) {
        patternCubes.push_back(makeCube(v));
    }

    for (auto c : patternCubes) {
        std::vector<Turn> result = solver.solveToCube(start, c);
        allTurns.push_back(result);
        start = c;
    }
    return allTurns;
}

std::vector<std::vector<ColorMap>> faceColorMap()
{
    size_t numFaces = 6;
    size_t numFacelets = 8;
    

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

    // Setup motors
    pioInit();
    setup();
    digitalWrite(sleepPin,HIGH);   

    std::clock_t startTime; 

    // assume the cube starts in the solved position
    // i.e. Up corresponds with Yellow, Front with Green, etc.
    std::vector<std::vector<ColorMap>> colorMaps(numFaces, std::vector<ColorMap>(numFacelets));

    std::vector<std::vector<Turn>> turnsVec = colorTour();

    for (size_t i = 0; i < turnsVec.size(); ++i){
        const std::vector<Turn>& turns = turnsVec[i];
        for (auto t : turns) {
            turn(t);
        }
        // Wait 1 second for feed to buffer
        startTime = std::clock();
        while( (std::clock()-startTime) / (double)CLOCKS_PER_SEC > 1.0){
            v0.read(img0);
            v1.read(img1);
            waitKey(1);
        }

        // Mat img0, img1;
        std::vector<std::vector<Scalar>> faceColors = getFaceColors(img0, img1);

        // load the maps with colors
        for (size_t face = 0; face < numFaces; ++face) {

            Color faceColor = colorPatterns[i][face];

            for (size_t facelet = 0; facelet < numFacelets; ++facelet) {

                ColorMap& cm = colorMaps[face][facelet];

                // NOTE: assume that each color is seen either once or twice
                // per facelet.  If seen twice, then the map stores the average
                if (cm.count(faceColor) == 0) {
                    cm[faceColor] = faceColors[face][facelet];
                }
                else {
                    Scalar oldValue = cm[faceColor];
                    cm[faceColor] = (faceColors[face][facelet] + oldValue)/2;
                }

            }
        }
    }

    digitalWrite(sleepPin, LOW);
    return colorMaps;
}

// TODO: serialize the color maps




