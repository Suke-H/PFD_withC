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
	double u_0, v_0, r, area;
	// コンストラクタ
	Circle(double u_0, double v_0, double r);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Line
{
public:
	double a, b, c;
	// コンストラクタ
	Line(double a, double b, double c);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Inter2d
{
public:
	std::function<double(double, double)> f1;
	std::function<double(double, double)> f2;
	// コンストラクタ
	Inter2d(std::function<double(double, double)> f1, std::function<double(double, double)> f2);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Spin2d
{
public:
	std::function<double(double, double)> f;
	double u_0;
	double v_0;
	double t;
	// コンストラクタ
	Spin2d(std::function<double(double, double)> f, double u_0, double v_0, double t);
	// 回転行列
	cv::Mat_<double> r;
	// 逆行列
	cv::Mat_<double> r_inv;
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Triangle
{
public:
	double u_0, v_0, r, t, area;
	// コンストラクタ
	Triangle(double u_0, double v_0, double r, double deg);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

class Rectangle
{
public:
	double u_0, v_0, w, h, t, area;
	// コンストラクタ
	Rectangle(double u_0, double v_0, double w, double h, double deg);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};