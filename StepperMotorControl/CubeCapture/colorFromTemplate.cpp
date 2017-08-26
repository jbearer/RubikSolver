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
#include <algorithm>
#include "faceCoords.hpp" // defines face pixel coordinates
#include <string>
#include <vector>

using namespace cv;
using namespace std;


enum Color {RED, GREEN, BLUE, ORANGE, YELLOW, BLACK, NONE};

/**
 * @brief      print operator for Color enum
 *
 * @return     output stream
 */
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

    if(s2 == "R") return RED;
    else if(s2 == "G") return GREEN;
    else if(s2 == "Y") return YELLOW;
    else if(s2 == "O") return ORANGE;
    else if(s2 == "BL") return BLUE;
    else if(s2 == "BK") return BLACK;
    else{
        cerr << "  ERROR: Unknown color imput. Only acceptable values are: R,O,Y,G,BL,BK" << endl;
        return NONE;
    }
}


bool isZero(uchar c) { return c == 0; }

double median( Mat& channel)
{
    std::vector<uchar> data;
    data.assign(channel.datastart, channel.dataend);

    //cout << "size before: " << data.size() << endl;

    std::vector<uchar> new_data;
    for (auto d : data) {
        if (d != 0) {
            new_data.push_back(d);
        }
    }

    //cout << "size after: " << new_data.size() << endl;

    size_t n = new_data.size()/2;
    std::nth_element(new_data.begin(), new_data.begin()+n, new_data.end());

    return new_data[n];
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
Scalar averageColor(const Mat src, const std::vector<std::vector<int>> points, Mat& masterMask, bool debug)
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
    Mat mask(src.rows, src.cols, CV_8UC3, Scalar(0,0,0));
    fillPoly(mask, corner_list, &num_points, 1, Scalar(255,255,255));

    // Apply mask
    Mat result;
    bitwise_and(src, mask, result);
    // Split channels
    Mat bgr[3];
    split(result,bgr);

    double medB = median(bgr[0]);
    double medG = median(bgr[1]);
    double medR = median(bgr[2]);
    Scalar medBGR (medB,medG,medR);

    // // Convert to HSV
    // avg = ScalarBGR2HSV(avg[0], avg[1], avg[2]);

    if(debug){
        // Showing the mask (an outline version, and true masked version)
        Mat maskPic0(src.rows, src.cols, CV_8UC3, Scalar(0,0,0)); // outline version
        polylines(maskPic0, corner_list, &num_points, 1, true, Scalar(255,255,255), 2, 8);
        Mat result0;
        bitwise_or(src, maskPic0, result0); // bitwise_or to keep rest of img
        // imshow("masked0",result0);
        waitKey(0);
    }

    polylines(masterMask, corner_list, &num_points, 1, true, Scalar(255,255,255), 2, 8);

    // Mat maskPic1(src.rows, src.cols, CV_8UC3, Scalar(0,0,0)); // true mask version
    // fillPoly(maskPic1, corner_list, &num_points, 1, Scalar(255,255,255));
    // Mat result1;
    // bitwise_and(src, maskPic1, result1); // bitwise_and to mask the image
    // imshow("masked1",result1);

    // waitKey(0);

    // std::cout << medBGR << std::endl;

    return medBGR;
}

