#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// points_t�^��cv::Mat�^�ɕϊ�
cv::Mat_<double> vec3_to_mat(points_t points);

// cv::Mat�^��std::vector< cv::Point >�^�ɕϊ�
std::vector< cv::Point > mat_to_vec2(cv::Mat_<double> mat);

// std::vector<double>�^�̔z���size���v�����g����
void print_vec_double(std::vector<double> v, int size, std::string name);

// low�`high�͈̔͂ň�l�����Ɋ�Â��Ď�����Ԃ�
double random_value(double low, double high);

//[x1, y1]         [x1, x2, ..., xn]
//    :       ->   [y1, y2, ..., yn]
//[xn, yn] 
// Mat_<double>�^��2�����_�Q��x, y�ɕ���
std::tuple<cv::Mat_<double>, cv::Mat_<double>> disassemble2d(cv::Mat_<double> points);

// Mat_<double>�^��x, y��2�����_�Q�ɍ���
cv::Mat_<double> composition2d(cv::Mat_<double> u, cv::Mat_<double> v);

// points��AABB���쐬
std::tuple<cv::Mat_<double>, double> build_aabb2d(cv::Mat_<double> points);

// ����start, �I��stop, ����̐�num�̓������񂪓������z����o��
cv::Mat_<double> linspace(double start, double stop, int num);

// u = [1,2,3], v = [4,5] -> uu = [1,2,3,1,2,3], vv = [4,4,4,5,5,5]
// u, v�̔z����O���b�h�^�ɂ���
std::tuple<cv::Mat_<double>, cv::Mat_<double>> meshgrid2d(cv::Mat_<double> U, cv::Mat_<double> V);

// std::vector<int>�^�̔z��v��x�����݂��邩�`�F�b�N����
int vec_search(std::vector<int> v, int x);

// std::vector<int>�^�̔z��v���烉���_����size�T���v�����O
std::vector<int> random_sample(std::vector<int> v, int size);

// row_list�Ŏw�肳�ꂽ�s��mat_in����폜����
cv::Mat_<double> delete_rows(cv::Mat_<double> mat_in, std::vector<int> row_list);

// delete_rows()�̋t�ŁArow_list�Ŏw�肳�ꂽ�s��mat_in�Ɏc���A�c����폜����
cv::Mat_<double> extract_rows(cv::Mat_<double> mat_in, std::vector<int> row_list);

// �}�`�̗֊s��`�悷�邽�߂ɁAf-rep�֐�����֊s�̓_�Q���쐬
cv::Mat_<double> make_contour(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, 
	double aabb_size=1.5, int grid_num=1000, double epsilon=0.01);

// �}�`�̈������`�悷�邽�߂ɁAf-rep�֐���������̓_�Q���쐬(make_contour�֐��ƈႢ�A�o�͂���_�Q��N����͂Ɏ��)
cv::Mat_<double> make_inside(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, int N,
	double aabb_size = 1.5, int grid_num = 200);