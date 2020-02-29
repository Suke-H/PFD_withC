#include <iostream>
#include <opencv2/opencv.hpp>

#include "figure2d.h"

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;


// �R���X�g���N�^
Circle::Circle(double u_0, double v_0, double r)
{
	this->u_0 = u_0;
	this->v_0 = v_0;
	this->r = r;

	// �ʐ�
	area = CV_PI * r * r;
}

// f_rep
double Circle::f_rep(double u, double v) {
	return r - sqrt(pow(u-u_0, 2) + pow(v-v_0, 2));
}

// f_rep�̃��X�g��Ԃ�
cv::Mat_<double> Circle::f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V) {
	int N = U.cols;

	// ���ʂ�ۑ�����Mat
	cv::Mat_<float> result = cv::Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(0, i), V(0, i));

	}

	return result;
}

// �R���X�g���N�^
Line::Line(double a, double b, double c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}

// f_rep
double Line::f_rep(double u, double v) {
	return c - (a * u + b * v);
}

// f_rep�̃��X�g��Ԃ�
cv::Mat_<double> Line::f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V) {
	int N = U.cols;

	// ���ʂ�ۑ�����Mat
	cv::Mat_<float> result = cv::Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(0, i), V(0, i));

	}

	return result;
}

// �R���X�g���N�^
Inter2d::Inter2d(std::function<double(double, double)> f1, std::function<double(double, double)> f2)
{
	this->f1 = f1;
	this->f2 = f2;
}

// f_rep
double Inter2d::f_rep(double u, double v) {
	return f1(u, v) + f2(u, v) - sqrt(pow(f1(u, v), 2) + pow(f2(u, v), 2));
}

// f_rep�̃��X�g��Ԃ�
cv::Mat_<double> Inter2d::f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V) {
	int N = U.cols;

	// ���ʂ�ۑ�����Mat
	cv::Mat_<float> result = cv::Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(0, i), V(0, i));
	}

	return result;
}

// �R���X�g���N�^
Spin2d::Spin2d(std::function<double(double, double)> f, double u_0, double v_0, double t)
{
	this->f = f;
	this->u_0 = u_0;
	this->v_0 = v_0;
	this->t = t;

	// ��]�s��
	double a = std::cos(t);
	double b = std::sin(t);
	r = (cv::Mat_<double>(2, 2) << a, -b, b, a);
	// �t�s��
	r_inv = r.inv();

}

// f_rep
// x = r_inv(X-x0) + x0 ��f(x)�ɑ������
double Spin2d::f_rep(double u, double v) {
	cv::Mat p = (cv::Mat_<double>(2, 1) << u, v);
	cv::Mat x_0 = (cv::Mat_<double>(2, 1) << u_0, v_0);

	cv::Mat_<double> x = r_inv * (p - x_0) + x_0;

	return f(x(0, 0), x(1, 0));
}

// f_rep�̃��X�g��Ԃ�
cv::Mat_<double> Spin2d::f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V) {
	int N = U.cols;

	// ���ʂ�ۑ�����Mat
	cv::Mat_<float> result = cv::Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(0, i), V(0, i));
	}

	return result;
}

// �R���X�g���N�^
Triangle::Triangle(double u_0, double v_0, double r, double t)
{
	this->u_0 = u_0;
	this->v_0 = v_0;
	this->r = r;
	this->t = t;

	// �ʐ�
	area = 3 * sqrt(3) / 4 * pow(r, 2);

}

// f_rep
double Triangle::f_rep(double u, double v) {

	// 3�ӂ̒�����`
	Line l1(0, -1, -v_0 + r / 2);
	Line l2(sqrt(3)/2, 0.5, sqrt(3)/2*u_0 + v_0/2 + r/2);
	Line l3(-sqrt(3)/2, 0.5, -sqrt(3)/2*u_0 + v_0/2 + r/2);

	std::function<double(double, double)> f1 =
		std::bind(&Line::f_rep, &l1, std::placeholders::_1, std::placeholders::_2);
	std::function<double(double, double)> f2 =
		std::bind(&Line::f_rep, &l2, std::placeholders::_1, std::placeholders::_2);
	std::function<double(double, double)> f3 =
		std::bind(&Line::f_rep, &l3, std::placeholders::_1, std::placeholders::_2);

	// 3�ӂ�intersection���Ƃ�
	Inter2d i12(f1, f2);
	std::function<double(double, double)> f12 =
		std::bind(&Inter2d::f_rep, &i12, std::placeholders::_1, std::placeholders::_2);

	Inter2d i123(f12, f3);
	std::function<double(double, double)> f123 =
		std::bind(&Inter2d::f_rep, &i123, std::placeholders::_1, std::placeholders::_2);

	// t(rad)��]������
	Spin2d triangle(f123, u_0, v_0, t);

	return triangle.f_rep(u, v);
}

// f_rep�̃��X�g��Ԃ�
cv::Mat_<double> Triangle::f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V) {
	int N = U.cols;

	// ���ʂ�ۑ�����Mat
	cv::Mat_<float> result = cv::Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(0, i), V(0, i));
	}

	return result;
}

// �R���X�g���N�^
Rectangle::Rectangle(double u_0, double v_0, double w, double h, double t)
{
	this->u_0 = u_0;
	this->v_0 = v_0;
	this->w = w;
	this->h = h;
	this->t = t;

	// �ʐ�
	area = w * h;
}

// f_rep
double Rectangle::f_rep(double u, double v) {
	Line l1(0, 1, v_0 + h / 2);
	Line l2(0, -1, -v_0 + h / 2);
	Line l3(-1, 0, -u_0 + w / 2);
	Line l4(1, 0, u_0 + w / 2);

	std::function<double(double, double)> f1 =
		std::bind(&Line::f_rep, &l1, std::placeholders::_1, std::placeholders::_2);
	std::function<double(double, double)> f2 =
		std::bind(&Line::f_rep, &l2, std::placeholders::_1, std::placeholders::_2);
	std::function<double(double, double)> f3 =
		std::bind(&Line::f_rep, &l3, std::placeholders::_1, std::placeholders::_2);
	std::function<double(double, double)> f4 =
		std::bind(&Line::f_rep, &l4, std::placeholders::_1, std::placeholders::_2);

	Inter2d i12(f1, f2);
	std::function<double(double, double)> f12 =
		std::bind(&Inter2d::f_rep, &i12, std::placeholders::_1, std::placeholders::_2);
	Inter2d i123(f12, f3);
	std::function<double(double, double)> f123 =
		std::bind(&Inter2d::f_rep, &i123, std::placeholders::_1, std::placeholders::_2);
	Inter2d i1234(f123, f4);
	std::function<double(double, double)> f1234 =
		std::bind(&Inter2d::f_rep, &i1234, std::placeholders::_1, std::placeholders::_2);

	// t(rad)��]������
	Spin2d rectangle(f1234, u_0, v_0, t);

	return rectangle.f_rep(u, v);
}

// f_rep�̃��X�g��Ԃ�
cv::Mat_<double> Rectangle::f_rep_list(cv::Mat_<double> U, cv::Mat_<double> V) {
	int N = U.cols;

	// ���ʂ�ۑ�����Mat
	cv::Mat_<double> result = cv::Mat_<double>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(0, i), V(0, i));
	}

	return result;
}