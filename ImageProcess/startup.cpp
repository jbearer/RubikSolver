/*
 * At start up, create a map of average color values from each location
 *
 */
#include "colorFromTemplate.hpp"
#include "startup.hpp"
#include "../CubeSolver/src/cubeSolver.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "turnCube.hpp"

#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

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

/*
L' R' D' U' B' F' F2 R L B2 U2 L' L' D2 F2 U2 R' R'
F2 B2 D2 B2 F2 U2 L' L' B2 F2 R' R'
L' R' B' F' D' U' R L F2 D2 F2 L R U2 F2 L R F2
L' R' B' F' D' U' R R D2 F2 D2 R' R' D2 F2 L R F2
F2 B2 D2 B2 F2 U2 L' L' B2 F2 R' R'
L' R' D' U' B' F' B2 R L F2 U2 L' L' U2 F2 D2 R' R'
L' R' D' U' B' F' F2 R L B2 U2 L' L' D2 F2 U2 R' R'
L' R' D' U' B' F' F2 R L F2 U2 R' R' U2 F2 U2 R' R'
L' R' D' U' B' F' F2 R L F2 U2 R' R' U2 F2 U2 R' R'
 */

static const std::vector<std::vector<Turn>> turnPatterns
{
    {LEFT_INVERTED, RIGHT_INVERTED, DOWN_INVERTED, UP_INVERTED, BACK_INVERTED, FRONT_INVERTED, FRONT_2, RIGHT, LEFT, BACK_2, UP_2, LEFT_INVERTED, LEFT_INVERTED, DOWN_2, FRONT_2, UP_2, RIGHT_INVERTED, RIGHT_INVERTED},
    {FRONT_2, BACK_2, DOWN_2, BACK_2, FRONT_2, UP_2, LEFT_INVERTED, LEFT_INVERTED, BACK_2, FRONT_2, RIGHT_INVERTED, RIGHT_INVERTED},
    {LEFT_INVERTED, RIGHT_INVERTED, BACK_INVERTED, FRONT_INVERTED, DOWN_INVERTED, UP_INVERTED, RIGHT, LEFT, FRONT_2, DOWN_2, FRONT_2, LEFT, RIGHT, UP_2, FRONT_2, LEFT, RIGHT, FRONT_2},
    {LEFT_INVERTED, RIGHT_INVERTED, BACK_INVERTED, FRONT_INVERTED, DOWN_INVERTED, UP_INVERTED, RIGHT, RIGHT, DOWN_2, FRONT_2, DOWN_2, RIGHT_INVERTED, RIGHT_INVERTED, DOWN_2, FRONT_2, LEFT, RIGHT, FRONT_2},
    {FRONT_2, BACK_2, DOWN_2, BACK_2, FRONT_2, UP_2, LEFT_INVERTED, LEFT_INVERTED, BACK_2, FRONT_2, RIGHT_INVERTED, RIGHT_INVERTED},
    {LEFT_INVERTED, RIGHT_INVERTED, DOWN_INVERTED, UP_INVERTED, BACK_INVERTED, FRONT_INVERTED, BACK_2, RIGHT, LEFT, FRONT_2, UP_2, LEFT_INVERTED, LEFT_INVERTED, UP_2, FRONT_2, DOWN_2, RIGHT_INVERTED, RIGHT_INVERTED},
    {LEFT_INVERTED, RIGHT_INVERTED, DOWN_INVERTED, UP_INVERTED, BACK_INVERTED, FRONT_INVERTED, FRONT_2, RIGHT, LEFT, BACK_2, UP_2, LEFT_INVERTED, LEFT_INVERTED, DOWN_2, FRONT_2, UP_2, RIGHT_INVERTED, RIGHT_INVERTED},
    {LEFT_INVERTED, RIGHT_INVERTED, DOWN_INVERTED, UP_INVERTED, BACK_INVERTED, FRONT_INVERTED, FRONT_2, RIGHT, LEFT, FRONT_2, UP_2, RIGHT_INVERTED, RIGHT_INVERTED, UP_2, FRONT_2, UP_2, RIGHT_INVERTED, RIGHT_INVERTED},
    {LEFT_INVERTED, RIGHT_INVERTED, DOWN_INVERTED, UP_INVERTED, BACK_INVERTED, FRONT_INVERTED, FRONT_2, RIGHT, LEFT, FRONT_2, UP_2, RIGHT_INVERTED, RIGHT_INVERTED, UP_2, FRONT_2, UP_2, RIGHT_INVERTED, RIGHT_INVERTED}
};

