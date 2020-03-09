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
#include "tools3d.h"
#include "ga.h"
#include "ga_score.h"
#include "extract_contour.h"
#include "make_dataset.h"
#include "projection.h"

int main()
{
	int sign_type = 1, scale = 1;
	double density = 2000, noise_rate = 0;

	cv::Mat_<double> points3d, center3d, u, v, o, aabb2d;
	std::vector<double> plane_p, fig_p;

	std::tie(points3d, plane_p, fig_p, center3d, u, v, o, aabb2d) = make_sign3d(sign_type, scale, density, noise_rate);

	points3d = put_points_on_plane(points3d, plane_p);

	/*for (int i = 0; i < points3d.rows; i++) {
		cout << points3d(i, 0) << " " << points3d(i, 1) << " " << points3d(i, 2) << endl;
	}*/

	cv::Mat_<double> points2d, u2, v2, o2;
	std::tie(points2d, u2, v2, o2) = projection2d(points3d, plane_p);

	plt::plot(points2d.col(0), points2d.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::show();
	plt::close();

}

