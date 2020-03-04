#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;
using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "figure2d.h"
#include "tools2d.h"
#include "ga.h"
#include "ga_score.h"
#include "extract_contour.h"

int main()
{
	// aabb
	cv::Mat_<double> aabb = (cv::Mat_<double>(1, 4) << -4.2, 4, -4, 4);

	// aabb
	cv::Mat_<double> aabb2 = (cv::Mat_<double>(1, 4) << 0.4, 2, -1, 1);

	// AABBのuv最大・最小値をとる
	double umin = aabb2(0, 0);
	double umax = aabb2(0, 1);
	double vmin = aabb2(0, 2);
	double vmax = aabb2(0, 3);

	int grid_num = 25;

	// AABBの範囲内でu, v座標の等差数列を作成
	cv::Mat_<double> u = linspace(umin, umax, grid_num);
	cv::Mat_<double> v = linspace(vmin, vmax, grid_num);

	// AABB内の格子点にする
	cv::Mat_<double> uu;
	cv::Mat_<double> vv;
	std::tie(uu, vv) = meshgrid2d(u, v);

	plt::plot(uu, vv, { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	// 正解
	Triangle t1(1, 0, 1, CV_PI / 6);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f1 =
		std::bind(&Triangle::f_rep_list, &t1, std::placeholders::_1, std::placeholders::_2);

	// 外枠
	Triangle t2(1, 0, 1.2, CV_PI / 6);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f2 =
		std::bind(&Triangle::f_rep_list, &t2, std::placeholders::_1, std::placeholders::_2);

	// 外枠の面積
	double ans_area = t1.area;
	double out_area = t2.area;

	// 点群生成
	cv::Mat_<double> points = make_inside(f1, aabb, 500, 1.5, 500);
	cout << "points: " << points.rows << endl;
	/*cv::Mat_<double> out_points = make_contour(f2, aabb, 1.5, 1000, 0.01);
	cout << "out_points:" << out_points.rows << endl;*/

	// 輪郭抽出
	int ret;
	cv::Mat_<double> out_points;
	double area;

	std::tie(ret, points, out_points, area) = extract_contour(points);

	cout << "ret: " << ret << endl;
	cout << area << "=" << ans_area << endl;

	// 輪郭の内外判定
	cv::Mat_<double> test_points = composition2d(uu, vv);
	std::vector<int> in_list;
	for (int i = 0; i < test_points.rows; i++) {
		if (inout_judgement(test_points(i, 0), test_points(i, 1), out_points)) {
			in_list.push_back(i);
		}
	}

	// 輪郭内にある点だけ抽出
	cv::Mat_<double> inside_points = extract_rows(test_points, in_list);
	cv::Mat_<double> outside_points = delete_rows(test_points, in_list);

	plt::plot(inside_points.col(0), inside_points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(outside_points.col(0), outside_points.col(1), { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "black"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	// 内外判定
	std::vector<double> in_x_vec, in_y_vec, out_x_vec, out_y_vec;
	
	for (int i = 0; i < uu.cols; i++) {
		if (inout_judgement(uu(0, i), vv(0, i), out_points)) {
			in_x_vec.push_back(uu(0, i));
			in_y_vec.push_back(vv(0, i));
		}

		else {
			out_x_vec.push_back(uu(0, i));
			out_y_vec.push_back(vv(0, i));
		}
	}

	
	plt::plot(in_x_vec, in_y_vec, { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_x_vec, out_y_vec, { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "black"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	

	//// xml形式にMatに格納された点群を保存
	//FileStorage cvfs("C:/Users/suke0/OneDrive/デスクトップ/test/tri.xml", CV_STORAGE_WRITE);
	//write(cvfs, "points", points);
	//write(cvfs, "mout_points", out_points);

	//// xml形式から点群を読み込み
	//Mat_<double> points, out_points;
	//FileStorage cvfs("C:/Users/suke0/OneDrive/デスクトップ/test/tri.xml", CV_STORAGE_READ);
	//FileNode node(cvfs.fs, NULL);
	//read(node["points"], points);
	//read(node["out_points"], out_points);

	// 点群描画
	/*plt::plot(points.col(0), points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();*/

	//std::vector<double> p{ 1, 0, 1.2, CV_PI / 6 };
	//Person ans(1, p);
	/*Person cpy(ans);
	
	for (int i = 0; i < cpy.p.size(); i++) {
		printf("%lf, ", cpy.p[i]);
	}
	printf("\n");*/
/*
	std::vector<double> p{ 1, 0, 1.2, CV_PI / 6 };
	Person ans(1, p);
	double score = calc_score(ans, points, out_points, out_area, 1);
	cout << "score: " << score << endl;*/

	// GA
	int fig_type = 1;
	Person person(single_ga(fig_type, points, out_points, out_area));
	//Person person(entire_ga(points, out_points, out_area));
	draw_person(person, points, out_points, aabb);
}