void getPallet(std::vector<std::vector<ColorMap>> colorMaps)
{
    int tileSize = 60;

    Mat avgColorPic1(8*tileSize,6*tileSize, CV_8UC3, Scalar(0,0,0));

    for(int row = 0; row < 6; row++){
        for(int col = 0; col < 8; col++){
            const ColorMap& cm = colorMaps[row][col];
            int offset = 0;
            for (auto pair : cm) {
                Scalar avgColor = pair.second;
                rectangle(avgColorPic1, Point(offset*tileSize,col*tileSize), Point((offset+1)*tileSize,(col+1)*tileSize), avgColor, -1);
                offset += 1;
            }
        }
        imwrite("avgColorPic" + std::to_string(row) + ".png", avgColorPic1);
    }

    //imshow("avgColorPic1", avgColorPic1);
    //imwrite("avgImg1.jpg",avgColorPic1);
}

// Get instructions to turn cube such that each color is visible at least
// once from each facelet
std::vector<std::vector<Turn>> colorTour()
{
    Cube start;
    std::vector<std::vector<Turn>> allTurns;

    Solver solver("ser/endMap_big");

    // create a vector of all the cubes used in the startup phase
    std::vector<Cube> patternCubes;
    for (auto& v : colorPatterns) {
        patternCubes.push_back(makeCube(v));
    }

    // figure out the steps to get from one cube to the next
    for (auto c : patternCubes) {
        std::vector<Turn> result = solver.solveToCube(start, c);
        allTurns.push_back(result);
        start = c;
    }

    for (auto& algorithm : allTurns) {
        for (auto t : algorithm) {
            std::cout << t;
        } std::cout << std::endl;
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
        //std::cout<<"ERROR ACQUIRING VIDEO FEED\n";
        throw std::exception();
        // std::cout<<"ERROR ACQUIRING VIDEO FEED\n";
        // getchar();
        // return -1;
    }


    // Initialize master mask
    v0.read(img0);
    v1.read(img1);
    Mat mask0(img0.rows, img0.cols, CV_8UC3, Scalar(0,0,0));
    Mat mask1(img1.rows, img1.cols, CV_8UC3, Scalar(0,0,0));
    getMasterMask0(mask0);
    getMasterMask1(mask1);
    Mat result0;
    Mat result1;



    // Setup motors
    pioInit();
    setup();
    digitalWrite(sleepPin,HIGH);

    std::clock_t startTime;

    // assume the cube starts in the solved position
    // i.e. Up corresponds with Yellow, Front with Green, etc.
    std::vector<std::vector<ColorMap>> colorMaps(numFaces, std::vector<ColorMap>(numFacelets));

    //std::vector<std::vector<Turn>> turnPatterns = colorTour();

    for (size_t i = 0; i < turnPatterns.size(); ++i){
        // const std::vector<Turn>& turns = turnPatterns[i];
        // for (auto t : turns) {
        //     turn(t);
        // }

        // // std::cout << "Fix cube, then enter input to continue: ";
        // // std::string input;
        // // std::cin >> input;
        // //


        // std::cout <<"taking picture " << i << std::endl;

        // // // Ncurses screen
        // // int ncurseChar = 0;
        // // initscr();
        // // timeout(0);

        // // Wait 1 second for feed to buffer
        // startTime = std::clock();
        // while( (std::clock()-startTime) / (double)CLOCKS_PER_SEC < 1.0){
        // // while(!ncurseChar){
        //     v0.read(img0);
        //     v1.read(img1);

        //     // bitwise_or(img0, mask0, result0); // bitwise_or to keep rest of img
        //     // imshow("Full Mask 0", result0);
        //     // bitwise_or(img1, mask1, result1); // bitwise_or to keep rest of img
        //     // imshow("Full Mask 1", result1);

        //     // ncurseChar=getch();
        //     // if(ncurseChar>0) ncurseChar = 1;
        //     // else ncurseChar = 0;

        //     waitKey(1);
        // }

        // imwrite("img0_"+std::to_string(i)+".png", img0);
        // imwrite("img1_"+std::to_string(i)+".png", img1);

        img0 = imread("img0_"+std::to_string(i)+".png");
        img1 = imread("img1_"+std::to_string(i)+".png");

        // endwin();

        // Mat img0, img1;
        std::vector<std::vector<Scalar>> faceColors = getFaceColors(img0, img1, false);

        std::cout << "loading map with colors " << i << std::endl;

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
    std::cout << "getting pallet" << std::endl;
    getPallet(colorMaps);

    digitalWrite(sleepPin, LOW);
    return colorMaps;
}

// TODO: serialize the color maps




