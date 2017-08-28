#pragma once

#include <opencv.hpp>
#include <vector>
#include <map>

#include "translate.h"

typedef std::map<CubeSolver::Color, cv::Scalar> ColorMap;
std::vector<std::vector<ColorMap>> faceColorMap();

std::vector<std::vector<std::vector<CubeSolver::Color>>> getColorCandidates(const std::vector<std::vector<cv::Scalar>>& faceColors, const std::vector<std::vector<ColorMap>>& colorMap);
std::vector<CubeSolver::Color> closestColor(cv::Scalar rgb, ColorMap colorMap);
