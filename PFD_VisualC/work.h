#pragma once

// ���ۂ̓_�Q�f�[�^����͂ɐ}�`���o
void test(cv::Mat_<double> points3d, std::vector<double> plane_p, int dilate_size, int close_size, int open_size, int add_size);

// �V�~�����[�V�����ō쐬�����_�Q�f�[�^����͂ɐ}�`���o
void simulation(int sign_type, int scale, double density, int dilate_size, int close_size, int open_size, int add_size);