#pragma once

#include <opencv.hpp>
#include <vector>
#include <map>

#include "translate.h"

typedef std::map<CubeSolver::Color, cv::Scalar> ColorMap;
std::vector<std::vector<ColorMap>> faceColorMap();
