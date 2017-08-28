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
#include <tuple>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "turnCube.hpp"

#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

using namespace CubeSolver;
using namespace cv;

using std::cout;
using std::endl;
using std::vector;

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
    {Li, Ri, Di, Ui, Bi, Fi, F2, R, L, B2, U2, Li, Li, D2, F2, U2, Ri, Ri},
    {F2, B2, D2, B2, F2, U2, Li, Li, B2, F2, Ri, Ri},
    {Li, Ri, Bi, Fi, Di, Ui, R, L, F2, D2, F2, L, R, U2, F2, L, R, F2},
    {Li, Ri, Bi, Fi, Di, Ui, R, R, D2, F2, D2, Ri, Ri, D2, F2, L, R, F2},
    {F2, B2, D2, B2, F2, U2, Li, Li, B2, F2, Ri, Ri},
    {Li, Ri, Di, Ui, Bi, Fi, B2, R, L, F2, U2, Li, Li, U2, F2, D2, Ri, Ri},
    {Li, Ri, Di, Ui, Bi, Fi, F2, R, L, B2, U2, Li, Li, D2, F2, U2, Ri, Ri},
    {Li, Ri, Di, Ui, Bi, Fi, F2, R, L, F2, U2, Ri, Ri, U2, F2, U2, Ri, Ri},
    {Li, Ri, Di, Ui, Bi, Fi, F2, R, L, F2, U2, Ri, Ri, U2, F2, U2, Ri, Ri}
};

void getPallet(std::vector<std::vector<ColorMap>> colorMaps)
{
    int tileSize = 60;

    Mat avgColorPic1(8*tileSize,6*tileSize, CV_8UC3, Scalar(0,0,0));

    for(int row = 0; row < 6; row++){
        for(int col = 0; col < 8; col++){
            std::cout << "color map for row " << row << " col " << col << std::endl;
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

void archivePatternPictures(const std::vector<std::tuple<Mat,Mat>>& pictures)
{
    for (size_t pic = 0; pic < pictures.size(); ++pic) {
        const Mat& img0 = std::get<0>(pictures[pic]);
        const Mat& img1 = std::get<1>(pictures[pic]);
        imwrite("img0_" + std::to_string(pic) + ".png", img0);
        imwrite("img0_" + std::to_string(pic) + ".png", img1);
    }
}

std::vector<std::tuple<Mat,Mat>> patternPictures()
{
    vector<std::tuple<Mat,Mat>> pictures;

    // set up video capture
    VideoCapture v0(PORT0);
    VideoCapture v1(PORT1);

    v0.open(PORT0);
    v1.open(PORT1);

    if(!v0.isOpened() || !v1.isOpened())
        throw std::runtime_error("ERROR: acquiring video feed");

    // set up motors
    pioInit();
    setup();
    digitalWrite(sleepPin,HIGH);

    for (size_t i = 0; i < turnPatterns.size(); ++i) {

        Mat img0;
        Mat img1;

        const std::vector<Turn>& turns = turnPatterns[i];
        for (auto t : turns) {
            turn(t);
        }

        std::cout << "taking picture " << i << std::endl;

        // Wait 1 second for feed to buffer
        std::clock_t startTime = std::clock();
        while( (std::clock()-startTime) / (double)CLOCKS_PER_SEC < 1.0){
            v0.read(img0);
            v1.read(img1);

            waitKey(1);
        }

        pictures.push_back(std::make_tuple(img0,img1));
    }
    archivePatternPictures(pictures);
    digitalWrite(sleepPin, LOW);

    return pictures;
}

std::vector<std::vector<ColorMap>> faceColorMap()
{
    size_t numFaces = 6;
    size_t numFacelets = 8;

    // assume the cube starts in the solved position
    // i.e. Up corresponds with Yellow, Front with Green, etc.
    std::vector<std::vector<ColorMap>> colorMaps(numFaces, std::vector<ColorMap>(numFacelets));

    for (size_t i = 0; i < turnPatterns.size(); ++i){

        Mat img0 = imread("TestImages/img0_"+std::to_string(i)+".png");
        Mat img1 = imread("TestImages/img1_"+std::to_string(i)+".png");

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

    return colorMaps;
}

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

vector<vector<vector<Color>>> getColorCandidates(const vector<vector<Scalar>>& faceColors, const vector<vector<ColorMap>>& colorMap)
{
    // hold the color candidates
    vector<vector<vector<Color>>> cubeColors(6, vector<vector<Color>>(8));

    cout << "filling out list of color candidates" << endl;

    for (size_t face = 0; face < faceColors.size(); ++face) {
        for (size_t facelet = 0; facelet < 8; ++facelet) {
            vector<Color> close_colors = closestColor(faceColors[face][facelet], colorMap[face][facelet]);
            cubeColors[face][facelet] = close_colors;
            cout << "face " << face << " facelet " << facelet << " color ";
            for (auto color : close_colors) {
                cout << color << " ";
            } cout << endl;

        }
    }
    return cubeColors;
}


// TODO: serialize the color maps




