#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools2d.h"

// 図形+ノイズの2次元点群を生成
std::tuple<std::vector<double>, Mat_<double>> make_onedataset2d(int fig_type, int N, double noise_rate,
	double low = -100, double high = 100, int grid_num = 50) {

	// 出力する点群数
	int size = (int)N*noise_rate;

	while (1) {
		cv::Mat_<double> aabb = cv::Mat_<double>::zeros(1, 4);

		while (1) {
			// AABBランダム生成
			for (int i = 0; i < 2; i++) {
				double u1 = random_value(low, high);
				double u2 = random_value(low, high);

				if (u1 < u2) {
					aabb(0, i * 2) = u1;
					aabb(0, i * 2 + 1) = u2;
				}

				else {
					aabb(0, i * 2) = u2;
					aabb(0, i * 2 + 1) = u1;
				}
			}
		}

		// AABBのuv最大・最小値をとる
		double umin = aabb(0, 0);
		double umax = aabb(0, 1);
		double vmin = aabb(0, 2);
		double vmax = aabb(0, 3);
		double l = sqrt(pow((umax - umin), 2) + pow((vmax - vmin), 2));
	}
}