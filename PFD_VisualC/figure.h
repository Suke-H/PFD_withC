#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

///// 3D図形 /////

// 平面
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

///// 2D図形 /////

// 円
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

// 正三角形
class Triangle
{
public:
	double u_0, v_0, r, t, area;
	// コンストラクタ
	Triangle(double u_0, double v_0, double r, double t);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// 長方形
class Rectangle
{
public:
	double u_0, v_0, w, h, t, area;
	// コンストラクタ
	Rectangle(double u_0, double v_0, double w, double h, double t);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// 直線
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

// intersection演算
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

// spin演算：(u0, v0)を中心に反時計回りにt(rad)回転させる
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