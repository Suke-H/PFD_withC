#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "make_dataset.h"
#include "tools.h"
#include "figure.h"
#include "projection.h"

//sign_type: �W��
// 0:        ���a0.3m�̉~
// 1:        1��0.8m�̐��O�p�`
// 2:        1��0.9m�̐����`
// 3.        1��0.45m�̂Ђ��`(�����`)
// �{��(scale)��2/3, 1, 1.5, 2��4��ނ���
// �����Ȏ�ނ̕W����f-rep�֐����Z�b�g
std::tuple<int, std::vector<double>, cv::Mat_<double>> set_sign2d(int sign_type, int scale) {

	int fig_type;
	std::vector<double> fig_p;
	cv::Mat_<double> aabb;

	// 0: ���a0.3m�̉~
	if (sign_type == 0) {

		// �~
		fig_type = 0;
		// p = [0, 0, 0.3*scale]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.3*scale);
		// �`��p��AABB
		aabb = cv::Mat_<double>{ -0.35, 0.35, -0.35, 0.35 }*scale;
	}

	// 1: 1��0.8m�̐��O�p�`
	if (sign_type == 1) {

		// ���O�p�`
		fig_type = 1;
		// p = [0, 0, 0.8/��3*scale, 0�`pi*3/2]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.8/sqrt(3)*scale);
		fig_p.push_back(random_value(0, CV_PI*3/2));
		// �`��p��AABB
		aabb = cv::Mat_<double>{ -0.45, 0.45, -0.45, 0.45 }*scale;
	}

	// 2: 1��0.9m�̐����`
	if (sign_type == 2) {

		// �����`
		fig_type = 2;
		// p = [0, 0, 0.9*scale, 0.9*scale, 0�`pi/2]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.9*scale);
		fig_p.push_back(0.9*scale);
		fig_p.push_back(random_value(0, CV_PI / 2));
		// �`��p��AABB
		double l = 0.9*sqrt(2) / 2;
		aabb = cv::Mat_<double>{ -l * 1.1, l*1.1, -l * 1.1, l*1.1 }*scale;
	}

	// 3. 1��0.45m�̂Ђ��`(�����`)
	if (sign_type == 3) {

		// �����`
		fig_type = 2;
		// p = [0, 0, 0.45*scale, 0.45*scale, 0�`pi/2]
		fig_p.push_back(0);
		fig_p.push_back(0);
		fig_p.push_back(0.45*scale);
		fig_p.push_back(0.45*scale);
		fig_p.push_back(random_value(0, CV_PI / 2));
		// �`��p��AABB
		double l = 0.45*sqrt(2) / 2;
		aabb = cv::Mat_<double>{ -l * 1.1, l*1.1, -l * 1.1, l*1.1 }*scale;
	}

	return std::forward_as_tuple(fig_type, fig_p, aabb);
}

// 1. �}�`�̓_�Q���͓_�Q���xdensity(/m^2)�Ɋ�Â�(�}�`�_�Q��=density*�}�`�̖ʐ�),
// 2. noise_rate�͑S�_�Q���ɑ΂���m�C�Y�_�Q�̊����B�m�C�Y�͐}�`�_�Q��AABB��1~1.5�{����AABB���Ɉ�l�����Ɋ�Â��č쐬

// �}�`�_�Q�{�m�C�Y�����킹���_�Q�A���ʃp�����[�^�A���̑�����}�`�p�����[�^�����o��
std::tuple<cv::Mat_<double>, std::vector<double>, std::vector<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>>
	make_sign3d(int sign_type, int scale, double density, double low, double high, int grid_num) {

	/////  �}�`�_�Q�쐬  /////

	// ���ʐ}�`�ݒ�
	int fig_type;
	std::vector<double> fig_p;
	cv::Mat_<double> aabb2d;
	std::tie(fig_type, fig_p, aabb2d) = set_sign2d(sign_type, scale);

	// �}�`�_�Q��
	int fig_size;

	// �}�`�_�Q�쐬
	cv::Mat_<double> fig_points;

	// �~
	if (fig_type == 0) {
		Circle figure(fig_p[0], fig_p[1], fig_p[2]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Circle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);

		fig_size = (int)density * figure.area;
		fig_points = make_inside(f, aabb2d, fig_size, 1, grid_num);

	}

	// ���O�p�`
	else if (fig_type == 1) {
		Triangle figure(fig_p[0], fig_p[1], fig_p[2], fig_p[3]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Triangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);

		fig_size = (int)density * figure.area;
		fig_points = make_inside(f, aabb2d, fig_size, 1, grid_num);
	}

	// �����`
	else {
		Rectangle figure(fig_p[0], fig_p[1], fig_p[2], fig_p[3], fig_p[4]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Rectangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);

		fig_size = (int)density * figure.area;
		fig_points = make_inside(f, aabb2d, fig_size, 1, grid_num);
	}

	/////  ���ʂ�2D�_�Q���ˉe  /////

	// ���ʃ����_���쐬
	std::vector<double> plane_p = random_plane();

	// ���ʏ��2�_o, p�������_���ɒ�߂�
	double a = plane_p[0], b = plane_p[1], c = plane_p[2], d = plane_p[3];
	double ox = random_value(low, high), oy = random_value(low, high), px = random_value(low, high), py = random_value(low, high);
	double oz = (d - a * ox - b * oy) / c, pz = (d - a * px - b * py) / c;
	cv::Mat_<double> o = (cv::Mat_<double>(1, 3) << ox, oy, oz);
	cv::Mat_<double> p = (cv::Mat_<double>(1, 3) << px, py, pz);

	// ���ʂ̖@��n, �񎟌����W��u, v���`
	cv::Mat_<double> n, u, v;
	n = (cv::Mat_<double>(1, 3) << a, b, c);
	cv::normalize(p - o, u);
	cv::normalize(u.cross(n), v);

	// �}�`�_�Q�Ɛ}�`�̒��S���W�𕽖ʂ�3D�ˉe
	cv::Mat_<double> points3d, center3d;
	std::tie(points3d, center3d) = projection3d(fig_points, fig_p, u, v, o);

	return std::forward_as_tuple(points3d, plane_p, fig_p, center3d, u, v, o, aabb2d);

}

// �����_���ɕ��ʍ쐬
std::vector<double> random_plane(int high) {

	// �@���쐬
	cv::Mat_<double> n = cv::Mat_<double>{ 0, 0, 0 };

	while (cv::norm(n) == 0) {
		n(0, 0) = random_value(-high, high);
		n(0, 1) = random_value(-high, high);
		n(0, 2) = random_value(-high, high);
	}

	// ���K��
	cv::normalize(n, n);

	// d�쐬
	double d = random_value(-high, high);

	// ���ʃp�����[�^
	std::vector<double> plane_p{ n(0, 0), n(0, 1), n(0, 2), d };

	return plane_p;
}