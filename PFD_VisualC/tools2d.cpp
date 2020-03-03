#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
#include <random>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools2d.h"

// std::vector<double>型の配列をsize分プリントする
void print_vec_double(std::vector<double> v, int size, std::string name) {

	name += ": {";
	printf("%s", name.c_str());

	for (int i = 0; i < size; i++) {
		printf("%lf, ", v[i]);
	}

	printf("}\n");
}

// low～highの範囲で実数の乱数を返す
double random_value(double low, double high) {
	
	// mt19937という疑似乱数を利用
	std::random_device rd;
	std::mt19937 gen(rd());

	// 0～1の範囲での一様乱数を返す
	double num = std::generate_canonical<double, 10>(gen);

	// low～highに直す
	return (high - low) * num + low;
}

//[x1, y1]         [x1, x2, ..., xn]
//    :       ->   [y1, y2, ..., yn]
//[xn, yn] 

std::tuple<cv::Mat_<double>, cv::Mat_<double>> disassemble2d(cv::Mat_<double> points) {
	//cv::Mat_<double> uv = Mat::zeros(2, u.cols, CV_32F);

	cv::Mat_<double> u = points.col(0);
	cv::Mat_<double> v = points.col(1);

	// 転置
	cv::transpose(u, u);
	cv::transpose(v, v);

	return std::forward_as_tuple(u, v);
}


//     [x1, x2, ..., xn]      [x1, y1]
//     [y1, y2, ..., yn]  ->      :    
//                            [xn, yn]
// このように変形する

cv::Mat_<double> composition2d(cv::Mat_<double> u, cv::Mat_<double> v) {
	//cv::Mat_<double> uv = Mat::zeros(2, u.cols, CV_32F);

	std::vector<Mat> tmp{ u, v };

	cv::Mat uv;
	cv::merge(tmp, uv);

	// uvを転置
	cv::transpose(uv, uv);

	return uv;
}

// pointsのAABBを作成
std::tuple<cv::Mat_<double>, double> build_aabb2d(cv::Mat_<double> points) {

	// pointsのu最大/最小, v最大/最小を見つける
	cv::Mat_<double> max_p, min_p;
	cv::reduce(points, max_p, 0, CV_REDUCE_MAX);
	cv::reduce(points, min_p, 0, CV_REDUCE_MIN);

	// AABB
	cv::Mat aabb = (cv::Mat_<double>(1, 4) << min_p(0, 0), max_p(0, 0), min_p(0, 1), max_p(0, 1));

	// AABBの対角線
	double l = sqrt(pow((max_p(0, 0) - min_p(0, 0)), 2) + pow((max_p(0, 1) - min_p(0, 1)), 2));

	return std::forward_as_tuple(aabb, l);
}

// 等差数列が入った配列を出力
// 初項start, 終項stopと数列の数numを入力とする(公差は関数内で算出)
cv::Mat_<double> linspace(double start, double stop, int num) {
	
	// 公差を算出
	double step = (stop - start) / (num-1); 

	// 等差数列をsequenceに入れていく
	std::vector<double> sequence; 
	double tmp = start;

	for (int i = 0; i < num; i++) {
		sequence.push_back(tmp);
		tmp += step;
	}

	// 縦ベクトルを横ベクトルに変換する
	cv::Mat_<double> seq = Mat(sequence);
	cv::transpose(seq, seq);
	
	return seq;
}

// u, vの配列をグリッド型にする
// u = [1,2,3], v = [4,5] -> uu = [1,2,3,1,2,3], vv = [4,4,4,5,5,5]
tuple<cv::Mat_<double>, cv::Mat_<double>> meshgrid2d(cv::Mat_<double> U, cv::Mat_<double> V) {

	std::vector<double> u;
	std::vector<double> v;
	U.copyTo(u);
	V.copyTo(v);

	std::vector<double> uu;
	std::vector<double> vv;

	// uuの処理
	for (int i = 0; i < v.size(); i++) {
		uu.insert(uu.end(), u.begin(), u.end());
	}
	
	// vvの処理
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < u.size(); j++) {
			vv.push_back(v[i]);
		}
	}

	cv::Mat_<double> mat_u = Mat(uu);
	cv::transpose(mat_u, mat_u);
	cv::Mat_<double> mat_v = Mat(vv);
	cv::transpose(mat_v, mat_v);

	return forward_as_tuple(mat_u, mat_v);
}

// std::vector<int>型の配列vにxが存在するかチェックする
int vec_search(std::vector<int> v, int x) {

	auto result = find(v.begin(), v.end(), x);

	if (result == v.end()) {
		return 0;
	}
	else {
		return 1;
	}

}

// std::vector<int>型の配列からランダムにsize個サンプリング
std::vector<int> random_sample(std::vector<int> v, int size) {

	// 以後、rand_min～rand_maxの整数からランダムに"重複なし"でsize個とった配列を生成
	int rand_min = 0;
	int rand_max = v.size() - 1;
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
	std::vector<int> w;

	for (int i = 0; i < size; i++) {
		w.push_back(v[re[i]]);
	}

	return w;
}

