#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <matplotlibcpp.h>
#include <random>

namespace plt = matplotlibcpp;
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

// �R�s�[�R���X�g���N�^
//Person::Person(const Person& person){
//
//	fig_type = person.fig_type;
//	
//	for (int i = 0; i < person.p.size(); i++) {
//		p.push_back(person.p[i]);
//	}
//
//	cout << "�R�s�[�R���X�g���N�^���Ă΂�܂����B" << endl;
//}

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
Person single_ga(int fig_type, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area,
	int n_epoch, int N, int save_num, int tournament_size, int half_reset_num, int all_reset_num) {

	// AABB����
	cv::Mat_<double> aabb;
	double l;
	std::tie(aabb, l) = build_aabb2d(points);

	// ���������p��RESET�w�����`
	int half_n = 0;
	int all_n = 0;

	// �O�����1�ʂ̃X�R�A���i�[����ϐ�
	double prev_score = 0;

	// �S�����O��1�ʂ�ۑ����Ă����z��
	std::vector<Person> records;
	std::vector<double> record_score;


	// N�l�̐���
	std::vector<Person> generation = create_random_population(N, fig_type, aabb, l);

	// GA�J�n
	for (int epoch = 0; epoch < n_epoch; epoch++) {
		cout << "epoch: " << epoch << endl;

		// �����L���O
		std::tie(generation, std::ignore) = ranking(generation, points, out_points, out_area);

		// ���save_num�l��ۑ�
		std::vector<Person> next_generation;
		/*next_generation.reserve(save_num);
		std::copy(generation.begin(), generation.begin() + save_num, next_generation.begin());*/
		for (int i = 0; i < save_num; i++) {
			next_generation.push_back(generation[i]);
		}

		// �����オN�l�𒴂��܂ŁA�g�[�i�����g�I�� -> ���� ���J��Ԃ�
		while (next_generation.size() < N) {

			// �g�[�i�����g�T�C�Y���̐l���o��
			std::vector<Person> entry = random_sample_people(generation, tournament_size, 0, tournament_size-1);

			// �}�`�̎�ނɂ������̂Ƃ��̐e�̌̐��ύX
			int parent_num;
			if (fig_type == 0) {
				parent_num = 4;
			}
			else if (fig_type == 1) {
				parent_num = 5;
			}
			else {
				parent_num = 6;
			}

			// ���parent_num�l�D��
			std::tie(entry, std::ignore) = ranking(entry, points, out_points, out_area);
			std::vector<Person> winners;
			for (int i = 0; i < parent_num; i++) {
				winners.push_back(entry[i]);
			}

			// ���҂��������Ď�����ɒǉ�
			next_generation.push_back(crossover(winners, fig_type, aabb, l));
		}

		// �������������Ɉڂ�
		std::copy(next_generation.begin(), next_generation.begin() + N, generation.begin());
		/*for (int i = 0; i < N; i++) {
			generation[i] = next_generation[i];
		}*/

		// �ȍ~�ARESET����
		std::vector<double> score_list;
		std::tie(generation, score_list) = ranking(generation, points, out_points, out_area);

		// �������1�ʂ̃X�R�A�ۑ�
		double current_score = score_list[0];

		std::cout << prev_score << " vs " << current_score << endl;

		// �X�R�A���O����ƕς��Ȃ��悤�Ȃ�half_n�𑝂₷
		if (prev_score >= current_score) {
			half_n++;
		}
		// �X�R�A������������half_n�����Z�b�g
		else {
			half_n = 0;
		}

		// half_n����߂��l(=half_reset_num)�ɒB������all_n�𑝂₵�Ahalf_n�̓��Z�b�g
		if (half_n == half_reset_num) {
			all_n++;
			half_n = 0;

			// all_n����߂��l(=all_reset_num)�ɒB������1�ʂ��L�^���đS���������Aall_n�̓��Z�b�g
			if (all_n == all_reset_num) {
				records.push_back(generation[0]);
				record_score.push_back(generation[0].score);
				generation = create_random_population(N, fig_type, aabb, l);
				all_n = 0;
			}

			// all_n���B���Ă��Ȃ�������1�ʂ�������������
			else {
				// �����̏����W�c����
				int half_size = N / 2;
				std::vector<Person> people = create_random_population(N/2, fig_type, aabb, l);
				// �����ォ��1��+�����𒊏o
				int sample_size = N - half_size;
				Person best(generation[0]);
				generation = random_sample_people(generation, sample_size - 1, 1, sample_size - 1);
				generation.push_back(best);
				// ����
				generation.reserve(generation.size() + people.size());
				std::copy(people.begin(), people.end(), std::back_inserter(generation));
			}

		}

		// ���݂̃X�R�A��O�̃X�R�A�Ƃ��āA�I���
		prev_score = current_score;

		std::cout << "(" << half_n << ", " << all_n << ")" << endl;

		// �r�����ʕ\��
		if (epoch % 10 == 0) {
			std::vector<double> score_list;
			std::tie(std::ignore, score_list) = ranking(generation, points, out_points, out_area);
			printf("score: ");
			for (int i = 0; i < 10; i++) {
				printf("%lf, ", score_list[i]);
			}
			printf("best: {");
			for (int i = 0; i < generation[0].p.size(); i++) {
				printf("%lf, ", generation[0].p[i]);
			}
			printf("}\n");
			//draw_person(generation[0], points, out_points, aabb);

		}
	}

	// 1�ʂ��o��
	std::tie(generation, std::ignore) = ranking(generation, points, out_points, out_area);

	printf("gene1: {");
	for (int i = 0; i < generation[0].p.size(); i++) {
		printf("%lf, ", generation[0].p[i]);
	}
	printf("}\n");

	cout << "���Ə���" << endl;

	// �L�^�����̂̒�������1�ʂ����߂�
	std::vector<double>::iterator iter = std::max_element(record_score.begin(), record_score.end());
	size_t index = std::distance(record_score.begin(), iter);

	printf("kako1: {");
	for (int i = 0; i < records[index].p.size(); i++) {
		printf("%lf, ", records[index].p[i]);
	}
	printf("}\n");
	
	// �ŏI�����1�ʂƋL�^����1�ʂ��r���Ă���
	std::vector<double> best_p;
	for (int i = 0; i < generation[0].p.size(); i++) {
		best_p.push_back(generation[0].p[i]);
	}

	std::cout << generation[0].score << " vs " << record_score[index] << endl;
	if (generation[0].score < record_score[index]) {
		for (int i = 0; i < records[index].p.size(); i++) {
			best_p[i] = (records[index].p[i]);
		}
	}

	Person best(fig_type, best_p);

	// 1�ʂ��o��
	printf("best: {");
	for (int i = 0; i < best.p.size(); i++) {
		printf("%lf, ", best.p[i]);
	}
	printf("}\n");
	draw_person(best, points, out_points, aabb);
	return best;
	
}

