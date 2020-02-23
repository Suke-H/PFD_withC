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
	std::vector<int> v{ 0, 1, 2, 3, 4, 5, 6, 7 };
	v = random_sample(v, 5);
	for (int i=0; i < v.size(); i++) {
		cout << v[i] << "," << endl;
	}

	double n = random_value(10, 30);
	cout << n << endl;

	cout << "\n" << endl;

	cv::Mat aabb = (cv::Mat_<double>(1, 4) << -3, 3, -3, 3);

	Rectangle r1(0, 0, 1, 1, 45);

	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f1 = 
		std::bind(&Rectangle::f_rep_list, &r1, 
					std::placeholders::_1, std::placeholders::_2);

	double d1 = r1.f_rep(0, 0);
	cout << d1 << endl;

	Triangle t1(1, 0, 2, 60);

	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f2 =
	std::bind(&Triangle::f_rep_list, &t1,
	std::placeholders::_1, std::placeholders::_2);

	Circle c1(0, 0, 1);

	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f3 =
	std::bind(&Circle::f_rep_list, &c1,
	std::placeholders::_1, std::placeholders::_2);

	double d2 = c1.f_rep(0, 0);
	cout << d2 << endl;


	cv::Mat_<double> points = make_inside(f1, aabb, 1000, 1.5, 500);

	std::cout << points.rows << "\n" << std::endl;

	plot_points2d(points);
}

