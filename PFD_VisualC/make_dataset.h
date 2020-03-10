#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// いろんな種類の標識のf-rep関数をセット
//sign_type: 標識
// 0:        半径0.3mの円
// 1:        1辺0.8mの正三角形
// 2:        1辺0.9mの正方形
// 3.        1辺0.45mのひし形(正方形)
// 倍率(scale)が2/3, 1, 1.5, 2の4種類ある
std::tuple<int, std::vector<double>, cv::Mat_<double>> set_sign2d(int sign_type, int scale);

// 図形点群＋ノイズを合わせた点群、平面パラメータ、その他正解図形パラメータ等を出力
// 1. 図形の点群数は点群密度density(/m^2)に基づく(図形点群数=density*図形の面積),
// 2. noise_rateは全点群数に対するノイズ点群の割合。ノイズは図形点群のAABBを1~1.5倍したAABB内に一様乱数に基づいて作成
std::tuple<cv::Mat_<double>, std::vector<double>, std::vector<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>>
	make_sign3d(int sign_type, int scale, double density, double low=-100, double high=100, int grid_num=50);

// ランダムに平面作成
std::vector<double> random_plane(int high = 1000);