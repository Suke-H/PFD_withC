#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

#include "tools2d.h"

Mat_<double> build_aabb_2d(Mat_<double> points) {
	// points��u�ő�/�ŏ�, v�ő�/�ŏ���������
	Mat_<double> max_p, min_p;
	reduce(points, max_p, 0, CV_REDUCE_MAX);
	reduce(points, min_p, 0, CV_REDUCE_MIN);

	Mat aabb = (Mat_<double>(1, 4) << min_p(0, 0), max_p(0, 0), min_p(0, 1), max_p(0, 1));

	return aabb;
}

// �������񂪓������z����o��
// ����start, �I��stop�Ɛ���̐�num����͂Ƃ���(�����͊֐����ŎZ�o)
Mat_<double> linspace(double start, double stop, int num) {
	
	// �������Z�o
	double step = (stop - start) / (num-1); 

	// ���������sequence�ɓ���Ă���
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

// u, v�̔z����O���b�h�^�ɂ���
// u = [1,2,3], v = [4,5] -> uu = [1,2,3,1,2,3], vv = [4,4,4,5,5,5]
tuple<Mat_<double>, Mat_<double>> meshgrid2d(Mat_<double> U, Mat_<double> V) {

	vector<double> u;
	vector<double> v;
	U.copyTo(u);
	V.copyTo(v);


	vector<double> uu;
	vector<double> vv;

	// uu�̏���
	for (int i = 0; i < v.size(); i++) {
		uu.insert(uu.end(), u.begin(), u.end());
	}
	
	// vv�̏���
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

double test_func(double x) {
	return x * x;
}

double test(function<double(double)> f, double x) {
	return f(x);
}