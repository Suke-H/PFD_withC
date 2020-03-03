#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// “_ŒQ‚ð‰æ‘œ‚É•ÏŠ·
std::tuple<cv::Mat, double, double, int, int, double, double> trans_pix(cv::Mat_<double> points);

std::tuple<int, std::vector< cv::Point >> morphology(cv::Mat pix, int dilate_size = 25, int close_size = 30, int open_size = 30, int add_size = 35);