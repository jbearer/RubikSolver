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
    cout << "initializing color map" << endl;
    vector<vector<ColorMap>> colorMap = faceColorMap();

    cout << "reading scrambled images" << endl;

    Mat img0, img1;
    img0 = imread("../TestImages/scrambledA0.png");
    img1 = imread("../TestImages/scrambledA1.png");

    // hold the captured BGR values
    vector<vector<Scalar>> faceColors = getFaceColors(img0, img1, false);
    // hold the color candidates
    vector<vector<vector<Color>>> cubeColors(6, vector<vector<Color>>(8, vector<Color>(1)));

    cout << "filling out list of color candidates" << endl;

    for (size_t face = 0; face < faceColors.size(); ++face) {
        for (size_t facelet = 0; facelet < 8; ++facelet) {
            // TODO: Austin sync EasyCube facelet order with getFaceColors facelet order
            vector<Color> close_colors = closestColor(faceColors[face][facelet], colorMap[face][facelet]);
            cubeColors[face][facelet] = close_colors;
            cout << "face " << face << " facelet " << facelet << " color ";
            for (auto color : close_colors) {
                cout << color << " ";
            } cout << endl;

        }
    }

    Cube cube = translate(cubeColors);

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
