#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

float circle_f_rep(float x, float y);

Mat_<float> circle(Mat_<float> X, Mat_<float> Y);