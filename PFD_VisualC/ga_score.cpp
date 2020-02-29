#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "ga.h"
#include "tools2d.h"
#include "figure2d.h"

// スコア算出の前の条件チェック
// aabb内に中心座標があるか・図形が小さすぎないか
int check_condition(Person person, cv::Mat_<double> points, cv::Mat_<double> aabb, double l) {

	double umin = aabb(0, 0);
	double umax = aabb(0, 1);
	double vmin = aabb(0, 2);
	double vmax = aabb(0, 3);

	double u_0, v_0, r, w, h;

	// 円
	if (person.fig_type == 0) {
		u_0 = person.p[0], v_0 = person.p[1], r = person.p[2];
		// ダミー
		w = l / 2, h = l / 2;
		
	}

	// 正三角形
	else if (person.fig_type == 1) {
		u_0 = person.p[0], v_0 = person.p[1], r = person.p[2];
		// ダミー
		w = l / 2, h = l / 2;

	}

	// 長方形
	else {
		u_0 = person.p[0], v_0 = person.p[1], w = person.p[2], h = person.p[3];
		// ダミー
		r = l / 2;

	}

	//aabb内に中心座標がある・図形が小さすぎないとき1を返す
	if ((umin < u_0 && u_0 < umax) && (vmin < v_0 && v_0 < vmax) && (l / 5 < r && r < l) && 
		(l / 5 < w && w < l) && (l / 5 < h && h < l)) {
		return 1;
	}

	// 条件を満たさなければ0を返す
	else {
		return 0;
	}
}

// スコアを算出
// score = Cin/Ain - Cout/Aout
double calc_score(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {

	// AABB生成
	cv::Mat_<double> aabb;
	double l;
	std::tie(aabb, l) = build_aabb2d(points);

	// 条件を満たさなければ大減点
	if (!check_condition(person, points, aabb, l)) {
		return -100;
	}

	// inの密度計算
	// W >= 0(-図形の内部)のインデックスを取り出し、その数をc_inとして保存
	// 図形の面積をa_inとする
	cv::Mat_<double> w, u, v;
	double a_in;
	std::tie(u, v) = disassemble2d(points);

	// 円
	if (person.fig_type == 0) {
		Circle cir(person.p[0], person.p[1], person.p[2]);
		w = cir.f_rep_list(u, v);
		a_in = cir.area;

	}

	// 正三角形
	else if (person.fig_type == 1) {
		Triangle tri(person.p[0], person.p[1], person.p[2], person.p[3]);
		w = tri.f_rep_list(u, v);
		a_in = tri.area;

	}

	// 長方形
	else {
		Rectangle rect(person.p[0], person.p[1], person.p[2], person.p[3], person.p[4]);
		w = rect.f_rep_list(u, v);
		a_in = rect.area;

	}	

	// |f(u, v)| >= 0を満たす行は255、それ以外は0になる
	cv::Mat_<int> w_bool = w >= 0;

	// W >= 0の数をCinとして保存
	int c_in = cv::countNonZero(w_bool);

	// inの密度計算
	// c_out = 全点群数 - c_in
	int c_out = points.rows - c_in;
	// a_out = 輪郭の面積
	int a_out = out_area;

	// score = Cin/Ain - Cout/Aout を出力
	return c_in / a_in - c_out / a_out;
}