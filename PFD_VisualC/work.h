#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 実際の点群データを入力に図形検出
void test(cv::Mat_<double> points3d, std::vector<double> plane_p, int dilate_size, int close_size, int open_size, int add_size);

// シミュレーションで作成した点群データを入力に図形検出
void simulation(int sign_type, int scale, double density, int dilate_size, int close_size, int open_size, int add_size);