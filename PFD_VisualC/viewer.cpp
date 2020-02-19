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

void plot_points2d(Mat_<double> points) {

	// 2�����_�Q��u��v�ɕ���
	Mat u = points.col(0);
	Mat v = points.col(1);

	// cv::Mat��std::vector�ɕϊ�
	vector<double> u_vec;
	u.copyTo(u_vec);
	vector<double> v_vec;
	v.copyTo(v_vec);

	plt::plot(u, v, { {"color", "red"} ,{ "marker", "o" }, { "linestyle", "" }});
	plt::show();
}