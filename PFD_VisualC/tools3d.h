#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
#include <random>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

//[x1, y1, z1]         [x1, x2, ..., xn]
//    :       ->       [y1, y2, ..., yn]
//[xn, yn, zn]         [z1, z2, ..., zn]
// Mat_<double>Œ^‚Ì3ŸŒ³“_ŒQ‚ğx, y, z‚É•ª—£
std::tuple<cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>> disassemble3d(cv::Mat_<double> points);

// points‚ÌAABB‚ğì¬
std::tuple<cv::Mat_<double>, double> build_aabb3d(cv::Mat_<double> points);