#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools2d.h"

//     [x1, x2, ..., xn]      [x1, y1]
//     [y1, y2, ..., yn]  ->      :    
//                            [xn, yn]
// このように変形する

Mat_<double> composition2d(Mat_<double> u, Mat_<double> v) {
	//Mat_<double> uv = Mat::zeros(2, u.cols, CV_32F);

	vector<Mat> tmp{ u, v };

	// uvにuとvを移す
	/*Mat A = uv.row(0);
	u.row(0).copyTo(uv);
	Mat B = uv.row(1);
	v.row(0).copyTo(uv);*/

	Mat uv;
	merge(tmp, uv);

	// uvを転置
	transpose(uv, uv);

	return uv;
}

Mat_<double> build_aabb_2d(Mat_<double> points) {
	// pointsのu最大/最小, v最大/最小を見つける
	Mat_<double> max_p, min_p;
	reduce(points, max_p, 0, CV_REDUCE_MAX);
	reduce(points, min_p, 0, CV_REDUCE_MIN);

	Mat aabb = (Mat_<double>(1, 4) << min_p(0, 0), max_p(0, 0), min_p(0, 1), max_p(0, 1));

	return aabb;
}

// 等差数列が入った配列を出力
// 初項start, 終項stopと数列の数numを入力とする(公差は関数内で算出)
Mat_<double> linspace(double start, double stop, int num) {
	
	// 公差を算出
	double step = (stop - start) / (num-1); 

	// 等差数列をsequenceに入れていく
	vector<double> sequence; 
	double tmp = start;

	for (int i = 0; i < num; i++) {
		sequence.push_back(tmp);
		tmp += step;
	}

	Mat_<double> seq = Mat(sequence);
	transpose(seq, seq);
	
	return seq;
}

// u, vの配列をグリッド型にする
// u = [1,2,3], v = [4,5] -> uu = [1,2,3,1,2,3], vv = [4,4,4,5,5,5]
tuple<Mat_<double>, Mat_<double>> meshgrid2d(Mat_<double> U, Mat_<double> V) {

	vector<double> u;
	vector<double> v;
	U.copyTo(u);
	V.copyTo(v);

	vector<double> uu;
	vector<double> vv;

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

	Mat_<double> mat_u = Mat(uu);
	transpose(mat_u, mat_u);
	Mat_<double> mat_v = Mat(vv);
	transpose(mat_v, mat_v);

	return forward_as_tuple(mat_u, mat_v);
}

// 配列vにxが存在するかチェックする
// (intしか対応していない)
int vec_in(vector<int> v, int x) {

	auto result = find(v.begin(), v.end(), x);

	if (result == v.end()) {
		return 0;
	}
	else {
		return 1;
	}

}

// row_listで指定された行をmat_inから削除する
Mat_<double> delete_rows(Mat_<double> mat_in, vector<int> row_list) {
	// 出力される行数
	int out_rows = mat_in.rows - row_list.size();
	// 出力
	Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// 削除リストに入ってない行を代入していく
		if (!vec_in(row_list, i)) {
			Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}
	
	return mat_out;
}

// delete_rows()の逆で、mat_inからrow_listで指定された行を残す
Mat_<double> extract_rows(Mat_<double> mat_in, vector<int> row_list) {
	// 出力される行数
	int out_rows = row_list.size();
	// 出力
	Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// 削除リストに入ってる行を代入していく
		if (vec_in(row_list, i)) {
			Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}

	return mat_out;
}

Mat_<double> make_contour(function<Mat_<double>(Mat_<double>, Mat_<double>)> f, Mat_<double> aabb, double aabb_size, int grid_num, double epsilon) {
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
	Mat_<double> u = linspace(umin, umax, grid_num);
	Mat_<double> v = linspace(vmin, vmax, grid_num);

	// AABB内の格子点にする
	Mat_<double> uu;
	Mat_<double> vv;
	tie(uu, vv) = meshgrid2d(u, v);

	//cout << u  << "\n" << v << "\n" << endl;

	// f-repの値をとる
	Mat_<double> w = f(uu, vv);

	//cout << w << "\n" << endl;

	// |f(u, v)| < εを満たす行は255、それ以外は0になる
	Mat_<int> w_bool = abs(w) < epsilon;

	//cout << w_bool << "\n" << endl;

	// 255のインデックスを配列に保存
	vector<int> row_list;

	for (int i = 0; i < w_bool.cols; i++) {
		if (w_bool(0, i) == 255) {
			row_list.push_back(i);
		}
	}


	// 条件を満たした(u, v)を抽出
	Mat_<double> uv = composition2d(uu, vv);
	Mat_<double> points = extract_rows(uv, row_list);

	return points;
}