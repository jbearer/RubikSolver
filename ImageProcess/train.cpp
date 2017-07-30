#include <opencv.hpp>
#include <highgui.hpp>
#include <videoio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "cube.h"

using cv::VideoCapture;
using CubeSolver::Cube;
using CubeSolver::Turn;

using namespace cv;
using namespace CommProtocol;
using namespace CubeSolver;

int PORT1 = 1;              // Ports for the cameras
int PORT2 = 2;
std::string PATH = "";
size_t DELAY = 500;         // Delay in milliseconds

std::vector<Turn> allTurns({
        FRONT, RIGHT, BACK, LEFT, UP, DOWN,
        FRONT_INVERTED, RIGHT_INVERTED, BACK_INVERTED,
        LEFT_INVERTED, UP_INVERTED, DOWN_INVERTED
    });

Turn randomTurn() {
    int i = rand() % allTurns.size();
    return allTurns[i];
}

void snap(size_t n, VideoCapture v1, VideoCapture v2){

    Mat save_img1; v1 >> save_img1;
    Mat save_img2; v2 >> save_img2;

    imwrite(PATH + "a" + std::to_string(n) + ".png", save_img1);
    imwrite(PATH + "b" + std::to_string(n) + ".png", save_img2);
}

void train(size_t n, VideoCapture v1, VideoCapture v2){
    Cube cube;
    std::vector<Cube> all_cubes;

    for (size_t i = 0; i < n; ++i) {
        // apply a random turn to the cube
        Turn t = randomTurn();
        cube = Cube::turn(cube, t);

        // HERE: call function to actually turn the physical cube

        // add it to the vector of cubes, and take a picture
        all_cubes.push_back(cube);
        snap(i, v1, v2);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // store the tables in turnTables.ser
    std::ofstream os(PATH + "cubes.ser", std::ios::binary);
    boost::archive::binary_oarchive oarch(os);

    oarch << all_cubes;

    os.close();
}

void read() {
    std::ifstream is(PATH + "cubes.ser", std::ios::binary);
    boost::archive::binary_iarchive iarch(is);

    std::vector<Cube> cubes;

    iarch >> cubes;
    for (Cube c: cubes) {
        std::cout << c << std::endl;
    }
    is.close();
}

int main() {
    //train(3);
    //read();
    VideoCapture v1(PORT1);
    VideoCapture v2(PORT2);

    snap(3, v1, v2);
    return 0;
}
