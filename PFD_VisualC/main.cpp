#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>


using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "vec_to_mat.hpp"
#include "figure2d.h"
#include "tools2d.h"
#include "viewer.h"
#include "ga.h"


int main()
{

	// aabb
	cv::Mat aabb = (cv::Mat_<double>(1, 4) << -4.2, 4, -4, 4);

	// 正解
	Rectangle t1(1, 0, 1, 1, CV_PI/6);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f1 =
		std::bind(&Rectangle::f_rep_list, &t1, std::placeholders::_1, std::placeholders::_2);

	// 外枠
	Rectangle t2(1, 0, 1.2, 1.2, CV_PI/6);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f2 =
		std::bind(&Rectangle::f_rep_list, &t2, std::placeholders::_1, std::placeholders::_2);

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

	// 初期集団生成
	int N = 10;
	int fig_type = 1;
	double l;
	std::tie(aabb, l) = build_aabb2d(points);
	std::vector<Person> people = create_random_population(N, fig_type, aabb, l);

	for (int i = 0; i < people.size(); i++) {
		cout << i << endl;
		people[i].profile();
		cout << endl;
	}

	// ランキング
	std::vector<double> score_list;
	std::tie(people, score_list) = ranking(people, points, out_points, out_area);

	for (int i = 0; i < people.size(); i++) {
		cout << score_list[i] << endl;
	}

	for (int i = 0; i < people.size(); i++) {
		cout << i << endl;
		people[i].profile();
		cout << endl;
	}

}

