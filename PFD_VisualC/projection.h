#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 平面にフィットした点を平面上にぴったし乗るように移動させる
cv::Mat_<double> put_points_on_plane(cv::Mat_<double> points, std::vector<double> plane_p);

// 平面にフィットした点群と平面パラメータを入力に、点群を平面上に2次元点群として投影する
std::tuple<cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>> 
	projection2d(cv::Mat_<double> points3d, std::vector<double> plane_p);

// 平面上の2次元点群(ついでに図形の中心座標も)を3次元に射影する
std::tuple < cv::Mat_<double>, cv::Mat_<double>> projection3d(cv::Mat_<double> points2d, std::vector<double> fig_p, 
	cv::Mat_<double> u, cv::Mat_<double> v, cv::Mat_<double> o);