// row_listで指定された行をcv::Mat_inから削除する
cv::Mat_<double> delete_rows(cv::Mat_<double> mat_in, std::vector<int> row_list) {
	// 出力される行数
	int out_rows = mat_in.rows - row_list.size();
	// 出力
	cv::Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// 削除リストに入ってない行を代入していく
		if (!vec_search(row_list, i)) {
			cv::Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}
	
	return mat_out;
}

// delete_rows()の逆で、mat_inからrow_listで指定された行を残す
cv::Mat_<double> extract_rows(cv::Mat_<double> mat_in, std::vector<int> row_list) {
	// 出力される行数
	int out_rows = row_list.size();
	// 出力
	cv::Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// 削除リストに入ってる行を代入していく
		if (vec_search(row_list, i)) {
			cv::Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}

	return mat_out;
}

// 図形の輪郭を描画するために、f-rep関数から輪郭の点群を作成
cv::Mat_<double> make_contour(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, 
	double aabb_size, int grid_num, double epsilon) {
	// AABBのuv最大・最小値をとる
	double umin = aabb(0, 0);
	double umax = aabb(0, 1);
	double vmin = aabb(0, 2);
	double vmax = aabb(0, 3);

	// 描画範囲拡大のため、AABBをaabb_size倍にする
	umax = umax + (umax - umin) / 2 * aabb_size;
	umin = umin - (umax - umin) / 2 * aabb_size;
	vmax = vmax + (vmax - vmin) / 2 * aabb_size;
	vmin = vmin - (vmax - vmin) / 2 * aabb_size;

	// AABBの範囲内でu, v座標の等差数列を作成
	cv::Mat_<double> u = linspace(umin, umax, grid_num);
	cv::Mat_<double> v = linspace(vmin, vmax, grid_num);

	// AABB内の格子点にする
	cv::Mat_<double> uu;
	cv::Mat_<double> vv;
	std::tie(uu, vv) = meshgrid2d(u, v);

	// f-repの値をとる
	cv::Mat_<double> w = f(uu, vv);

	// |f(u, v)| < εを満たす行は255、それ以外は0になる
	cv::Mat_<int> w_bool = cv::abs(w) < epsilon;

	// 255のインデックスを配列に保存
	std::vector<int> row_list;

	for (int i = 0; i < w_bool.cols; i++) {
		if (w_bool(0, i) == 255) {
			row_list.push_back(i);
		}
	}

	// 条件を満たした(u, v)を抽出
	cv::Mat_<double> uv = composition2d(uu, vv);
	cv::Mat_<double> points = extract_rows(uv, row_list);

	return points;
}

// 図形領域内部を描画するために、f-rep関数から内部の点群を作成
// make_contour関数と違い、出力する点群数Nを入力に取る
cv::Mat_<double> make_inside(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, int N,
	double aabb_size, int grid_num) {
	// AABBのuv最大・最小値をとる
	double umin = aabb(0, 0);
	double umax = aabb(0, 1);
	double vmin = aabb(0, 2);
	double vmax = aabb(0, 3);

	// 描画範囲拡大のため、AABBをaabb_size倍にする
	umax = umax + (umax - umin) / 2 * aabb_size;
	umin = umin - (umax - umin) / 2 * aabb_size;
	vmax = vmax + (vmax - vmin) / 2 * aabb_size;
	vmin = vmin - (vmax - vmin) / 2 * aabb_size;

	cv::Mat_<double> uu;
	cv::Mat_<double> vv;
	std::vector<int> row_list;

	// 点群数がNに達するまで繰り返す
	while (1) {

		// AABBの範囲内でu, v座標の等差数列を作成
		cv::Mat_<double> u = linspace(umin, umax, grid_num);
		cv::Mat_<double> v = linspace(vmin, vmax, grid_num);

		// AABB内の格子点にする
		std::tie(uu, vv) = meshgrid2d(u, v);

		// f-repの値をとる
		cv::Mat_<double> w = f(uu, vv);

		// |f(u, v)| >= 0を満たす行は255、それ以外は0になる
		cv::Mat_<int> w_bool = w >= 0;

		// 255のインデックスを配列に保存
		for (int i = 0; i < w_bool.cols; i++) {
			if (w_bool(0, i) == 255) {
				row_list.push_back(i);
			}
		}

		printf("size:%d\n", row_list.size());

		// 条件を満たす点がN個以上あったら終了
		if (row_list.size() >= N) {
			row_list = random_sample(row_list, N);
			break;
		}

		// grid_numを50足してやり直し
		grid_num += 50;

		// row_listをリセット
		row_list.clear();
		row_list.shrink_to_fit();
	}

	// 条件を満たした(u, v)を抽出
	cv::Mat_<double> uv = composition2d(uu, vv);
	cv::Mat_<double> points = extract_rows(uv, row_list);

	return points;
}