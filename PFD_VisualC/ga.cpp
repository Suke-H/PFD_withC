#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "tools2d.h"
#include "figure2d.h"
#include "ga.h"
#include "ga_score.h"

// �̂�class
// �R���X�g���N�^
Person::Person(int fig_type, std::vector<double> p) {
	this->fig_type = fig_type;
	this->p = p;
}

// �̏����o��
void Person::profile() {
	cout << "fig_type: " << fig_type << endl;

	printf("p: [");
	for (int i = 0; i < p.size(); i++) {
		printf("%lf, ", p[i]);
	}
	printf("]\n");

	cout << "score: " << score << "\n"
		<< "score_flag: " << score_flag << endl;
}

// 1��ނ̐}�`�P�̂�GA
//Person single_ga(int fig_type, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area,
//	int n_epoch, int N, int save_num, int tournament_size, int half_reset_num, int all_reset_num) {
//
//	// AABB����
//	cv::Mat_<double> aabb;
//	double l;
//	std::tie(aabb, l) = build_aabb_2d(points);
//
//	// N�l�̐���
//	std::vector<Person> generation = create_random_population(N, fig_type, aabb, l);
//
//	for (int epoch = 0; epoch < n_epoch; epoch++) {
//		cout << "epoch:" << epoch << endl;
//
//		// �����L���O
//		std::tie(generation, std::ignore) = ranking(generation, points, out_points, out_area);
//		// ���save_num�l��ۑ�
//		std::vector<Person> next_generation;
//		std::copy(generation.begin(), generation.begin() + save_num, next_generation.begin());
//
//		// �����オN�l�𒴂��܂ŁA�g�[�i�����g�I�� -> ���� ���J��Ԃ�
//		while (next_generation.size() < N) {
//			// �g�[�i�����g�T�C�Y���̐l���o��
//			std::vector<Person> entry = random_sample_people(generation, tournament_size);
//
//			// �}�`�̎�ނɂ������̂Ƃ��̐e�̌̐��ύX
//			int parent_num;
//			if (fig_type == 0) {
//				parent_num = 4;
//			}
//			else if (fig_type == 1) {
//				parent_num = 5;
//			}
//			else {
//				parent_num = 6;
//			}
//
//			//���parent_num�l�D��
//			std::vector<Person> winners;
//			std::tie(winners, std::ignore) = ranking(entry, points, out_points, out_area);
//
//			// ���҂�����
//			Person child = crossover(winners, fig_type, aabb, l);
//
//			// ������Ɏq��ǉ�
//			next_generation.push_back(child);
//		}
//
//		// ��������������
//		std::copy(next_generation.begin(), next_generation.begin() + N, generation.begin());
//
//		// �r�����ʕ\��
//		if (epoch % 50 == 0) {
//			std::vector<double> score_list;
//			std::tie(std::ignore, score_list) = ranking(generation, points, out_points, out_area);
//
//		}
//	}
//
//	// 1�ʂ��o��
//	std::tie(generation, std::ignore) = ranking(generation, points, out_points, out_area);
//	return generation[0];
//	
//}

// �����̐���
Person create_random_person(int fig_type, cv::Mat_<double> aabb, double l) {

	cout << aabb << "���" << endl;

	std::vector<double> p;

	// �~
	if (fig_type == 0) {
		// ���S���W��AABB�͈͓̔�
		double u_0 = random_value(aabb(0, 0), aabb(0, 1));
		double v_0 = random_value(aabb(0, 2), aabb(0, 3));
		// l/5 < r < 2/3*l
		double r = random_value(l / 5, l * 2 / 3);

		p.push_back(u_0);
		p.push_back(v_0);
		p.push_back(r);
	}

	else if (fig_type == 1) {
		// ���S���W��AABB�͈͓̔�
		double u_0 = random_value(aabb(0, 0), aabb(0, 1));
		double v_0 = random_value(aabb(0, 2), aabb(0, 3));
		// l/5 < r < 2/3*l
		double r = random_value(l / 5, l * 2 / 3);
		// 0 < t < 2/3*pi
		double t = random_value(0, CV_PI * 2 / 3);

		p.push_back(u_0);
		p.push_back(v_0);
		p.push_back(r);
		p.push_back(t);
	}

	else {
		// ���S���W��AABB�͈͓̔�
		double u_0 = random_value(aabb(0, 0), aabb(0, 1));
		double v_0 = random_value(aabb(0, 2), aabb(0, 3));
		// l/5 < w, h < 2/3*l
		double w = random_value(l / 5, l * 2 / 3);
		double h = random_value(l / 5, l * 2 / 3);
		// 0 < t < pi
		double t = random_value(0, CV_PI);

		p.push_back(u_0);
		p.push_back(v_0);
		p.push_back(w);
		p.push_back(h);
		p.push_back(t);
	}

	Person person(fig_type, p);
	return person;
}

