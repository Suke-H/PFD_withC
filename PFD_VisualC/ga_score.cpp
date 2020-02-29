#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "ga.h"
#include "tools2d.h"
#include "figure2d.h"

// �X�R�A�Z�o�̑O�̏����`�F�b�N
// aabb���ɒ��S���W�����邩�E�}�`�����������Ȃ���
int check_condition(Person person, cv::Mat_<double> points, cv::Mat_<double> aabb, double l) {

	double umin = aabb(0, 0);
	double umax = aabb(0, 1);
	double vmin = aabb(0, 2);
	double vmax = aabb(0, 3);

	double u_0, v_0, r, w, h;

	// �~
	if (person.fig_type == 0) {
		u_0 = person.p[0], v_0 = person.p[1], r = person.p[2];
		// �_�~�[
		w = l / 2, h = l / 2;
		
	}

	// ���O�p�`
	else if (person.fig_type == 1) {
		u_0 = person.p[0], v_0 = person.p[1], r = person.p[2];
		// �_�~�[
		w = l / 2, h = l / 2;

	}

	// �����`
	else {
		u_0 = person.p[0], v_0 = person.p[1], w = person.p[2], h = person.p[3];
		// �_�~�[
		r = l / 2;

	}

	//aabb���ɒ��S���W������E�}�`�����������Ȃ��Ƃ�1��Ԃ�
	if ((umin < u_0 && u_0 < umax) && (vmin < v_0 && v_0 < vmax) && (l / 5 < r && r < l) && 
		(l / 5 < w && w < l) && (l / 5 < h && h < l)) {
		return 1;
	}

	// �����𖞂����Ȃ����0��Ԃ�
	else {
		return 0;
	}
}

// �X�R�A���Z�o
// score = Cin/Ain - Cout/Aout
double calc_score(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {

	// AABB����
	cv::Mat_<double> aabb;
	double l;
	std::tie(aabb, l) = build_aabb2d(points);

	// �����𖞂����Ȃ���Α匸�_
	if (!check_condition(person, points, aabb, l)) {
		return -100;
	}

	// in�̖��x�v�Z
	// W >= 0(-�}�`�̓���)�̃C���f�b�N�X�����o���A���̐���c_in�Ƃ��ĕۑ�
	// �}�`�̖ʐς�a_in�Ƃ���
	cv::Mat_<double> w, u, v;
	double a_in;
	std::tie(u, v) = disassemble2d(points);

	// �~
	if (person.fig_type == 0) {
		Circle cir(person.p[0], person.p[1], person.p[2]);
		w = cir.f_rep_list(u, v);
		a_in = cir.area;

	}

	// ���O�p�`
	else if (person.fig_type == 1) {
		Triangle tri(person.p[0], person.p[1], person.p[2], person.p[3]);
		w = tri.f_rep_list(u, v);
		a_in = tri.area;

	}

	// �����`
	else {
		Rectangle rect(person.p[0], person.p[1], person.p[2], person.p[3], person.p[4]);
		w = rect.f_rep_list(u, v);
		a_in = rect.area;

	}	

	// |f(u, v)| >= 0�𖞂����s��255�A����ȊO��0�ɂȂ�
	cv::Mat_<int> w_bool = w >= 0;

	// W >= 0�̐���Cin�Ƃ��ĕۑ�
	int c_in = cv::countNonZero(w_bool);

	// in�̖��x�v�Z
	// c_out = �S�_�Q�� - c_in
	int c_out = points.rows - c_in;
	// a_out = �֊s�̖ʐ�
	int a_out = out_area;

	// score = Cin/Ain - Cout/Aout ���o��
	return c_in / a_in - c_out / a_out;
}