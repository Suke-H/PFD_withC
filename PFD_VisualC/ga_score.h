#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "ga.h"

// スコア算出の前の条件チェック
// aabb内に中心座標があるか・図形が小さすぎないか
int check_condition(Person person, cv::Mat_<double> points, cv::Mat_<double> aabb, double l);

// スコアを算出
// score = Cin/Ain - Cout/Aout
double calc_score(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area, int flag = 0);