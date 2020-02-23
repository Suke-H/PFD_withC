#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

std::tuple<double, double, int, int, double, double> trans_pix(cv::Mat_<double> points);

