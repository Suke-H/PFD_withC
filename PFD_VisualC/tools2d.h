#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

Mat_<double> composition2d(Mat_<double> u, Mat_<double> v);

Mat_<double> build_aabb_2d(Mat_<double> points);

Mat_<double> linspace(double start, double stop, int num);

tuple<Mat_<double>, Mat_<double>> meshgrid2d(Mat_<double> U, Mat_<double> V);

int vec_in(vector<int> v, int x);

Mat_<double> delete_rows(Mat_<double> mat_in, vector<int> row_list);

Mat_<double> extract_rows(Mat_<double> mat_in, vector<int> row_list);

Mat_<double> make_contour(function<Mat_<double>(Mat_<double>, Mat_<double>)> f, Mat_<double> aabb, double aabb_size, int grid_num, double epsilon);
