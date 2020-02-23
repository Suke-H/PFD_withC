#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

class Circle
{
public:
	double u_0, v_0, r;
	double area = CV_PI * r * r;

	Circle(double u_0, double v_0, double r);
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Line
{
public:
	double a, b, c;

	Line(double a, double b, double c);
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Inter2d
{
public:
	std::function<double(double, double)> f1;
	std::function<double(double, double)> f2;

	Inter2d(std::function<double(double, double)> f1, std::function<double(double, double)> f2);
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Triangle
{
public:
	double u_0, v_0, r, t;
	double area = 3 * sqrt(3) / 4 * pow(r, 2);

	Triangle(double u_0, double v_0, double r, double deg);
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Rectangle
{
public:
	double u_0, v_0, w, h, t;
	double area = w * h;

	Rectangle(double u_0, double v_0, double w, double h, double deg);
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};