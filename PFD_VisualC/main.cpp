#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "vec_to_mat.hpp"
#include "figure2d.h"
#include "tools2d.h"
#include "viewer.h"


int main()
{
	// 2x3 の行列
	Mat m1 = (Mat_<double>(6, 2) << 1, 2, 7, 8, 3, 4, 5, 6, 2, 3, 4, 5);

	cout << m1 << "\n" << endl;

	// m2
	vector<int> del = { 1, 3};
	Mat_<double> m2 = extract_rows(m1, del);

	cout << m2 << "\n" << endl;

	Mat u = (Mat_<double>(1, 4) << 1.1, 2.2, 3.3, 4.4);
	Mat v = (Mat_<double>(1, 4) << 5.5, 6.6, 7.7, 8.8);

	Mat_<double> uv = composition2d(u, v);

	cout << uv << "\n" << endl;

	Circle c(0, 0, 1);
	Mat aabb = (Mat_<double>(1, 4) << -1, 1, -1, 1);

	Mat_<double> points = make_contour(circle_f_rep_list, aabb, 1, 1000, 0.01);

	/*cout << points << "\n" << endl;*/

	plot_points2d(points);
}

