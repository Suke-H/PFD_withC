#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools.h"
#include "figure.h"

// ���ʂɃt�B�b�g�����_�𕽖ʏ�ɂ҂��������悤�Ɉړ�������
cv::Mat_<double> put_points_on_plane(cv::Mat_<double> points, std::vector<double> plane_p) {

	// ���ʂ̃N���X�쐬
	Plane plane(plane_p[0], plane_p[1], plane_p[2], plane_p[3]);
	std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>)> f =
		std::bind(&Plane::f_rep_list, &plane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_1);

	// ���ʂ̖@��
	cv::Mat_<double> n = (cv::Mat_<double>(1, 3) << plane_p[0], plane_p[1], plane_p[2]);
	double a = n(0, 0), b = n(0, 1), c = n(0, 2);

	// 3�����_�Q��X, Y, Z�ɕ���
	cv::Mat_<double> X = points.col(0);
	cv::Mat_<double> Y = points.col(1);
	cv::Mat_<double> Z = points.col(2);
	cv::transpose(X, X);
	cv::transpose(Y, Y);
	cv::transpose(Z, Z); 

	// �@�������ɓ_�𓮂����ĕ��ʂɗ��Ƃ�����
	// f(p0 + t n) = 0 <=> t = f(p0) / (a ^ 2 + b ^ 2 + c ^ 2)
	cv::Mat_<double> t = f(X, Y, Z) / (pow(a, 2) + pow(b, 2) + pow(c, 2));

	// �傫��t��n�����ɓ_�Q�𓮂���
	// p = p0 + t n

	int N = t.cols;
	cv::Mat_<double> tn = cv::Mat_<double>::zeros(N, 3);

	for (int i = 0; i < N; i++) {
		tn.row(i) = t(0, i) * n;
	}
	
	return points + tn;
}

// ���ʂɃt�B�b�g�����_�Q�ƕ��ʃp�����[�^����͂ɁA�_�Q�𕽖ʏ��2�����_�Q�Ƃ��ē��e����
std::tuple<cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>> 
	projection2d(cv::Mat_<double> points3d, std::vector<double> plane_p) {

	// ���ʂ̖@��
	cv::Mat_<double> n = (cv::Mat_<double>(1, 3) << plane_p[0], plane_p[1], plane_p[2]);

	// �V�������_��K���ɑI��1�_�ɂ���
	cv::Mat_<double> o = (cv::Mat_<double>(1, 3) << points3d(0, 0), points3d(0, 1), points3d(0, 2));

	// ������_�I��
	cv::Mat_<double> p = (cv::Mat_<double>(1, 3) << points3d(1, 0), points3d(1, 1), points3d(1, 2));

	// �񎟌����W��u, v���`
	cv::Mat_<double> u, v;
	cv::normalize(p - o, u);
	cv::normalize(u.cross(n), v);

	// [o, o, .., o]�Ƃ����`��mat�쐬
	points_t o_list;
	o_list.reserve(points3d.rows);
	for (int i = 0; i < points3d.rows; i++) {
		point_t o_point(o(0, 0), o(0, 1), o(0, 2));
		o_list.push_back(o_point);
	}

	cv::Mat_<double> o_mat = vec3_to_mat(o_list);

	// �񎟌����W�ɕϊ�
	cv::Mat_<double> points2d;
	cv::Mat_<double> uv = (cv::Mat_<double>(3, 2) << u(0, 0), v(0, 0), u(0, 1), v(0, 1), u(0, 2), v(0, 2));

	points2d = (points3d - o_mat) * uv;

	return std::forward_as_tuple(points2d, u, v, o);
	
}

// ���ʏ��2�����_�Q(���łɐ}�`�̒��S���W��)��3�����Ɏˉe����
std::tuple < cv::Mat_<double>, cv::Mat_<double>> projection3d(cv::Mat_<double> points2d, std::vector<double> fig_p,
	cv::Mat_<double> u, cv::Mat_<double> v, cv::Mat_<double> o) {

	// [o, o, .., o]�Ƃ����`��mat�쐬
	points_t o_list;
	o_list.reserve(points2d.rows);
	for (int i = 0; i < points2d.rows; i++) {
		point_t o_point( o(0, 0), o(0, 1), o(0, 2) );
		o_list.push_back(o_point);
	}

	cv::Mat_<double> o_mat = vec3_to_mat(o_list);

	// 2�����_�Q��3�����Ɏˉe
	cv::Mat_<double> uv = (cv::Mat_<double>(2, 3) << u(0, 0), u(0, 1), u(0, 2), v(0, 0),  v(0, 1), v(0, 2));
	cv::Mat_<double> points3d = points2d * uv + o_mat;

	// �}�`�̒��S���W
	cv::Mat_<double> center2d = ( cv::Mat_<double>(1, 2) << fig_p[0], fig_p[1] );

	// ���S���W��3�����Ɏˉe
	cv::Mat_<double> center3d = center2d * uv + o;

	return std::forward_as_tuple(points3d, center3d);
}