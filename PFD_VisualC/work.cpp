#include <iostream>
#include <opencv2/opencv.hpp>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;
using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "figure.h"
#include "tools.h"
#include "ga.h"
#include "ga_score.h"
#include "extract_contour.h"
#include "make_dataset.h"
#include "projection.h"
#include "work.h"

// é¿ç€ÇÃì_åQÉfÅ[É^Çì¸óÕÇ…ê}å`åüèo
void test(cv::Mat_<double> points3d, std::vector<double> plane_p, int dilate_size, int close_size, int open_size, int add_size) {

	cout << "2éüå≥Ç…ìäâe" << endl;

	// ì_åQÇïΩñ è„Ç…Ç“Ç¡ÇΩÇµèÊÇÈÇÊÇ§Ç…à⁄ìÆÇ≥ÇπÇÈ
	points3d = put_points_on_plane(points3d, plane_p);

	// 2éüå≥Ç…ìäâe
	cv::Mat_<double> points2d, u2, v2, o2;
	std::tie(points2d, u2, v2, o2) = projection2d(points3d, plane_p);

	plt::plot(points2d.col(0), points2d.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	cout << "ó÷äsíäèo" << endl;

	// ó÷äsíäèo
	int ret;
	cv::Mat_<double> out_points;
	double out_area;
	std::tie(ret, points2d, out_points, out_area) = extract_contour(points2d, dilate_size, close_size, open_size, add_size);

	plt::plot(points2d.col(0), points2d.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	cout << "GA" << endl;

	// GA
	Person person(entire_ga(points2d, out_points, out_area));

	cout << "///////////////////////////////////" << endl;
	print_vec_double(person.p, person.p.size(), "fig_p");
	print_vec_double(plane_p, plane_p.size(), "plane_p");
	cout << "u: " << u2 << endl;
	cout << "v: " << v2 << endl;
	cout << "o: " << o2 << endl;
	
}

// ÉVÉ~ÉÖÉåÅ[ÉVÉáÉìÇ≈çÏê¨ÇµÇΩì_åQÉfÅ[É^Çì¸óÕÇ…ê}å`åüèo
void simulation(int sign_type, int scale, double density, int dilate_size, int close_size, int open_size, int add_size) {

	cout << "ì¸óÕì_åQÉfÅ[É^çÏê¨" << endl;

	// ì¸óÕì_åQÉfÅ[É^çÏê¨
	cv::Mat_<double> points3d, center3d, u, v, o, aabb2d;
	std::vector<double> plane_p, fig_p;
	std::tie(points3d, plane_p, fig_p, center3d, u, v, o, aabb2d) = make_sign3d(sign_type, scale, density);

	cout << "2éüå≥Ç…ìäâe" << endl;

	// ì_åQÇïΩñ è„Ç…Ç“Ç¡ÇΩÇµèÊÇÈÇÊÇ§Ç…à⁄ìÆÇ≥ÇπÇÈ(ì¸óÕì_åQÇ…åÎç∑Ç∆Ç©Çó^Ç¶ÇƒÇ»ÇØÇÍÇŒïKóvÇ»Çµ)
	//points3d = put_points_on_plane(points3d, plane_p);

	// 2éüå≥Ç…ìäâe
	cv::Mat_<double> points2d, u2, v2, o2;
	std::tie(points2d, u2, v2, o2) = projection2d(points3d, plane_p);

	plt::plot(points2d.col(0), points2d.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	cout << "ó÷äsíäèo" << endl;

	// ó÷äsíäèo
	int ret;
	cv::Mat_<double> out_points;
	double out_area;
	std::tie(ret, points2d, out_points, out_area) = extract_contour(points2d, dilate_size, close_size, open_size, add_size);

	plt::plot(points2d.col(0), points2d.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

	cout << "GA" << endl;

	// GA
	Person person(entire_ga(points2d, out_points, out_area));

	cout << "///////////////////////////////////" << endl;
	print_vec_double(person.p, person.p.size(), "fig_p");
	print_vec_double(plane_p, plane_p.size(), "plane_p");
	cout << "center3d: " << center3d << endl;
	cout << "u: " << u << endl;
	cout << "v: " << v << endl;
	cout << "o: " << o << endl;
	cout << "u2: " << u2 << endl;
	cout << "v2: " << v2 << endl;
	cout << "o2: " << o2 << endl;
	cout << "aabb2d " << aabb2d << endl;

}


