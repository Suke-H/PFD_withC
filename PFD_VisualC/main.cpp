#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;
using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "vec_to_mat.hpp"
#include "figure2d.h"
#include "tools2d.h"
#include "ga.h"
#include "ga_score.h"

int main()
{

	// aabb
	cv::Mat aabb = (cv::Mat_<double>(1, 4) << -4.2, 4, -4, 4);

	// 正解
	Triangle t1(1, 0, 1, CV_PI/6);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f1 =
		std::bind(&Triangle::f_rep_list, &t1, std::placeholders::_1, std::placeholders::_2);

	// 外枠
	Triangle t2(1, 0, 1.2, CV_PI/6);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f2 =
		std::bind(&Triangle::f_rep_list, &t2, std::placeholders::_1, std::placeholders::_2);

	// 外枠の面積
	double out_area = t2.area;

	// 点群生成
	cv::Mat_<double> points = make_inside(f1, aabb, 500, 1.5, 500);
	cout << "points:" << points.rows << endl;
	cv::Mat_<double> out_points = make_contour(f2, aabb, 1.5, 1000, 0.01);
	cout << "out_points:" <<  out_points.rows << endl;

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

	std::vector<double> p{ 1, 0, 1.2, CV_PI / 6 };
	Person ans(1, p);
	double score = calc_score(ans, points, out_points, out_area, 1);
	cout << "score: " << score << endl;
	draw_person(ans, points, out_points, aabb);

	// GA
	int fig_type = 1;
	Person person(single_ga(fig_type, points, out_points, out_area));

}

