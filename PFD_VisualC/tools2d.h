#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

double random_value(double low, double high);

std::tuple<cv::Mat_<double>, cv::Mat_<double>> disassemble2d(cv::Mat_<double> points);

cv::Mat_<double> composition2d(cv::Mat_<double> u, cv::Mat_<double> v);

std::tuple<cv::Mat_<double>, double> build_aabb2d(cv::Mat_<double> points);

cv::Mat_<double> linspace(double start, double stop, int num);

std::tuple<cv::Mat_<double>, cv::Mat_<double>> meshgrid2d(cv::Mat_<double> U, cv::Mat_<double> V);

int vec_search(std::vector<int> v, int x);

std::vector<int> random_sample(std::vector<int> v, int size);

cv::Mat_<double> delete_rows(cv::Mat_<double> Mat_in, std::vector<int> row_list);

cv::Mat_<double> extract_rows(cv::Mat_<double> Mat_in, std::vector<int> row_list);

cv::Mat_<double> make_contour(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, 
	double aabb_size=1.5, int grid_num=1000, double epsilon=0.01);

cv::Mat_<double> make_inside(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, int N,
	double aabb_size = 1.5, int grid_num = 200);