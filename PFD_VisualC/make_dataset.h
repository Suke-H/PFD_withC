#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// �����Ȏ�ނ̕W����f-rep�֐����Z�b�g
//sign_type: �W��
// 0:        ���a0.3m�̉~
// 1:        1��0.8m�̐��O�p�`
// 2:        1��0.9m�̐����`
// 3.        1��0.45m�̂Ђ��`(�����`)
// �{��(scale)��2/3, 1, 1.5, 2��4��ނ���
std::tuple<int, std::vector<double>, cv::Mat_<double>> set_sign2d(int sign_type, int scale);

// �}�`�_�Q�{�m�C�Y�����킹���_�Q�A���ʃp�����[�^�A���̑�����}�`�p�����[�^�����o��
// 1. �}�`�̓_�Q���͓_�Q���xdensity(/m^2)�Ɋ�Â�(�}�`�_�Q��=density*�}�`�̖ʐ�),
// 2. noise_rate�͑S�_�Q���ɑ΂���m�C�Y�_�Q�̊����B�m�C�Y�͐}�`�_�Q��AABB��1~1.5�{����AABB���Ɉ�l�����Ɋ�Â��č쐬
std::tuple<cv::Mat_<double>, std::vector<double>, std::vector<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>>
	make_sign3d(int sign_type, int scale, double density, double low=-100, double high=100, int grid_num=50);

// �����_���ɕ��ʍ쐬
std::vector<double> random_plane(int high = 1000);