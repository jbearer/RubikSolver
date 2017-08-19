/**
 * colorFromTemplate.cpp
 *
 * Experiment with getting avergae color from a pre-captured image
 *
 */

#include <opencv.hpp>
#include <highgui.hpp>
#include <unistd.h>
#include <stdio.h>
#include "faceCoords.hpp" // defines face pixel coordinates
#include <string>
#include <vector>
#include "../CubeSolver/src/cube.h"
#include "../CubeSolver/src/translate.h"

using namespace cv;
using namespace std;
using namespace CubeSolver;

//enum Color {RED, GREEN, BLUE, ORANGE, YELLOW, BLACK, NONE};

/**
 * @brief      print operator for Color enum
 *
 * @return     output stream
 */
/*
std::ostream& operator<<(std::ostream& out, const Color c){
    const char* s = 0;
    #define PROCESS_VAL(p) case(p): s = #p; break;
    switch(c){
        PROCESS_VAL(RED);
        PROCESS_VAL(GREEN);
        PROCESS_VAL(BLUE);
        PROCESS_VAL(YELLOW);
        PROCESS_VAL(ORANGE);
        PROCESS_VAL(BLACK);
        PROCESS_VAL(NONE);
    }
    #undef PROCESS_VAL
    return out << s;
}
*/
/**
 * @brief      parse string input to Color enum
 *
 * @param[in]  s     string obtained from cin
 *
 * @return     Color enum
 */
Color parseColor(string s){
    string s2 = "";
    for(char& c : s){
        s2 += toupper(c);
    }
    cout << "s2: " << s2 << endl;

    if(s2 == "R") return Red;
    else if(s2 == "G") return Green;
    else if(s2 == "Y") return Yellow;
    else if(s2 == "O") return Orange;
    else if(s2 == "BL") return Blue;
    else return White;
    // else{
    //     cerr << "ERROR: Unknown color imput. Only acceptable values are: R,O,Y,G,BL,BK" << endl;
    //     return NONE;
    // }
}

/**
 * @brief      Calculate the avergae color given a predefined quadrilateral using a
 *             mask method. Also displays the mask image
 *
 * @param[in]  src     src image
 * @param[in]  points  coordinates of the corners (quadrilateral only)
 *
 * @return     Scalar average B,G,R,alpha value
 */
Scalar averageColor(const Mat src, const int points[4][2])
{
    // Organize coordinates as point array
    cv::Point corners[1][4];
    corners[0][0] = Point(points[0][0],points[0][1]);
    corners[0][1] = Point(points[1][0],points[1][1]);
    corners[0][2] = Point(points[2][0],points[2][1]);
    corners[0][3] = Point(points[3][0],points[3][1]);
    const Point* corner_list[1] = {corners[0]};

    // Create the mask
    int num_points = 4;
    Mat mask(src.rows, src.cols, CV_8U, Scalar(0,0,0));
    fillPoly(mask, corner_list, &num_points, 1, Scalar(255,255,255));
    // Calculate the average color
    Scalar avg = mean(src, mask);
    cout<<avg<<endl;

    // Showing the mask (an outline version, and true masked version)
    Mat maskPic0(src.rows, src.cols, CV_8UC3, Scalar(0,0,0)); // outline version
    polylines(maskPic0, corner_list, &num_points, 1, true, Scalar(255,255,255), 2, 8);
    Mat result0;
    bitwise_or(src, maskPic0, result0); // bitwise_or to keep rest of img
    imshow("masked0",result0);

    Mat maskPic1(src.rows, src.cols, CV_8UC3, Scalar(0,0,0)); // true mask version
    fillPoly(maskPic1, corner_list, &num_points, 1, Scalar(255,255,255));
    Mat result1;
    bitwise_and(src, maskPic1, result1); // bitwise_and to mask the image
    imshow("masked1",result1);

    waitKey(1);

    return avg;
}

/**
 * @brief      Loops through face coordinates, calculates the average color,
 *             and asks for user input to map between BGR vlau and user defined
 *             color.
 *
 * @param[in]  src0  camera0 (holed)
 * @param[in]  src1  camera1 (stand)
 */
