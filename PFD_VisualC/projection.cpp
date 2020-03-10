#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools.h"
#include "figure.h"

// 平面にフィットした点を平面上にぴったし乗るように移動させる
cv::Mat_<double> put_points_on_plane(cv::Mat_<double> points, std::vector<double> plane_p) {

	// 平面のクラス作成
	Plane plane(plane_p[0], plane_p[1], plane_p[2], plane_p[3]);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>)> f =
		std::bind(&Plane::f_rep_list, &plane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_1);

	// 平面の法線
	cv::Mat_<double> n = (cv::Mat_<double>(1, 3) << plane_p[0], plane_p[1], plane_p[2]);
	double a = n(0, 0), b = n(0, 1), c = n(0, 2);

	// 3次元点群をX, Y, Zに分離
	cv::Mat_<double> X = points.col(0);
	cv::Mat_<double> Y = points.col(1);
	cv::Mat_<double> Z = points.col(2);
	cv::transpose(X, X);
	cv::transpose(Y, Y);
	cv::transpose(Z, Z); 

	// 法線方向に点を動かして平面に落とし込む
	// f(p0 + t n) = 0 <=> t = f(p0) / (a ^ 2 + b ^ 2 + c ^ 2)
	cv::Mat_<double> t = f(X, Y, Z) / (pow(a, 2) + pow(b, 2) + pow(c, 2));

	// 大きさtでn方向に点群を動かす
	// p = p0 + t n

	int N = t.cols;
	cv::Mat_<double> tn = cv::Mat_<double>::zeros(N, 3);

	for (int i = 0; i < N; i++) {
		tn.row(i) = t(0, i) * n;
	}
	
	return points + tn;
}

// 平面にフィットした点群と平面パラメータを入力に、点群を平面上に2次元点群として投影する
std::tuple<cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>> 
	projection2d(cv::Mat_<double> points3d, std::vector<double> plane_p) {

	// 平面の法線
	cv::Mat_<double> n = (cv::Mat_<double>(1, 3) << plane_p[0], plane_p[1], plane_p[2]);

	// 新しい原点を適当に選んだ1点にする
	cv::Mat_<double> o = (cv::Mat_<double>(1, 3) << points3d(0, 0), points3d(0, 1), points3d(0, 2));

	// もう一点選ぶ
	cv::Mat_<double> p = (cv::Mat_<double>(1, 3) << points3d(1, 0), points3d(1, 1), points3d(1, 2));

	// 二次元座標軸u, vを定義
	cv::Mat_<double> u, v;
	cv::normalize(p - o, u);
	cv::normalize(u.cross(n), v);

	// [o, o, .., o]という形のmat作成
	points_t o_list;
	o_list.reserve(points3d.rows);
	for (int i = 0; i < points3d.rows; i++) {
		point_t o_point(o(0, 0), o(0, 1), o(0, 2));
		o_list.push_back(o_point);
	}

	cv::Mat_<double> o_mat = vec3_to_mat(o_list);

	// 二次元座標に変換
	cv::Mat_<double> points2d;
	cv::Mat_<double> uv = (cv::Mat_<double>(3, 2) << u(0, 0), v(0, 0), u(0, 1), v(0, 1), u(0, 2), v(0, 2));

	points2d = (points3d - o_mat) * uv;

	return std::forward_as_tuple(points2d, u, v, o);
	
}

// 平面上の2次元点群(ついでに図形の中心座標も)を3次元に射影する
std::tuple < cv::Mat_<double>, cv::Mat_<double>> projection3d(cv::Mat_<double> points2d, std::vector<double> fig_p,
	cv::Mat_<double> u, cv::Mat_<double> v, cv::Mat_<double> o) {

	// [o, o, .., o]という形のmat作成
	points_t o_list;
	o_list.reserve(points2d.rows);
	for (int i = 0; i < points2d.rows; i++) {
		point_t o_point( o(0, 0), o(0, 1), o(0, 2) );
		o_list.push_back(o_point);
	}

	cv::Mat_<double> o_mat = vec3_to_mat(o_list);

	// 2次元点群を3次元に射影
	cv::Mat_<double> uv = (cv::Mat_<double>(2, 3) << u(0, 0), u(0, 1), u(0, 2), v(0, 0),  v(0, 1), v(0, 2));
	cv::Mat_<double> points3d = points2d * uv + o_mat;

	// 図形の中心座標
	cv::Mat_<double> center2d = ( cv::Mat_<double>(1, 2) << fig_p[0], fig_p[1] );

	// 中心座標を3次元に射影
	cv::Mat_<double> center3d = center2d * uv + o;

	return std::forward_as_tuple(points3d, center3d);
}