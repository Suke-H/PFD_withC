#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
#include <random>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools3d.h"

//[x1, y1, z1]         [x1, x2, ..., xn]
//    :       ->       [y1, y2, ..., yn]
//[xn, yn, zn]         [z1, z2, ..., zn]
// Mat_<double>Œ^‚Ì3ŸŒ³“_ŒQ‚ğx, y, z‚É•ª—£
std::tuple<cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>> disassemble3d(cv::Mat_<double> points) {

	// 3ŸŒ³“_ŒQ‚ğX, Y, Z‚É•ª—£
	cv::Mat_<double> X = points.col(0);
	cv::Mat_<double> Y = points.col(1);
	cv::Mat_<double> Z = points.col(2);
	cv::transpose(X, X);
	cv::transpose(Y, Y);
	cv::transpose(Z, Z);

	return std::forward_as_tuple(X, Y, Z);
}

// points‚ÌAABB‚ğì¬
std::tuple<cv::Mat_<double>, double> build_aabb3d(cv::Mat_<double> points) {

	// points‚ÌxÅ‘å/Å¬, yÅ‘å/Å¬, zÅ‘å/Å¬‚ğŒ©‚Â‚¯‚é
	cv::Mat_<double> max_p, min_p;
	cv::reduce(points, max_p, 0, CV_REDUCE_MAX);
	cv::reduce(points, min_p, 0, CV_REDUCE_MIN);

	// AABB
	cv::Mat aabb = (cv::Mat_<double>(1, 6) << min_p(0, 0), max_p(0, 0), min_p(0, 1), max_p(0, 1), min_p(0, 2), max_p(0, 2));

	// AABB‚Ì‘ÎŠpü
	double l = sqrt(pow((max_p(0, 0) - min_p(0, 0)), 2) + pow((max_p(0, 1) - min_p(0, 1)), 2) + pow((max_p(0, 2) - min_p(0, 2)), 2));

	return std::forward_as_tuple(aabb, l);
}