// �����̐���
Person create_random_person(int fig_type, cv::Mat_<double> aabb, double l) {

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
// rand_min�`rand_max�͎��o���C���f�b�N�X�͈̔�(��{��rand_min=0, rand_max=size-1)
// (size >= rand_min�`rand_max�Ԃ̐��ł��邱�Ƃ�����)
std::vector<Person> random_sample_people(std::vector<Person> people, int size, int rand_min, int rand_max) {

	// �Ȍ�Arand_min�`rand_max�̐������烉���_����"�d���Ȃ�"��size�Ƃ����z��re�𐶐�
	std::vector<int> re(size);

	// rand_min�`rand_max�̐����̈�l����
	std::uniform_int_distribution<int> rand(rand_min, rand_max);
	// �񌈒�I�ȗ���������𐶐�
	std::random_device rnd;
	//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	auto mt = std::mt19937(rnd());

	for (auto& i : re) {
		i = rand(mt);
	}
	bool is_all_no_conflict = false;
	do {
		is_all_no_conflict = true;
		for (auto j = re.begin(); j != re.end(); ++j) {
			for (auto k = j + 1; k != re.end(); ++k) {
				if (*k == *j) {
					*k = rand(mt);
					is_all_no_conflict = false;
				}
			}
		}
	} while (!is_all_no_conflict);

	// �쐬�����z��̗v�f���C���f�b�N�X�Ƃ��ĕ��ёւ�
	std::vector<Person> w;

	for (int i = 0; i < size; i++) {
		Person person(people[re[i]]);
		w.push_back(person);
	}

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
	size_t N = people.size();
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

	// score_list���~���\�[�g�����Ƃ��̃C���f�b�N�X��indices��Ή�������
	std::sort(indices.begin(), indices.end(), [&score_list](int i1, int i2) {
		return score_list[i1] > score_list[i2];
	});

	// score_list�~���\�[�g
	std::sort(score_list.begin(), score_list.end(), std::greater<int>());

	// �X�R�A�ɂ��people���\�[�g���ĕԂ�
	// score_list���Ԃ�
	return std::forward_as_tuple(sort_people(people, indices), score_list);

}

// ����
Person crossover(std::vector<Person> parents, int fig_type, cv::Mat_<double> aabb, double l) {

	// n: �p�����[�^�̐�, x: n+1�l�̐e�̃p�����[�^�̔z��
	int n = parents[0].p.size();
	cv::Mat_<double> x = Mat::zeros(n+1, n, CV_32F);
	for (int i = 0; i < n+1; i++) {
		for (int j = 0; j < n; j++) {
			x(i, j) = parents[i].p[j];
		}
	}

	// g: x�̏d�S
	cv::Mat_<double> g;
	cv::reduce(x, g, 0, CV_REDUCE_AVG);

	// �� = ��(n+2)���`
	double alpha = std::sqrt(n + 2);

	// c, p���`
	cv::Mat_<double> c = Mat::zeros(n + 1, n, CV_32F);
	cv::Mat_<double> p = Mat::zeros(n + 1, n, CV_32F);

	// ����c[0], p[0]���Z�o(c_0��0�x�N�g���Ȃ̂ŉ������Ȃ�)
	cv::Mat_<double> p_0 = g + alpha * (x.row(0) - g);

	for (int j = 0; j < n; j++) {
		p(0, j) = p_0(0, j);
	}

	// c[i], p[i](i = 1...n)���Z�o���Ă���
	for (int i = 1; i <= n; i++) {
		double r = std::pow(random_value(0, 1), 1.0 / i);
		cv::Mat_<double> p_i = g + alpha * (x.row(i) - g);
		cv::Mat_<double> c_i = r * (p.row(i - 1) - p_i + c.row(i - 1));

		for (int j = 0; j < n; j++) {
			c(i, j) = c_i(0, j);
			p(i, j) = p_i(0, j);
		}
	}

	// �q�̃p�����[�^��p[n] + c[n]�ƂȂ�
	Mat_<double> child_p = c.row(n) + p.row(n);
	//cout << child_p << endl;

	std::vector<double> child_v;
	child_p.copyTo(child_v);

	Person child(fig_type, child_v);
	return child;
}

// �̕`��
void draw_person(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, cv::Mat_<double> aabb) {

	// �_�Q�`��
	plt::plot(points.col(0), points.col(1), { {"color", "orange"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });

	// �̕`��
	// �~
	if (person.fig_type == 0) {
		Circle figure(person.p[0], person.p[1], person.p[2]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Circle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);
		cv::Mat_<double> fig_points = make_contour(f, aabb, 1.5, 1000, 0.01);
		plt::plot(fig_points.col(0), fig_points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
		
	}

	// ���O�p�`
	else if (person.fig_type == 1) {
		Triangle figure(person.p[0], person.p[1], person.p[2], person.p[3]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Triangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);
		cv::Mat_<double> fig_points = make_contour(f, aabb, 1.5, 1000, 0.01);
		plt::plot(fig_points.col(0), fig_points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	}

	// �����`
	else {
		Rectangle figure(person.p[0], person.p[1], person.p[2], person.p[3], person.p[4]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Rectangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);
		cv::Mat_<double> fig_points = make_contour(f, aabb, 1.5, 1000, 0.01);
		plt::plot(fig_points.col(0), fig_points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	}

	plt::show();
	plt::close();
}