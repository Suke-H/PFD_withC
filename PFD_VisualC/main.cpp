#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;
using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "work.h"
#include "tools.h"

int main()
{

	///// 実際の点群を入力にする場合 /////

	// 以下2つの配列が入力となる
	// points3d: cv::Mat_<double>型の点群
	// plane_p: std::vector<double>型の平面パラメータ

	// 他、dilate_size, close_size, open_size, add_sizeはモルフォロジー演算のカーネルサイズ
	// モルフォロジー演算がうまくいかなかったらdilate_sizeを上げる必要あり
	// その際、dilate + close = openにすること推奨

	// 以降、points3dはtxtファイルから読み込み、plane_pは法線nと平面上の点p1の座標を手入力する想定でテスト

	/////////////////////////////////////

	// 使用する点群を保存したtxtファイル
	ifstream ifs("../data/FC_00040.jpg.txt");

	// 平面の法線n
	cv::Vec3d n(-0.958910, 0.276703, -0.062668);

	// 平面上の点p1
	cv::Vec3d p1(-95786.281250, 12506.445313, 122.647003);

	// 平面パラメータ
	std::vector<double> plane_p;
	plane_p.push_back(n[0]);
	plane_p.push_back(n[1]);
	plane_p.push_back(n[2]);
	plane_p.push_back(n.dot(p1));

	// モルフォロジー演算のカーネルサイズ
	int dilate_size = 80, close_size = 20, open_size = 100, add_size = 20;

	// 以後pointsにtxtファイルの点群を保存する処理
	points_t points;

	string str;

	if (ifs.fail()) {
		cerr << "File do not exist.\n";
		exit(0);
	}

	float a = 0, b = 0, c = 0;
	
	while (getline(ifs, str)) {

		a = 0; b = 0; c = 0;
		sscanf(str.data(), "%f\t%f\t%f", &a, &b, &c);

		point_t point(a, b, c);
		points.push_back(point);
		
	}

	// points_t型からcv::Mat_<double>型に変換
	cv::Mat_<double> points3d;
	points3d = vec3_to_mat(points);

	test(points3d, plane_p, dilate_size, close_size, open_size, add_size);

	///// シミュレーションによる点群を入力にする場合 /////

	//sign_type: 標識
	// 0:        半径0.3mの円
	// 1:        1辺0.8mの正三角形
	// 2:        1辺0.9mの正方形
	// 3.        1辺0.45mのひし形(正方形)
	// 倍率(scale)が2/3, 1, 1.5, 2の4種類ある

	// densityは点群の密度(数/m^2)

	/////////////////////////////////////////////////////

	/*int sign_type = 0, scale = 1;
	double density = 2500;
	int dilate_size = 50, close_size = 20, open_size = 70, add_size = 20;
	simulation(sign_type, scale, density, dilate_size, close_size, open_size, add_size);*/
	
}

