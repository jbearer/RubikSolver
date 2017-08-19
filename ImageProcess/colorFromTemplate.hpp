#pragma once
/**
 * colorFromTemplate.hpp
 * TODO: expand header
 */

#include <opencv.hpp>
#include <vector>

std::vector<std::vector<cv::Scalar>> getFaceColors(const cv::Mat& img0, const cv::Mat& img1);
