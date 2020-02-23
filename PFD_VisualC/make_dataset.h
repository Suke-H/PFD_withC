#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

std::tuple<std::vector<double>, Mat_<double>> make_onedataset2d(int fig_type, int N, double noise_rate, 
	double low = -100, double high = 100, int grid_num = 50);