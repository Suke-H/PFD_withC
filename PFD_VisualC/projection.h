#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ���ʂɃt�B�b�g�����_�𕽖ʏ�ɂ҂��������悤�Ɉړ�������
cv::Mat_<double> put_points_on_plane(cv::Mat_<double> points, std::vector<double> plane_p);

// ���ʂɃt�B�b�g�����_�Q�ƕ��ʃp�����[�^����͂ɁA�_�Q�𕽖ʏ��2�����_�Q�Ƃ��ē��e����
std::tuple<cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>, cv::Mat_<double>> 
	projection2d(cv::Mat_<double> points3d, std::vector<double> plane_p);

// ���ʏ��2�����_�Q(���łɐ}�`�̒��S���W��)��3�����Ɏˉe����
std::tuple < cv::Mat_<double>, cv::Mat_<double>> projection3d(cv::Mat_<double> points2d, std::vector<double> fig_p, 
	cv::Mat_<double> u, cv::Mat_<double> v, cv::Mat_<double> o);

