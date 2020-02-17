#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

class circle
{
public:
	double u_0, v_0, r;
	double area = CV_PI * r * r;

	circle(double u_0, double v_0, double r);
	double f_rep(double u, double v);
	Mat_<double> f_rep_list(Mat_<double> U, Mat_<double> V);

};