void findFaceColors(const Mat src0, const Mat src1)
{

    int tileSize = 60; // size of tiles when showing the pallete

    // Image 0
    Scalar faceAvgColors0 [27];
    Mat avgColorPic0(3*tileSize,9*tileSize, CV_8UC3, Scalar(0,0,0));

    Color faceColors [27];

    // Loop through each facele
    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 9; col++){
            // Calculate average color of facelet
            faceAvgColors0[row*9+col] = averageColor(src0, allFaces0[row*9+col]);
            // Draw rectangle of that color
            rectangle(avgColorPic0, Point(col*tileSize,row*tileSize), Point((col+1)*tileSize,(row+1)*tileSize), faceAvgColors0[row*9+col], -1);
            imshow("avgColorPic0", avgColorPic0);
            waitKey(500);
            // Wait for user input to determine the color
            std::string color;
            cout << "What color is this?: ";
            cin >> color;
            faceColors[row*9 + col] = parseColor(color);
        }
    }

    // Print out current facelet colros (defined by user)
    cout << "[";
    for(int i = 0; i < 27; i++){
        cout << faceColors[i]<<',';
    }
    cout << "]" << endl;

    imshow("avgColorPic0", avgColorPic0);
    imwrite("avgImg0.jpg",avgColorPic0);

    // Image 1
    Scalar faceAvgColors1 [27];
    Mat avgColorPic1(3*tileSize,9*tileSize, CV_8UC3, Scalar(0,0,0));

    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 9; col++){
            faceAvgColors1[row*9+col] = averageColor(src1, allFaces1[row*9+col]);
            rectangle(avgColorPic1, Point(col*tileSize,row*tileSize), Point((col+1)*tileSize,(row+1)*tileSize), faceAvgColors1[row*9+col], -1);
            imshow("avgColorPic1", avgColorPic1);
            waitKey(0);
        }
    }

    imshow("avgColorPic1", avgColorPic1);
    imwrite("avgImg1.jpg",avgColorPic1);

}

vector<vector<Scalar>> getFaceColors(const Mat& src0, const Mat& src1)
{
    vector<vector<Scalar>> faceColors(6,vector<Scalar>(9));
    for (size_t face = 0; face < 3; ++face) {
        for (size_t facelet = 0; facelet < 9; ++facelet) {
            faceColors[face][facelet] = averageColor(src0, allFaces0[9*face+facelet]);
        }
    }

    for (size_t face = 3; face < 6; ++face) {
        for (size_t facelet = 0; facelet < 9; ++facelet) {
            faceColors[face][facelet] = averageColor(src1, allFaces0[9*face+facelet]);
        }
    }

    return faceColors;
}

int main(void)
{
    Mat img0, img1;
    // Load the images
    img0 = imread("template0.jpg"); //, CV_LOAD_IMAGE_COLOR);
    img1 = imread("template1.jpg"); //, CV_LOAD_IMAGE_COLOR);

    // Make sure img aren't empty
    if((!img0.data) || (!img1.data))
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    // Display original images
    imshow("img0", img0);
    imshow("img1", img1);
    waitKey(1);

    // Do all the processing here
    //findFaceColors(img0, img1);
    getFaceColors(img0, img1);



/*
    // Crop polygon
    // Blue square on UL face image 0
    cv::Point cns[1][4];
    cns[0][0] = Point(325,76);
    cns[0][1] = Point(392,100);
    cns[0][2] = Point(416,160);
    cns[0][3] = Point(348,138);

    const Point* corner_list[1] = {cns[0]};

    int num_points = 4;
    Mat mask1(img0.rows, img0.cols, CV_8U, Scalar(0,0,0));
    Mat mask2(img0.rows, img0.cols, CV_8UC3, Scalar(0,0,0));
    fillPoly(mask1, corner_list, &num_points, 1, Scalar(255,255,255));
    fillPoly(mask2, corner_list, &num_points, 1, Scalar(255,255,255));
    // Mat1b mask(img0.rows, img0.cols, uchar(0));
    // fillPoly(mask, corner_list, &num_points, 1, Scalar(255));

    Scalar avg = mean(img0, mask1);
    std::cout << "Average color: " << avg << std::endl;

    Mat averagePic(100,100,CV_8UC4, avg);
    imshow("avg color",averagePic);

    Mat result;
    bitwise_and(img0, mask2, result);
    imshow("masked",result);

    // Mat s1 (img0, Rect(100,100,104,104));
*/

    waitKey(0);

    return 0;
}
