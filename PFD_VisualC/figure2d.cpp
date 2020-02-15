#include <iostream>
#include <opencv2/opencv.hpp>

#include "figure2d.h"

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

float circle_f_rep(float x, float y) {
	return x + y;
}

Mat_<float> circle(Mat_<float> X, Mat_<float> Y) {

	int N = X.rows;
	printf("N:%d", N);
	std::cout << X << "\n" << std::endl;
	std::cout << Y << "\n" << std::endl;

	// Œ‹‰Ê‚ð•Û‘¶‚·‚éMat
	Mat_<float> f_rep = Mat_<float>::zeros(1, N);

	for (int i = 0; i < N; i++) {
		printf("(%f, %f)\n", X(i, 0), Y(i, 0));
		f_rep(0, i) = circle_f_rep(X(i, 0), Y(i, 0));

	}

	return f_rep;
}