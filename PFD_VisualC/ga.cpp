#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <matplotlibcpp.h>
#include <random>

namespace plt = matplotlibcpp;
using namespace std;
using namespace cv;

#include "tools.h"
#include "figure.h"
#include "ga.h"
#include "ga_score.h"

// 個体のclass
// コンストラクタ
Person::Person(int fig_type, std::vector<double> p) {
	this->fig_type = fig_type;
	this->p = p;
}

// 個体情報を出力
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

// 1種類の図形単体のGA
Person single_ga(int fig_type, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area,
	int n_epoch, int N, int save_num, int tournament_size, int half_reset_num, int all_reset_num) {

	// AABB生成
	cv::Mat_<double> aabb;
	double l;
	std::tie(aabb, l) = build_aabb2d(points);

	// 淘汰処理用のRESET指数を定義
	int half_n = 0;
	int all_n = 0;

	// 前世代の1位のスコアを格納する変数
	double prev_score = 0;

	// 全淘汰前に1位を保存しておく配列
	std::vector<Person> records;
	std::vector<double> record_score;


	// N人個体生成
	std::vector<Person> generation = create_random_population(N, fig_type, aabb, l);

	// GA開始
	for (int epoch = 0; epoch < n_epoch; epoch++) {
		//cout << "epoch: " << epoch << endl;

		// ランキング
		std::tie(generation, std::ignore) = ranking(generation, points, out_points, out_area);

		// 上位save_num人を保存
		std::vector<Person> next_generation;
		for (int i = 0; i < save_num; i++) {
			next_generation.push_back(generation[i]);
		}

		// 次世代がN人を超すまで、トーナメント選択 -> 交叉 を繰り返す
		while (next_generation.size() < N) {

			// トーナメントサイズ分の人数出場
			std::vector<Person> entry = random_sample_people(generation, tournament_size, 0, tournament_size-1);

			// 図形の種類により交叉のときの親の個体数変更
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

			// 上位parent_num人優勝
			std::tie(entry, std::ignore) = ranking(entry, points, out_points, out_area);
			std::vector<Person> winners;
			for (int i = 0; i < parent_num; i++) {
				winners.push_back(entry[i]);
			}

			// 勝者を交叉して次世代に追加
			next_generation.push_back(crossover(winners, fig_type, aabb, l));
		}

		// 次世代を現世代に移す
		std::copy(next_generation.begin(), next_generation.begin() + N, generation.begin());

		// 以降、RESET処理
		std::vector<double> score_list;
		std::tie(generation, score_list) = ranking(generation, points, out_points, out_area);

		// 現世代の1位のスコア保存
		double current_score = score_list[0];

		// スコアが前世代と変わらないようならhalf_nを増やす
		if (prev_score >= current_score) {
			half_n++;
		}
		// スコアが増加したらhalf_nをリセット
		else {
			half_n = 0;
		}

		// half_nが定めた値(=half_reset_num)に達したらall_nを増やし、half_nはリセット
		if (half_n == half_reset_num) {
			all_n++;
			half_n = 0;

			// all_nが定めた値(=all_reset_num)に達したら1位を記録して全数淘汰し、all_nはリセット
			if (all_n == all_reset_num) {
				records.push_back(generation[0]);
				record_score.push_back(generation[0].score);
				generation = create_random_population(N, fig_type, aabb, l);
				all_n = 0;
			}

			// all_nが達していなかったら1位を除き半数淘汰
			else {
				// 半数の初期集団生成
				int half_size = N / 2;
				std::vector<Person> people = create_random_population(N/2, fig_type, aabb, l);

				// 現世代から1位+半数を抽出
				int sample_size = N - half_size;
				Person best(generation[0]);
				generation = random_sample_people(generation, sample_size - 1, 1, sample_size - 1);
				generation.push_back(best);

				// 合体
				generation.reserve(generation.size() + people.size());
				std::copy(people.begin(), people.end(), std::back_inserter(generation));
			}

		}

		// 現在のスコアを前のスコアとして、終わり
		prev_score = current_score;

		// 途中結果表示
		if (epoch % 100 == 0) {
			cout << "epoch: " << epoch << endl;
			std::vector<double> score_list;
			std::tie(std::ignore, score_list) = ranking(generation, points, out_points, out_area);
			print_vec_double(score_list, 10, "score");
			print_vec_double(generation[0].p, generation[0].p.size(), "gene1");
			//draw_person(generation[0], points, out_points, aabb);
			cout << "///////////////////////////////////" << endl;

		}
	}

	// 1位を出力
	std::tie(generation, std::ignore) = ranking(generation, points, out_points, out_area);
	print_vec_double(generation[0].p, generation[0].p.size(), "gene1");

	// 1位の図形パラメータ
	std::vector<double> best_p;
	for (int i = 0; i < generation[0].p.size(); i++) {
		best_p.push_back(generation[0].p[i]);
	}

	// 1位のスコア
	double best_score = generation[0].score;

	// 記録した個体の中から1位のインデックスを取り出す
	std::vector<double>::iterator iter = std::max_element(record_score.begin(), record_score.end());
	size_t index = std::distance(record_score.begin(), iter);
	print_vec_double(records[index].p, records[index].p.size(), "kako1");
	
	// 最終世代の1位と記録した1位を比較して、記録1位が勝ったら上書き
	std::cout <<  "gene1: " << generation[0].score << " vs " << "kako1: " << record_score[index] << endl;
	if (generation[0].score < record_score[index]) {
		for (int i = 0; i < records[index].p.size(); i++) {
			best_p[i] = records[index].p[i];
			best_score = records[index].score;
		}
	}

	// 最終結果をPersonクラスに格納
	Person best(fig_type, best_p);
	best.score = best_score;
	best.score_flag = 1;

	// 最終結果の個体を描画
	print_vec_double(best.p, best.p.size(), "best");
	draw_person(best, points, out_points, aabb);
	return best;
	
}

