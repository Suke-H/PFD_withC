#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

Mat_<double> build_aabb_2d(Mat_<double> points);

Mat_<double> linspace(double start, double stop, int num);

tuple<Mat_<double>, Mat_<double>> meshgrid2d(Mat_<double> U, Mat_<double> V);

//double make_contour(function<double(double)> f, );
