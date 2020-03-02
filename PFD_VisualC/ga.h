#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 個体のclass
class Person
{
public:
	// 図形の種類
	int fig_type;
	// 図形パラメータ
	std::vector<double> p;
	// スコア
	double score = 0;
	// スコアが算出されたら立つフラグ
	int score_flag = 0;

	// コンストラクタ
	Person(int fig_type, std::vector<double> p);

	// コピーコンストラクタ
	//Person(const Person& person);

	// 個体情報を出力
	void profile();
};

//1種類の図形単体のGA
Person single_ga(int fig_type, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area,
	int n_epoch = 300, int N = 100, int save_num = 5, int tournament_size = 20, 
	int half_reset_num = 15, int all_reset_num = 9);

// 3種類の図形単体GAを回してスコア最大の図形を選択
//Person entire_ga(cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area);

// 初期個体生成
Person create_random_person(int fig_type, cv::Mat_<double> aabb, double l);

// 初期集団生成
std::vector<Person> create_random_population(int N, int fig_type, cv::Mat_<double> aabb, double l);

// std::vector<Person>型の配列からランダムにsize個サンプリング
// rand_min～rand_maxは取り出すインデックスの範囲(基本はrand_min=0, rand_max=size-1)
// (size >= rand_min～rand_max間の数であることが条件)
std::vector<Person> random_sample_people(std::vector<Person> people, int size, int rand_min, int rand_max);

// std::vector<Person>型の配列をインデックス順に並び変える
std::vector<Person> sort_people(std::vector<Person> people, std::vector<int> indices);

// スコア算出
Person scoring(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area);

// 個体をランク付けしてソート
std::tuple<std::vector<Person>, std::vector<double>> ranking(
	std::vector<Person> people, cv::Mat_<double> points, cv::Mat_<double> out_points, double out_area);

// 交叉
Person crossover(std::vector<Person> parents, int fig_type, cv::Mat_<double> aabb, double l);

// 個体描画
void draw_person(Person person, cv::Mat_<double> points, cv::Mat_<double> out_points, cv::Mat_<double> aabb);