// 3種類の図形単体GAを回してスコア最大の図形を選択
Person entire_ga(cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {

	// 円、正三角形、長方形の3種類の図形単体GAを回す
	cout << "円" << endl;
	Person best_circle(single_ga(0, points, out_points, out_area, 300, 200, 20, 10, 15, 9));

	cout << "///////////////////////////////////" << endl;
	cout << "正三角形" << endl;
	Person best_tri(single_ga(1, points, out_points, out_area, 500, 200, 20, 10, 20, 10));

	cout << "///////////////////////////////////" << endl;
	cout << "長方形" << endl;
	Person best_rect(single_ga(2, points, out_points, out_area, 600, 200, 20, 25, 20, 10));

	// スコアが一番高い図形を最終結果として出力
	std::vector<double> score_list{ best_circle.score, best_tri.score, best_rect.score };
	std::vector<double>::iterator iter = std::max_element(score_list.begin(), score_list.end());
	size_t index = std::distance(score_list.begin(), iter);

	// AABB生成
	cv::Mat_<double> aabb;
	std::tie(aabb, std::ignore) = build_aabb2d(points);

	if (index == 0) {
		draw_person(best_circle, points, out_points, aabb);
		return best_circle;
	}

	else if (index == 1) {
		draw_person(best_tri, points, out_points, aabb);
		return best_tri;
	}

	else {
		draw_person(best_rect, points, out_points, aabb);
		return best_rect;
	}
	
}

// 初期個体生成
Person create_random_person(int fig_type, cv::Mat_<double> aabb, double l) {

	std::vector<double> p;

	// 円
	if (fig_type == 0) {
		// 中心座標はAABBの範囲内
		double u_0 = random_value(aabb(0, 0), aabb(0, 1));
		double v_0 = random_value(aabb(0, 2), aabb(0, 3));
		// l/5 < r < 2/3*l
		double r = random_value(l / 5, l * 2 / 3);

		p.push_back(u_0);
		p.push_back(v_0);
		p.push_back(r);
	}

	else if (fig_type == 1) {
		// 中心座標はAABBの範囲内
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
		// 中心座標はAABBの範囲内
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

// 初期集団生成
std::vector<Person> create_random_population(int N, int fig_type, cv::Mat_<double> aabb, double l) {

	std::vector<Person> people;
	people.reserve(N);
	for (int i = 0; i < N; i++) {
		people.push_back(create_random_person(fig_type, aabb, l));
	}

	return people;
}

// std::vector<Person>型の配列からランダムにsize個サンプリング
// rand_min～rand_maxは取り出すインデックスの範囲(基本はrand_min=0, rand_max=size-1)
// (size >= rand_min～rand_max間の数であることが条件)
std::vector<Person> random_sample_people(std::vector<Person> people, int size, int rand_min, int rand_max) {

	// 以後、rand_min～rand_maxの整数からランダムに"重複なし"でsize個とった配列reを生成
	std::vector<int> re(size);

	// rand_min～rand_maxの整数の一様乱数
	std::uniform_int_distribution<int> rand(rand_min, rand_max);
	// 非決定的な乱数生成器を生成
	std::random_device rnd;
	//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
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

	// 作成した配列の要素をインデックスとして並び替え
	std::vector<Person> w;

	for (int i = 0; i < size; i++) {
		Person person(people[re[i]]);
		w.push_back(person);
	}

	return w;
}

// std::vector<Person>型の配列をインデックス順に並び変える
std::vector<Person> sort_people(std::vector<Person> people, std::vector<int> indices) {
	
	std::vector<Person> sorted_people;

	for (int i = 0; i < people.size(); i++) {
		sorted_people.push_back(people[indices[i]]);
	}

	return sorted_people;
}

// スコア算出
Person scoring(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {

	// スコアフラグが立ってなかったらスコア算出して、スコアフラグを立てる
	if (!person.score_flag) {
		person.score = calc_score(person, points, out_points, out_area);
		person.score_flag = 1;
	}

	return person;
}

// 個体をランク付けしてソート
std::tuple<std::vector<Person>, std::vector<double>> ranking(
	std::vector<Person> people, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area) {

	// peopleをスコア付けしていく
	size_t N = people.size();
	for (int i = 0; i < N; i++) {
		people[i] = scoring(people[i], points, out_points, out_area);
	}

	// スコアを記録していく
	std::vector<double> score_list;
	score_list.reserve(N);
	for (int i = 0; i < N; i++) {
		score_list.push_back(people[i].score);
	}

	// ソート
	// 配列のインデックス indices = {0, 1, 2, ...} を作成する。
	std::vector<int> indices;
	for (int i = 0; i < N; i++) {
		indices.push_back(i);
	}

	// score_listを降順ソートしたときのインデックスとindicesを対応させる
	std::sort(indices.begin(), indices.end(), [&score_list](int i1, int i2) {
		return score_list[i1] > score_list[i2];
	});

	// score_list降順ソート
	std::sort(score_list.begin(), score_list.end(), std::greater<int>());

	// スコアによりpeopleをソートして返す
	// score_listも返す
	return std::forward_as_tuple(sort_people(people, indices), score_list);

}

// 交叉
Person crossover(std::vector<Person> parents, int fig_type, cv::Mat_<double> aabb, double l) {

	// n: パラメータの数, x: n+1人の親のパラメータの配列
	int n = parents[0].p.size();
	cv::Mat_<double> x = Mat::zeros(n+1, n, CV_32F);
	for (int i = 0; i < n+1; i++) {
		for (int j = 0; j < n; j++) {
			x(i, j) = parents[i].p[j];
		}
	}

	// g: xの重心
	cv::Mat_<double> g;
	cv::reduce(x, g, 0, CV_REDUCE_AVG);

	// α = √(n+2)を定義
	double alpha = std::sqrt(n + 2);

	// c, pを定義
	cv::Mat_<double> c = Mat::zeros(n + 1, n, CV_32F);
	cv::Mat_<double> p = Mat::zeros(n + 1, n, CV_32F);

	// 初項c[0], p[0]を算出(c_0は0ベクトルなので何もしない)
	cv::Mat_<double> p_0 = g + alpha * (x.row(0) - g);

	p.row(0) = p_0;

	// c[i], p[i](i = 1...n)を算出していく
	for (int i = 1; i <= n; i++) {
		double r = std::pow(random_value(0, 1), 1.0 / i);
		cv::Mat_<double> p_i = g + alpha * (x.row(i) - g);
		cv::Mat_<double> c_i = r * (p.row(i - 1) - p_i + c.row(i - 1));

		c.row(i) = c_i;
		p.row(i) = p_i;

	}

	// 子のパラメータはp[n] + c[n]となる
	Mat_<double> child_p = c.row(n) + p.row(n);

	std::vector<double> child_v;
	child_p.copyTo(child_v);

	Person child(fig_type, child_v);
	return child;
}

// 個体描画
void draw_person(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, cv::Mat_<double> aabb) {

	// 点群描画
	plt::plot(points.col(0), points.col(1), { {"color", "orange"} ,{ "marker", "o"}, { "linestyle", ""} });
	plt::plot(out_points.col(0), out_points.col(1), { {"color", "red"} ,{ "marker", "o"}, { "linestyle", ""} });

	// 個体描画
	// 円
	if (person.fig_type == 0) {
		Circle figure(person.p[0], person.p[1], person.p[2]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Circle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);
		cv::Mat_<double> fig_points = make_contour(f, aabb, 1.5, 1000, 0.01);
		plt::plot(fig_points.col(0), fig_points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
		
	}

	// 正三角形
	else if (person.fig_type == 1) {
		Triangle figure(person.p[0], person.p[1], person.p[2], person.p[3]);
		std::function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f =
			std::bind(&Triangle::f_rep_list, &figure, std::placeholders::_1, std::placeholders::_2);
		cv::Mat_<double> fig_points = make_contour(f, aabb, 1.5, 1000, 0.01);
		plt::plot(fig_points.col(0), fig_points.col(1), { {"color", "blue"} ,{ "marker", "o"}, { "linestyle", ""} });
	}

	// 長方形
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