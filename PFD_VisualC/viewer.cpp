#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "viewer.h"

void plot_points2d(cv::Mat_<double> points) {

	// 2次元点群をuとvに分解
	cv::Mat u = points.col(0);
	cv::Mat v = points.col(1);

	// cv::Matをstd::vectorに変換
	std::vector<double> u_vec;
	u.copyTo(u_vec);
	std::vector<double> v_vec;
	v.copyTo(v_vec);

	plt::plot(u, v, { {"color", "red"} ,{ "marker", "o" }, { "linestyle", "" }});
	plt::show();
}