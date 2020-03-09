#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

class Plane
{
public:
	double a, b, c, d;
	// コンストラクタ
	Plane(double a, double b, double c, double d);
	// f-rep
	double f_rep(double x, double y, double z);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> X, cv::Mat_<double> Y, cv::Mat_<double> Z);

};