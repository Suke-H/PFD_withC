#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools2d.h"

// �_�Q���摜�ɕϊ�
std::tuple<cv::Mat, double, double, int, int, double, double> trans_pix(cv::Mat_<double> points) {

	// AABB�̉���dx, �c��dy���o��
	cv::Mat_<double> aabb;
	std::tie(aabb, std::ignore) = build_aabb2d(points);
	double dx = std::abs(aabb(0, 1) - aabb(0, 0));
	double dy = std::abs(aabb(0, 3) - aabb(0, 2));

	// �摜�̃s�N�Z���T�C�Y�͉���1000�ɌŒ肷�� -> �c��int(dy/dx*1000)
	int px = 1000;
	int py = (int)(dy / dx * px);

	// �摜�̌��_[0, 0]�̓_�Q��̍��W
	double cx = aabb(0, 0);
	double cy = aabb(0, 2);

	// �s�N�Z�����ɓ_�������255, �Ȃ����0�̉摜�쐬
	Mat pix = Mat::zeros(py, px, CV_8U);

	Mat_<double> X, Y;
	std::tie(X, Y) = disassemble2d(points);

	for (int i = 0; i < py; i++) {

		if (i % 100 == 0) {
			cout << i << endl;
		}

		for (int j = 0; j < px; j++) {

			// �e�s�N�Z���͈͓̔��ɓ_�Q�����݂��Ă��邩�`�F�b�N
			Mat_<int> x1 = (cx + dx / px * j <= X) / 255;
			Mat_<int> x2 = (X <= cx + dx / px * (j + 1)) / 255;
			Mat_<int> y1 = (cy + dy / py * i <= Y) / 255;
			Mat_<int> y2 = (Y <= cy + dy / py * (i + 1)) / 255;

			Mat_<int> judge = x1.mul(x2);
			judge = judge.mul(y1);
			judge = judge.mul(y2);

			// �z��judge����1��1�ł��������炱�̃s�N�Z����255�ɂ���
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

// �����t�H���W�[���Z�ŗ֊s���o
std::tuple<int, std::vector< cv::Point >> morphology(cv::Mat pix, int dilate_size, int close_size, int open_size, int add_size) {

	// �J�[�l���쐬
	cv::Mat dilate_kernel(dilate_size, dilate_size, CV_8U, cv::Scalar::all(255));
	cv::Mat close_kernel(close_size, close_size, CV_8U, cv::Scalar::all(255));
	cv::Mat open_kernel(open_size, open_size, CV_8U, cv::Scalar::all(255));
	cv::Mat add_kernel(add_size, add_size, CV_8U, cv::Scalar::all(255));

	// �c��
	cv::morphologyEx(pix, pix, MORPH_DILATE, dilate_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/dilate.png", pix);

	// �N���[�W���O
	cv::morphologyEx(pix, pix, MORPH_CLOSE, close_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/close.png", pix);

	// �I�[�v�j���O
	cv::morphologyEx(pix, pix, MORPH_OPEN, open_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/open.png", pix);

	// �c��
	cv::morphologyEx(pix, pix, MORPH_DILATE, add_kernel, cv::Point(-1, -1), 1);
	cv::imwrite("../data/add.png", pix);

	// �֊s�̍��W���X�g
	std::vector< std::vector< cv::Point > > contours;

	// �֊s���o
	cv::findContours(pix, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	// �ʐύő�̗֊s�����o��
	double max_area = 0;
	std::vector< cv::Point > max_contour;

	for (int i = 0; i < contours.size(); i++) {
		double area = cv::contourArea(contours[i]);

		if (area > max_area){
			max_area = area;
			max_contour = contours[i];
		}

	}

	// �֊s�����o���Ȃ��ꍇ�̏���
	if (max_contour.empty()) {
		return std::forward_as_tuple(0, max_contour);
	}

	// �ʕ�̎擾
	std::vector< cv::Point > hull;
	cv::convexHull(max_contour, hull);

	return std::forward_as_tuple(1, hull);
}