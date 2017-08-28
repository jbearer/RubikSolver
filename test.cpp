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

using namespace CubeSolver;

int main()
{
    cout << "initializing color map" << endl;
    vector<vector<ColorMap>> colorMap = faceColorMap();

    cout << "reading scrambled images" << endl;

    cv::Mat img0, img1;
    img0 = cv::imread("TestImages/scrambledA0.png");
    img1 = cv::imread("TestImages/scrambledA1.png");

    // hold the captured BGR values
    vector<vector<cv::Scalar>> faceColors = getFaceColors(img0, img1, false);

    vector<vector<vector<Color>>> colorCandidates = getColorCandidates(faceColors, colorMap);

    Cube cube = translate(colorCandidates);

    vector<Turn> steps
    {
        Fi, Di, F, Bi, Ri, B, Di, Ui, R, Fi, F2, Ri,
        D2, L, B2, D2, B2, R, B2, L2, B2, U2, F2
    };

    for (auto t : steps) {
        cube = Cube::turn(cube, t);
    }

    assert(cube.isSolved());

    cout << "test passed" << endl;

}
