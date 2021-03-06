#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// Φso
std::tuple<int, cv::Mat_<double>, cv::Mat_<double>, double> extract_contour(cv::Mat_<double> points, int dilate_size = 50, int close_size = 20, int open_size = 70, int add_size = 20);

// _QπζΙΟ·
std::tuple<cv::Mat, double, double, int, int, double, double> trans_pix(cv::Mat_<double> points);

// tHW[ZΕΦso
std::tuple<int, std::vector< cv::Point >, double> morphology(cv::Mat pix, int dilate_size, int close_size, int open_size, int add_size);

// ζΐWΜΦs_π_QΐWΙΟ·
cv::Mat_<double> trans_points(std::vector< cv::Point >  contour, double dx, double dy, int px, int py, double cx, double cy);

// _pͺΦs_contourΰΙ ι©Μ»θ
int inout_judgement(double px, double py, cv::Mat_<double> contour);