// PFD_VisualC.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "vec_to_mat.hpp"
#include "figure2d.h"
#include "tools2d.h"


int main()
{
	// 2x3 の行列
	Mat m1 = (Mat_<double>(4, 2) << 1, 2, 7, 8, 3, 4, 5, 6);

	// m1
	std::cout << m1 << "\n" << std::endl;


	vector<double> aabb = build_aabb_2d(m1);

	for (int i = 0; i < aabb.size(); i++)
		printf("%lf,", aabb[i]);

	printf("\n");


	Mat m2 = Mat_<float>::zeros(1, 10);
	// m2
	std::cout << m2 << "\n" << std::endl;

	Mat_<double> x = linspace(0, 4, 5);
	Mat_<double> y = linspace(5, 7, 3);

	std::cout << x << "\n" << std::endl;
	std::cout << y << "\n\n" << std::endl;

	Mat_<double> xx; 
	Mat_<double> yy;

	tie(xx, yy) = meshgrid2d(x, y);

	std::cout << xx << "\n" << std::endl;
	std::cout << yy << "\n\n" << std::endl;

}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
