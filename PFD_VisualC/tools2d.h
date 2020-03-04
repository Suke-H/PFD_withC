#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// points_t型をcv::Mat型に変換
cv::Mat_<double> vec3_to_mat(points_t points);

// cv::Mat型をstd::vector< cv::Point >型に変換
std::vector< cv::Point > mat_to_vec2(cv::Mat_<double> mat);

// std::vector<double>型の配列をsize分プリントする
void print_vec_double(std::vector<double> v, int size, std::string name);

// low～highの範囲で一様乱数に基づいて実数を返す
double random_value(double low, double high);

//[x1, y1]         [x1, x2, ..., xn]
//    :       ->   [y1, y2, ..., yn]
//[xn, yn] 
// Mat_<double>型の2次元点群をx, yに分離
std::tuple<cv::Mat_<double>, cv::Mat_<double>> disassemble2d(cv::Mat_<double> points);

// Mat_<double>型のx, yを2次元点群に合成
cv::Mat_<double> composition2d(cv::Mat_<double> u, cv::Mat_<double> v);

// pointsのAABBを作成
std::tuple<cv::Mat_<double>, double> build_aabb2d(cv::Mat_<double> points);

// 初項start, 終項stop, 数列の数numの等差数列が入った配列を出力
cv::Mat_<double> linspace(double start, double stop, int num);

// u = [1,2,3], v = [4,5] -> uu = [1,2,3,1,2,3], vv = [4,4,4,5,5,5]
// u, vの配列をグリッド型にする
std::tuple<cv::Mat_<double>, cv::Mat_<double>> meshgrid2d(cv::Mat_<double> U, cv::Mat_<double> V);

// std::vector<int>型の配列vにxが存在するかチェックする
int vec_search(std::vector<int> v, int x);

// std::vector<int>型の配列vからランダムにsize個サンプリング
std::vector<int> random_sample(std::vector<int> v, int size);

// row_listで指定された行をmat_inから削除する
cv::Mat_<double> delete_rows(cv::Mat_<double> mat_in, std::vector<int> row_list);

// delete_rows()の逆で、row_listで指定された行をmat_inに残し、残りを削除する
cv::Mat_<double> extract_rows(cv::Mat_<double> mat_in, std::vector<int> row_list);

// 図形の輪郭を描画するために、f-rep関数から輪郭の点群を作成
cv::Mat_<double> make_contour(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, 
	double aabb_size=1.5, int grid_num=1000, double epsilon=0.01);

// 図形領域内部を描画するために、f-rep関数から内部の点群を作成(make_contour関数と違い、出力する点群数Nを入力に取る)
cv::Mat_<double> make_inside(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, int N,
	double aabb_size = 1.5, int grid_num = 200);