// �����W�c����
std::vector<Person> create_random_population(int N, int fig_type, cv::Mat_<double> aabb, double l) {

	std::vector<Person> people;
	people.reserve(N);
	for (int i = 0; i < N; i++) {
		people.push_back(create_random_person(fig_type, aabb, l));
	}

	return people;
}

// std::vector<Person>�^�̔z�񂩂烉���_����size�T���v�����O
std::vector<Person> random_sample_people(std::vector<Person> people, int size) {

	// �����V�[�h������
	srand((unsigned int)time(NULL));

	// v���V���b�t������
	for (int i = 0; i < people.size(); i++) {
		int j = rand() % people.size();
		Person p = people[i];
		people[i] = people[j];
		people[j] = p;
	}

	// v[0�`size]�����o��
	std::vector<Person> w;
	std::copy(people.begin(), people.begin() + size, w.begin());

	return w;
}

// std::vector<Person>�^�̔z����C���f�b�N�X���ɕ��ѕς���
std::vector<Person> sort_people(std::vector<Person> people, std::vector<int> indices) {
	
	std::vector<Person> sorted_people;

	for (int i = 0; i < people.size(); i++) {
		sorted_people.push_back(people[indices[i]]);
	}

	return sorted_people;
}

// �X�R�A�Z�o
Person scoring(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {
	// �X�R�A�t���O�������ĂȂ�������X�R�A�Z�o���āA�X�R�A�t���O�𗧂Ă�
	if (!person.score_flag) {
		person.score = calc_score(person, points, out_points, out_area);
		person.score_flag = 1;
	}

	return person;
}

// �̂������N�t�����ă\�[�g
std::tuple<std::vector<Person>, std::vector<double>> ranking(
	std::vector<Person> people, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {

	// people���X�R�A�t�����Ă���
	int N = people.size();
	for (int i = 0; i < N; i++) {
		people[i] = scoring(people[i], points, out_points, out_area);
	}

	// �X�R�A���L�^���Ă���
	std::vector<double> score_list;
	score_list.reserve(N);
	for (int i = 0; i < N; i++) {
		score_list.push_back(people[i].score);
	}

	// �\�[�g
	// �z��̃C���f�b�N�X indices = {0, 1, 2, ...} ���쐬����B
	std::vector<int> indices;
	for (int i = 0; i < N; i++) {
		indices.push_back(i);
	}

	printf("score_list: ");
	for (int i = 0; i < N; i++) {
		printf("%lf, ", score_list[i]);
	}
	printf("\n");

	// score_list���~���\�[�g�����Ƃ��̃C���f�b�N�X��indices��Ή�������
	std::sort(indices.begin(), indices.end(), [&score_list](int i1, int i2) {
		return score_list[i1] > score_list[i2];
	});

	printf("indices: ");
	for (int i = 0; i < indices.size(); i++) {
		printf("%d, ", indices[i]);
	}
	printf("\n");

	// score_list�~���\�[�g
	std::sort(score_list.begin(), score_list.end(), std::greater<int>());

	// �X�R�A�ɂ��people���\�[�g���ĕԂ�
	// score_list���Ԃ�
	return std::forward_as_tuple(sort_people(people, indices), score_list);

}