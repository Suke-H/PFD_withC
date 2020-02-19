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
// ���̂悤�ɕό`����

Mat_<double> composition2d(Mat_<double> u, Mat_<double> v) {
	//Mat_<double> uv = Mat::zeros(2, u.cols, CV_32F);

	vector<Mat> tmp{ u, v };

	// uv��u��v���ڂ�
	/*Mat A = uv.row(0);
	u.row(0).copyTo(uv);
	Mat B = uv.row(1);
	v.row(0).copyTo(uv);*/

	Mat uv;
	merge(tmp, uv);

	// uv��]�u
	transpose(uv, uv);

	return uv;
}

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

// �z��v��x�����݂��邩�`�F�b�N����
// (int�����Ή����Ă��Ȃ�)
int vec_in(vector<int> v, int x) {

	auto result = find(v.begin(), v.end(), x);

	if (result == v.end()) {
		return 0;
	}
	else {
		return 1;
	}

}

// row_list�Ŏw�肳�ꂽ�s��mat_in����폜����
Mat_<double> delete_rows(Mat_<double> mat_in, vector<int> row_list) {
	// �o�͂����s��
	int out_rows = mat_in.rows - row_list.size();
	// �o��
	Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// �폜���X�g�ɓ����ĂȂ��s�������Ă���
		if (!vec_in(row_list, i)) {
			Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}
	
	return mat_out;
}

// delete_rows()�̋t�ŁAmat_in����row_list�Ŏw�肳�ꂽ�s���c��
Mat_<double> extract_rows(Mat_<double> mat_in, vector<int> row_list) {
	// �o�͂����s��
	int out_rows = row_list.size();
	// �o��
	Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// �폜���X�g�ɓ����Ă�s�������Ă���
		if (vec_in(row_list, i)) {
			Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}

	return mat_out;
}

Mat_<double> make_contour(function<Mat_<double>(Mat_<double>, Mat_<double>)> f, Mat_<double> aabb, double aabb_size, int grid_num, double epsilon) {
	// AABB��uv�ő�E�ŏ��l���Ƃ�
	double umin = aabb(0, 0);
	double umax = aabb(0, 1);
	double vmin = aabb(0, 2);
	double vmax = aabb(0, 3);

	// �`��͈͊g��̂��߁AAABB��aabb_size�{�ɂ���
	umax = umax + (umax - umin) / 2 * aabb_size;
	umin = umin - (umax - umin) / 2 * aabb_size;
	vmax = vmax + (vmax - vmin) / 2 * aabb_size;
	vmin = vmin - (vmax - vmin) / 2 * aabb_size;

	// AABB�͈͓̔���u, v���W�̓���������쐬
	Mat_<double> u = linspace(umin, umax, grid_num);
	Mat_<double> v = linspace(vmin, vmax, grid_num);

	// AABB���̊i�q�_�ɂ���
	Mat_<double> uu;
	Mat_<double> vv;
	tie(uu, vv) = meshgrid2d(u, v);

	//cout << u  << "\n" << v << "\n" << endl;

	// f-rep�̒l���Ƃ�
	Mat_<double> w = f(uu, vv);

	//cout << w << "\n" << endl;

	// |f(u, v)| < �Â𖞂����s��255�A����ȊO��0�ɂȂ�
	Mat_<int> w_bool = abs(w) < epsilon;

	//cout << w_bool << "\n" << endl;

	// 255�̃C���f�b�N�X��z��ɕۑ�
	vector<int> row_list;

	for (int i = 0; i < w_bool.cols; i++) {
		if (w_bool(0, i) == 255) {
			row_list.push_back(i);
		}
	}


	// �����𖞂�����(u, v)�𒊏o
	Mat_<double> uv = composition2d(uu, vv);
	Mat_<double> points = extract_rows(uv, row_list);

	return points;
}