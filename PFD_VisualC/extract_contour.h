#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// 輪郭抽出
std::tuple<int, cv::Mat_<double>, cv::Mat_<double>, double> extract_contour(cv::Mat_<double> points, int dilate_size = 25, int close_size = 30, int open_size = 30, int add_size = 35);

// 点群を画像に変換
std::tuple<cv::Mat, double, double, int, int, double, double> trans_pix(cv::Mat_<double> points);

// モルフォロジー演算で輪郭抽出
std::tuple<int, std::vector< cv::Point >> morphology(cv::Mat pix, int dilate_size, int close_size, int open_size, int add_size);

// 画像座標の輪郭点を点群座標に変換
cv::Mat_<double> trans_points(std::vector< cv::Point >  contour, double dx, double dy, int px, int py, double cx, double cy);

// 点pが輪郭点contour内にあるかの判定
int inout_judgement(double px, double py, cv::Mat_<double> contour);