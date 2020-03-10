#include <iostream>
#include <opencv2/opencv.hpp>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools.h"
#include "extract_contour.h"

// �֊s���o
std::tuple<int, cv::Mat_<double>, cv::Mat_<double>, double> extract_contour(cv::Mat_<double> points, int dilate_size, int close_size, int open_size, int add_size) {

	// �_�Q->�摜
	cv::Mat pix;
	double dx, dy, cx, cy;
	int px, py;
	std::tie(pix, dx, dy, px, py, cx, cy) = trans_pix(points);

	// �摜���烂���t�H���W�[�𗘗p���āA�̈�ʐύő�̗֊s�_���o
	int ret;
	std::vector< cv::Point > contour;
	double pix_area;
	std::tie(ret, contour, pix_area) = morphology(pix, dilate_size, close_size, open_size, add_size);

	// �摜��ł̐}�`�̖ʐ�
	double area = pix_area * (dx*dy) / (px*py);
	cout << "area: " << area << endl;

	// �����t�H���W�[���Z�Ɏ��s�����ꍇ�̏���
	if (!ret) {
		return std::forward_as_tuple(0, cv::Mat_<double>::zeros(1, 1), cv::Mat_<double>::zeros(1, 1), 0);
	}

	// �摜->�_�Q
	cv::Mat_<double> contour_mat = trans_points(contour, dx, dy, px, py, cx, cy);

	// �֊s�̖ʐ�
	/*std::vector< cv::Point > contour_points = mat_to_vec2(contour_mat);
	double area = cv::contourArea(contour_points);*/

	// �֊s�̓��O����
	std::vector<int> in_list;
	for (int i = 0; i < points.rows; i++) {
		if (inout_judgement(points(i, 0), points(i, 1), contour_mat)) {
			in_list.push_back(i);
		}
	}

	// �֊s���ɂ���_�������o
	cv::Mat_<double> inside_points = extract_rows(points, in_list);

	return std::forward_as_tuple(1, inside_points, contour_mat, area);

}


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

		if (i % 200 == 0) {
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
			}
		}
	}

	cv::imwrite("../data/test.png", pix);

	return std::forward_as_tuple(pix, dx, dy, px, py, cx, cy);
}

// �����t�H���W�[���Z�ŗ֊s���o
std::tuple<int, std::vector< cv::Point >, double> morphology(cv::Mat pix, int dilate_size, int close_size, int open_size, int add_size) {

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
		return std::forward_as_tuple(0, max_contour, 0);
	}

	// �ʕ�̎擾
	std::vector< cv::Point > hull;
	cv::convexHull(max_contour, hull);

	return std::forward_as_tuple(1, hull, max_area);
}

// �摜���W�̗֊s�_��_�Q���W�ɕϊ�
cv::Mat_<double> trans_points(std::vector< cv::Point >  contour, double dx, double dy, int px, int py, double cx, double cy) {

	// �֊s�_�̐�
	int N = contour.size();
	// �o�͂���_�Q
	Mat_<double> points = cv::Mat_<double>::zeros(N, 2);

	// �摜���W�̗֊s�_��_�Q���W�ɕϊ�
	for (int i = 0; i < N; i++) {
		points(i, 0) = cx + dx / px * (2 * contour[i].x + 1) / 2;
		points(i, 1) = cy + dy / py * (2 * contour[i].y + 1) / 2;
	}

	return points;

}


// �_p���֊s�_contour���ɂ��邩�̔���
int inout_judgement(double px, double py, cv::Mat_<double> contour) {

	// ��������
	int cross_count = 0;

	// �֊s�_�̐�
	int N = contour.rows;

	for (int i1 = 0; i1 < N; i1++) {

		int i2 = (i1 + 1) % N;

		// ��ab��a��b�̍��W��`
		double ax = contour(i1, 0), ay = contour(i1, 1), bx = contour(i2, 0), by = contour(i2, 1);

		// ���[��1, 2, 3
		if (((ay <= py) && (by > py)) || ((ay > py) && (by <= py))) {

			// ���[��4: cx > px�Ȃ��������(cross_count�𑝂₷)
			double cx = (py * (ax - bx) + ay * bx - ax * by) / (ay - by);
			if (cx > px) {
				cross_count++;
			}
		}
	}

	// �������������Ȃ�O�A��Ȃ��
	if (cross_count % 2 == 0) {
		return 0;
	}

	else {
		return 1;
	}
}