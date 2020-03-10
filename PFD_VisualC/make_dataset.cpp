#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "make_dataset.h"
#include "tools.h"
#include "figure.h"
#include "projection.h"

//sign_type: 標識
// 0:        半径0.3mの円
// 1:        1辺0.8mの正三角形
// 2:        1辺0.9mの正方形
// 3.        1辺0.45mのひし形(正方形)
// 倍率(scale)が2/3, 1, 1.5, 2の4種類ある
// いろんな種類の標識のf-rep関数をセット
std::tuple<int, std::vector<double>, cv::Mat_<double>> set_sign2d(int sign_type, int scale) {

	int fig_type;
	std::vector<double> fig_p;
	cv::Mat_<double> aabb;

	// 0: 半径0.3mの円
	if (sign_type == 0) {

		// 円
		fig_type = 0;
		// p = [0, 0, 0.3*scale]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.3*scale);
		// 描画用のAABB
		aabb = cv::Mat_<double>{ -0.35, 0.35, -0.35, 0.35 }*scale;
	}

	// 1: 1辺0.8mの正三角形
	if (sign_type == 1) {

		// 正三角形
		fig_type = 1;
		// p = [0, 0, 0.8/√3*scale, 0～pi*3/2]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.8/sqrt(3)*scale);
		fig_p.push_back(random_value(0, CV_PI*3/2));
		// 描画用のAABB
		aabb = cv::Mat_<double>{ -0.45, 0.45, -0.45, 0.45 }*scale;
	}

	// 2: 1辺0.9mの正方形
	if (sign_type == 2) {

		// 長方形
		fig_type = 2;
		// p = [0, 0, 0.9*scale, 0.9*scale, 0～pi/2]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.9*scale);
		fig_p.push_back(0.9*scale);
		fig_p.push_back(random_value(0, CV_PI / 2));
		// 描画用のAABB
		double l = 0.9*sqrt(2) / 2;
		aabb = cv::Mat_<double>{ -l * 1.1, l*1.1, -l * 1.1, l*1.1 }*scale;
	}

	// 3. 1辺0.45mのひし形(正方形)
	if (sign_type == 3) {

		// 長方形
		fig_type = 2;
		// p = [0, 0, 0.45*scale, 0.45*scale, 0～pi/2]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.45*scale);
		fig_p.push_back(0.45*scale);
		fig_p.push_back(random_value(0, CV_PI / 2));
		// 描画用のAABB
		double l = 0.45*sqrt(2) / 2;
		aabb = cv::Mat_<double>{ -l * 1.1, l*1.1, -l * 1.1, l*1.1 }*scale;
	}

	return std::forward_as_tuple(fig_type, fig_p, aabb);
}

// 1. 図形の点群数は点群密度density(/m^2)に基づく(図形点群数=density*図形の面積),
// 2. noise_rateは全点群数に対するノイズ点群の割合。ノイズは図形点群のAABBを1~1.5倍したAABB内に一様乱数に基づいて作成

// 図形点群＋ノイズを合わせた点群、平面パラメータ、その他正解図形パラメータ等を出力
std::tuple<cv::Mat_<double>, std::vector<double>, std::vector<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>>
	make_sign3d(int sign_type, int scale, double density, double low, double high, int grid_num) {

	/////  図形点群作成  /////

	// 平面図形設定
	int fig_type;
	std::vector<double> fig_p;
	cv::Mat_<double> aabb2d;
	std::tie(fig_type, fig_p, aabb2d) = set_sign2d(sign_type, scale);

	// 図形点群数
	int fig_size;

	// 図形点群作成
	cv::Mat_<double> fig_points;

	// 円
	if (fig_type == 0) {
		Circle figure(fig_p[0], fig_p[1], fig_p[2]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Circle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);

		fig_size = (int)density * figure.area;
		fig_points = make_inside(f, aabb2d, fig_size, 1, grid_num);

	}

	// 正三角形
	else if (fig_type == 1) {
		Triangle figure(fig_p[0], fig_p[1], fig_p[2], fig_p[3]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Triangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);

		fig_size = (int)density * figure.area;
		fig_points = make_inside(f, aabb2d, fig_size, 1, grid_num);
	}

	// 長方形
	else {
		Rectangle figure(fig_p[0], fig_p[1], fig_p[2], fig_p[3], fig_p[4]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Rectangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);

		fig_size = (int)density * figure.area;
		fig_points = make_inside(f, aabb2d, fig_size, 1, grid_num);
	}

	/////  平面に2D点群を射影  /////

	// 平面ランダム作成
	std::vector<double> plane_p = random_plane();

	// 平面上の2点o, pをランダムに定める
	double a = plane_p[0], b = plane_p[1], c = plane_p[2], d = plane_p[3];
	double ox = random_value(low, high), oy = random_value(low, high), px = random_value(low, high), py = random_value(low, high);
	double oz = (d - a * ox - b * oy) / c, pz = (d - a * px - b * py) / c;
	cv::Mat_<double> o = (cv::Mat_<double>(1, 3) << ox, oy, oz);
	cv::Mat_<double> p = (cv::Mat_<double>(1, 3) << px, py, pz);

	// 平面の法線n, 二次元座標軸u, vを定義
	cv::Mat_<double> n, u, v;
	n = (cv::Mat_<double>(1, 3) << a, b, c);
	cv::normalize(p - o, u);
	cv::normalize(u.cross(n), v);

	// 図形点群と図形の中心座標を平面に3D射影
	cv::Mat_<double> points3d, center3d;
	std::tie(points3d, center3d) = projection3d(fig_points, fig_p, u, v, o);

	return std::forward_as_tuple(points3d, plane_p, fig_p, center3d, u, v, o, aabb2d);

}

// ランダムに平面作成
std::vector<double> random_plane(int high) {

	// 法線作成
	cv::Mat_<double> n = cv::Mat_<double>{ 0, 0, 0 };

	while (cv::norm(n) == 0) {
		n(0, 0) = random_value(-high, high);
		n(0, 1) = random_value(-high, high);
		n(0, 2) = random_value(-high, high);
	}

	// 正規化
	cv::normalize(n, n);

	// d作成
	double d = random_value(-high, high);

	// 平面パラメータ
	std::vector<double> plane_p{ n(0, 0), n(0, 1), n(0, 2), d };

	return plane_p;
}