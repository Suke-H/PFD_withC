#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "ga.h"

// �X�R�A�Z�o�̑O�̏����`�F�b�N
// aabb���ɒ��S���W�����邩�E�}�`�����������Ȃ���
int check_condition(Person person, cv::Mat_<double> points, cv::Mat_<double> aabb, double l);

// �X�R�A���Z�o
// score = Cin/Ain - Cout/Aout
double calc_score(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area, int flag = 0);