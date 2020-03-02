#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// �̂�class
class Person
{
public:
	// �}�`�̎��
	int fig_type;
	// �}�`�p�����[�^
	std::vector<double> p;
	// �X�R�A
	double score = 0;
	// �X�R�A���Z�o���ꂽ�痧�t���O
	int score_flag = 0;

	// �R���X�g���N�^
	Person(int fig_type, std::vector<double> p);

	// �R�s�[�R���X�g���N�^
	//Person(const Person& person);

	// �̏����o��
	void profile();
};

//1��ނ̐}�`�P�̂�GA
Person single_ga(int fig_type, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area,
	int n_epoch = 300, int N = 100, int save_num = 5, int tournament_size = 20, 
	int half_reset_num = 15, int all_reset_num = 9);

// 3��ނ̐}�`�P��GA���񂵂ăX�R�A�ő�̐}�`��I��
//Person entire_ga(cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area);

// �����̐���
Person create_random_person(int fig_type, cv::Mat_<double> aabb, double l);

// �����W�c����
std::vector<Person> create_random_population(int N, int fig_type, cv::Mat_<double> aabb, double l);

// std::vector<Person>�^�̔z�񂩂烉���_����size�T���v�����O
// rand_min�`rand_max�͎��o���C���f�b�N�X�͈̔�(��{��rand_min=0, rand_max=size-1)
// (size >= rand_min�`rand_max�Ԃ̐��ł��邱�Ƃ�����)
std::vector<Person> random_sample_people(std::vector<Person> people, int size, int rand_min, int rand_max);

// std::vector<Person>�^�̔z����C���f�b�N�X���ɕ��ѕς���
std::vector<Person> sort_people(std::vector<Person> people, std::vector<int> indices);

// �X�R�A�Z�o
Person scoring(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area);

// �̂������N�t�����ă\�[�g
std::tuple<std::vector<Person>, std::vector<double>> ranking(
	std::vector<Person> people, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area);

// ����
Person crossover(std::vector<Person> parents, int fig_type, cv::Mat_<double> aabb, double l);

// �̕`��
void draw_person(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, cv::Mat_<double> aabb);