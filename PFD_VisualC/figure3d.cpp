#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "figure3d.h"

// コンストラクタ
Plane::Plane(double a, double b, double c, double d)
{
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;

}

// f_rep
double Plane::f_rep(double x, double y, double z) {
	return d - (a*x + b*y + c*z);
}

// f_repのリストを返す
cv::Mat_<double> Plane::f_rep_list(cv::Mat_<double> X, cv::Mat_<double> Y, cv::Mat_<double> Z) {
	int N = X.cols;

	// 結果を保存するMat
	cv::Mat_<float> result = cv::Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		result(0, i) = f_rep(X(0, i), Y(0, i), Z(0, i));

	}

	return result;
}