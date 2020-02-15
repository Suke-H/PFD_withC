// PFD_VisualC.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "VecToMat.hpp"

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

//int circle(Mat X, Mat Y) {
//
//	return 
//}



int main()
{
	std::cout << "Hello World!\n" << endl;


	// お試し定義
	point_t p1(1, 2, 3);
	point_t p2(4, 5, 6);
	point_t p3(7, 8, 9);
	points_t points{ p1, p2, p3 };

	printf("%d\n", points.size());

	for (int i = 0; i < points.size(); i++)
	{
		std::cout << points[i] << "\n" << endl;
	}

	//Mat pm = Mat(points);

	Mat_<float> pm;
	VecToMat(points, pm);

	std::cout << pm << std::endl;

	std::cout << "(" << pm.rows << ", " << pm.cols << ")\n" << std::endl;
	// 要素の総数
	std::cout << "total:" << pm.total() << std::endl;
	// 0列目
	std::cout << pm.col(0) << std::endl << std::endl;



	// 3x3 の行列
	cv::Mat m1 = (cv::Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);

	std::cout << m1 << std::endl;

	// 0列目
	std::cout << m1.col(0) << std::endl << std::endl;


	//Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

	//// Red，太さ3，4近傍連結
	//line(img, cv::Point(100, 100), cv::Point(400, 105), cv::Scalar(0, 0, 200), 3, 4);
	//// Green，太さ5，8近傍連結
	//line(img, cv::Point(100, 200), cv::Point(400, 205), cv::Scalar(0, 200, 0), 5, 8);
	//// Blue，太さ10，アンチエイリアス
	//line(img, cv::Point(100, 300), cv::Point(400, 305), cv::Scalar(200, 0, 0), 10, CV_AA);

	//cv::namedWindow("drawing", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	//cv::imshow("drawing", img);
	//cv::waitKey(0);



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
