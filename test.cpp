/*
 * Makeshift test to make sure I don't break anything
 * Tests creating the color mapping from a series of images,
 * reading in a new image of a scrambled cube, and constructing the
 * cube based on the color mappings.
 * Does NOT test cameras, motors, or solver (see CubeSolver/test)
 */

#include "startup.hpp"
#include "colorFromTemplate.hpp"
#include "cubeSolver.h"

#include <vector>
#include <iostream>
#include <cassert>

using std::vector;
using std::cout;
using std::endl;

using namespace cv;
using namespace CubeSolver;

int main()
{
    cout << "initializing color map" << endl;
    vector<vector<ColorMap>> colorMap = faceColorMap();

    cout << "reading scrambled images" << endl;

    Mat img0, img1;
    img0 = imread("TestImages/scrambledA0.png");
    img1 = imread("TestImages/scrambledA1.png");

    // hold the captured BGR values
    vector<vector<Scalar>> faceColors = getFaceColors(img0, img1, false);

    vector<vector<vector<Color>>> colorCandidates = getColorCandidates(faceColors, colorMap);

    Cube cube = translate(colorCandidates);

    vector<Turn> steps
    {
        FRONT_INVERTED, DOWN_INVERTED, FRONT, BACK_INVERTED, RIGHT_INVERTED, BACK,
        DOWN_INVERTED, UP_INVERTED, RIGHT, FRONT_INVERTED, FRONT_2, RIGHT_INVERTED,
        DOWN_2, LEFT, BACK_2, DOWN_2, BACK_2, RIGHT, BACK_2, LEFT_2, BACK_2, UP_2, FRONT_2
    };

    for (auto t : steps) {
        cube = Cube::turn(cube, t);
    }

    assert(cube.isSolved());

    cout << "test passed" << endl;

}
