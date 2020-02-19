#include <iostream>
#include <opencv2/opencv.hpp>

#include "figure2d.h"

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;


// コンストラクタ
Circle::Circle(double u_0, double v_0, double r)
{
	this->u_0 = u_0;
	this->v_0 = v_0;
	this->r = r;
}

// f_rep
double Circle::f_rep(double u, double v) {
	//return r - sqrt(u*u + v * v);
	return u + v;
}

// f_repのリストを返す
Mat_<double> Circle::f_rep_list(Mat_<double> U, Mat_<double> V) {
	int N = U.rows;

	// 結果を保存するMat
	Mat_<float> result = Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(U(i, 0), V(i, 0));

	}

	return result;
}

// f_rep
double circle_f_rep(double u, double v) {
	return 1 - sqrt(u*u + v * v);
}

// f_repのリストを返す
Mat_<double> circle_f_rep_list(Mat_<double> U, Mat_<double> V) {
	int N = U.cols;

	// 結果を保存するMat
	Mat_<float> result = Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = circle_f_rep(U(0, i), V(0, i));

	}

	return result;
}