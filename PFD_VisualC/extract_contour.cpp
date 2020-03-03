#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools2d.h"

// 点群を画像に変換
std::tuple<cv::Mat, double, double, int, int, double, double> trans_pix(cv::Mat_<double> points) {

	// AABBの横長dx, 縦長dyを出す
	cv::Mat_<double> aabb;
	std::tie(aabb, std::ignore) = build_aabb2d(points);
	double dx = std::abs(aabb(0, 1) - aabb(0, 0));
	double dy = std::abs(aabb(0, 3) - aabb(0, 2));

	// 画像のピクセルサイズは横を1000に固定する -> 縦はint(dy/dx*1000)
	int px = 1000;
	int py = (int)(dy / dx * px);

	// 画像の原点[0, 0]の点群上の座標
	double cx = aabb(0, 0);
	double cy = aabb(0, 2);

	// ピクセル内に点があれば255, なければ0の画像作成
	Mat pix = Mat::zeros(py, px, CV_8U);

	Mat_<double> X, Y;
	std::tie(X, Y) = disassemble2d(points);

	for (int i = 0; i < py; i++) {

		if (i % 100 == 0) {
			cout << i << endl;
		}

		for (int j = 0; j < px; j++) {

			// 各ピクセルの範囲内に点群が存在しているかチェック
			Mat_<int> x1 = (cx + dx / px * j <= X) / 255;
			Mat_<int> x2 = (X <= cx + dx / px * (j + 1)) / 255;
			Mat_<int> y1 = (cy + dy / py * i <= Y) / 255;
			Mat_<int> y2 = (Y <= cy + dy / py * (i + 1)) / 255;

			Mat_<int> judge = x1.mul(x2);
			judge = judge.mul(y1);
			judge = judge.mul(y2);

			// 配列judge内に1が1つでもあったらこのピクセルを255にする
			double minVal, maxVal;
			cv::Point minLoc, maxLoc;
			cv::minMaxLoc(judge, &minVal, &maxVal, &minLoc, &maxLoc);

			if ((int)maxVal == 1) {
				pix.data[i * px + j] = 255;
				//pix.at<int>(j, i) = 255;
				//pix(i, j) = 255;
			}
		}
	}

	cv::imwrite("../data/test.png", pix);

	return std::forward_as_tuple(pix, dx, dy, px, py, cx, cy);
}

// モルフォロジー演算で輪郭抽出
std::tuple<int, std::vector< cv::Point >> morphology(cv::Mat pix, int dilate_size, int close_size, int open_size, int add_size) {

	// カーネル作成
	cv::Mat dilate_kernel(dilate_size, dilate_size, CV_8U, cv::Scalar::all(255));
	cv::Mat close_kernel(close_size, close_size, CV_8U, cv::Scalar::all(255));
	cv::Mat open_kernel(open_size, open_size, CV_8U, cv::Scalar::all(255));
	cv::Mat add_kernel(add_size, add_size, CV_8U, cv::Scalar::all(255));

	// 膨張
	cv::morphologyEx(pix, pix, MORPH_DILATE, dilate_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/dilate.png", pix);

	// クロージング
	cv::morphologyEx(pix, pix, MORPH_CLOSE, close_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/close.png", pix);

	// オープニング
	cv::morphologyEx(pix, pix, MORPH_OPEN, open_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/open.png", pix);

	// 膨張
	cv::morphologyEx(pix, pix, MORPH_DILATE, add_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/add.png", pix);

	// 輪郭の座標リスト
	std::vector< std::vector< cv::Point > > contours;

	// 輪郭抽出
	cv::findContours(pix, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	// 面積最大の輪郭を取り出す
	double max_area = 0;
	std::vector< cv::Point > max_contour;

	for (int i = 0; i < contours.size(); i++) {
		double area = cv::contourArea(contours[i]);

		if (area > max_area){
			max_area = area;
			max_contour = contours[i];
		}

	}

	// 輪郭が取り出せない場合の処理
	if (max_contour.empty()) {
		return std::forward_as_tuple(0, max_contour);
	}

	// 凸包の取得
	std::vector< cv::Point > hull;
	cv::convexHull(max_contour, hull);

	return std::forward_as_tuple(1, hull);
}