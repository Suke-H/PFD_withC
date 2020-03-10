#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

///// 3D�}�` /////

// ����
class Plane
{
public:
	double a, b, c, d;
	// �R���X�g���N�^
	Plane(double a, double b, double c, double d);
	// f-rep
	double f_rep(double x, double y, double z);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> X, cv::Mat_<double> Y, cv::Mat_<double> Z);

};

///// 2D�}�` /////

// �~
class Circle
{
public:
	double u_0, v_0, r, area;
	// �R���X�g���N�^
	Circle(double u_0, double v_0, double r);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// ���O�p�`
class Triangle
{
public:
	double u_0, v_0, r, t, area;
	// �R���X�g���N�^
	Triangle(double u_0, double v_0, double r, double t);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// �����`
class Rectangle
{
public:
	double u_0, v_0, w, h, t, area;
	// �R���X�g���N�^
	Rectangle(double u_0, double v_0, double w, double h, double t);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// ����
class Line
{
public:
	double a, b, c;
	// �R���X�g���N�^
	Line(double a, double b, double c);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// intersection���Z
class Inter2d
{
public:
	std::function<double(double, double)> f1;
	std::function<double(double, double)> f2;
	// �R���X�g���N�^
	Inter2d(std::function<double(double, double)> f1, std::function<double(double, double)> f2);
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};

// spin���Z�F(u0, v0)�𒆐S�ɔ����v����t(rad)��]������
class Spin2d
{
public:
	std::function<double(double, double)> f;
	double u_0;
	double v_0;
	double t;
	// �R���X�g���N�^
	Spin2d(std::function<double(double, double)> f, double u_0, double v_0, double t);
	// ��]�s��
	cv::Mat_<double> r;
	// �t�s��
	cv::Mat_<double> r_inv;
	// f-rep
	double f_rep(double u, double v);
	cv::Mat_<double> f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V);

};