Scalar averageColor2(const Mat src, const std::vector<std::vector<int>> points, Mat& masterMask, bool debug)
{
    // cout << "entering average color 2" << endl;
    // Organize coordinates as point array
    cv::Point corners1[1][4];
    corners1[0][0] = Point(points[0][0],points[0][1]);
    corners1[0][1] = Point(points[1][0],points[1][1]);
    corners1[0][2] = Point(points[2][0],points[2][1]);
    corners1[0][3] = Point(points[3][0],points[3][1]);
    const Point* corner_list1[1] = {corners1[0]};

    // Organize coordinates as point array
    cv::Point corners2[1][4];
    corners2[0][0] = Point(points[4][0],points[4][1]);
    corners2[0][1] = Point(points[5][0],points[5][1]);
    corners2[0][2] = Point(points[6][0],points[6][1]);
    corners2[0][3] = Point(points[7][0],points[7][1]);
    const Point* corner_list2[1] = {corners2[0]};


    // Create the mask
    int num_points = 4;
    Mat mask(src.rows, src.cols, CV_8U, Scalar(0,0,0));
    fillPoly(mask, corner_list1, &num_points, 1, Scalar(255,255,255));
    fillPoly(mask, corner_list2, &num_points, 1, Scalar(255,255,255));

    // Apply mask
    Mat result;
    bitwise_and(src, mask, result);
    // Split channels
    Mat bgr[3];
    split(result,bgr);

    double medB = median(bgr[0]);
    double medG = median(bgr[1]);
    double medR = median(bgr[2]);
    Scalar medBGR (medB,medG,medR);

    if(debug){
        // Showing the mask (an outline version, and true masked version)
        Mat maskPic0(src.rows, src.cols, CV_8UC3, Scalar(0,0,0)); // outline version
        polylines(maskPic0, corner_list1, &num_points, 1, true, Scalar(255,255,255), 2, 8);
        polylines(maskPic0, corner_list2, &num_points, 1, true, Scalar(255,255,255), 2, 8);
        Mat result0;
        bitwise_or(src, maskPic0, result0); // bitwise_or to keep rest of img
        // imshow("masked0",result0);

        waitKey(0);
    }

    polylines(masterMask, corner_list1, &num_points, 1, true, Scalar(255,255,255), 2, 8);
    polylines(masterMask, corner_list2, &num_points, 1, true, Scalar(255,255,255), 2, 8);

    // avg = ScalarBGR2HSV(avg[0], avg[1], avg[2]);
    // cout << avg1 << endl;
    // cout << "Average avg for one avg" << endl;

    // std::cout << medBGR << std::endl;

    return medBGR;

}

vector<vector<Scalar>> getFaceColors(const Mat& src0, const Mat& src1, bool debug)
{
    Mat mask0(src0.rows, src0.cols, CV_8UC3, Scalar(0,0,0));

    vector<vector<Scalar>> faceColors(6,vector<Scalar>(8));
    for (size_t face = 0; face < 3; ++face) {
        for (size_t facelet = 0; facelet < 8; ++facelet) {
            if (face == 1 && facelet == 5){
                faceColors[face][facelet] = averageColor2(src0, allFaces0[8*face+facelet], mask0, debug);
            }
            else{
                faceColors[face][facelet] = averageColor(src0, allFaces0[8*face+facelet], mask0, debug);
            }
            //cout << facelet << endl;
        }
    }

    Mat result0;
    bitwise_or(src0, mask0, result0); // bitwise_or to keep rest of img
    // imshow("Full Mask 0", result0);

    // waitKey(0);

    Mat mask1(src1.rows, src1.cols, CV_8UC3, Scalar(0,0,0));

    // cout << "face colors 2" << endl;
    for (size_t face = 0; face < 3; ++face) {
        for (size_t facelet = 0; facelet < 8; ++facelet) {
            if ((face == 1 && facelet == 0) ||  (face == 0 && facelet == 2) || (face == 2 && facelet == 5) ){
                faceColors[face + 3][facelet] = averageColor2(src1, allFaces1[8*face+facelet], mask1, debug);
            }
            else{
                faceColors[face + 3][facelet] = averageColor(src1, allFaces1[8*face+facelet], mask1, debug);
            }
            //cout << facelet << endl;
        }
    }

    Mat result1;
    bitwise_or(src1, mask1, result1); // bitwise_or to keep rest of img
    // imshow("Full Mask 1", result1);
    // waitKey(0);


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
    cout << "About to display images" << endl;
    // Display original images
    // imshow("img0", img0);
    // imshow("img1", img1);
    waitKey(1);
    cout << "done displaying" << endl;
    // Do all the processing here
    // findFaceColors(img0, img1);
    getFaceColors(img0, img1, false);



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
