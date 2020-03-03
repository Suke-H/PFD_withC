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

// std::vector<double>�^�̔z���size���v�����g����
void print_vec_double(std::vector<double> v, int size, std::string name) {

	name += ": {";
	printf("%s", name.c_str());

	for (int i = 0; i < size; i++) {
		printf("%lf, ", v[i]);
	}

	printf("}\n");
}

// low�`high�͈̔͂Ŏ����̗�����Ԃ�
double random_value(double low, double high) {
	
	// mt19937�Ƃ����^�������𗘗p
	std::random_device rd;
	std::mt19937 gen(rd());

	// 0�`1�͈̔͂ł̈�l������Ԃ�
	double num = std::generate_canonical<double, 10>(gen);

	// low�`high�ɒ���
	return (high - low) * num + low;
}

//[x1, y1]         [x1, x2, ..., xn]
//    :       ->   [y1, y2, ..., yn]
//[xn, yn] 

std::tuple<cv::Mat_<double>, cv::Mat_<double>> disassemble2d(cv::Mat_<double> points) {
	//cv::Mat_<double> uv = Mat::zeros(2, u.cols, CV_32F);

	cv::Mat_<double> u = points.col(0);
	cv::Mat_<double> v = points.col(1);

	// �]�u
	cv::transpose(u, u);
	cv::transpose(v, v);

	return std::forward_as_tuple(u, v);
}


//     [x1, x2, ..., xn]      [x1, y1]
//     [y1, y2, ..., yn]  ->      :    
//                            [xn, yn]
// ���̂悤�ɕό`����

cv::Mat_<double> composition2d(cv::Mat_<double> u, cv::Mat_<double> v) {
	//cv::Mat_<double> uv = Mat::zeros(2, u.cols, CV_32F);

	std::vector<Mat> tmp{ u, v };

	cv::Mat uv;
	cv::merge(tmp, uv);

	// uv��]�u
	cv::transpose(uv, uv);

	return uv;
}

// points��AABB���쐬
std::tuple<cv::Mat_<double>, double> build_aabb2d(cv::Mat_<double> points) {

	// points��u�ő�/�ŏ�, v�ő�/�ŏ���������
	cv::Mat_<double> max_p, min_p;
	cv::reduce(points, max_p, 0, CV_REDUCE_MAX);
	cv::reduce(points, min_p, 0, CV_REDUCE_MIN);

	// AABB
	cv::Mat aabb = (cv::Mat_<double>(1, 4) << min_p(0, 0), max_p(0, 0), min_p(0, 1), max_p(0, 1));

	// AABB�̑Ίp��
	double l = sqrt(pow((max_p(0, 0) - min_p(0, 0)), 2) + pow((max_p(0, 1) - min_p(0, 1)), 2));

	return std::forward_as_tuple(aabb, l);
}

// �������񂪓������z����o��
// ����start, �I��stop�Ɛ���̐�num����͂Ƃ���(�����͊֐����ŎZ�o)
cv::Mat_<double> linspace(double start, double stop, int num) {
	
	// �������Z�o
	double step = (stop - start) / (num-1); 

	// ���������sequence�ɓ���Ă���
	std::vector<double> sequence; 
	double tmp = start;

	for (int i = 0; i < num; i++) {
		sequence.push_back(tmp);
		tmp += step;
	}

	// �c�x�N�g�������x�N�g���ɕϊ�����
	cv::Mat_<double> seq = Mat(sequence);
	cv::transpose(seq, seq);
	
	return seq;
}

// u, v�̔z����O���b�h�^�ɂ���
// u = [1,2,3], v = [4,5] -> uu = [1,2,3,1,2,3], vv = [4,4,4,5,5,5]
tuple<cv::Mat_<double>, cv::Mat_<double>> meshgrid2d(cv::Mat_<double> U, cv::Mat_<double> V) {

	std::vector<double> u;
	std::vector<double> v;
	U.copyTo(u);
	V.copyTo(v);

	std::vector<double> uu;
	std::vector<double> vv;

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

	cv::Mat_<double> mat_u = Mat(uu);
	cv::transpose(mat_u, mat_u);
	cv::Mat_<double> mat_v = Mat(vv);
	cv::transpose(mat_v, mat_v);

	return forward_as_tuple(mat_u, mat_v);
}

// std::vector<int>�^�̔z��v��x�����݂��邩�`�F�b�N����
int vec_search(std::vector<int> v, int x) {

	auto result = find(v.begin(), v.end(), x);

	if (result == v.end()) {
		return 0;
	}
	else {
		return 1;
	}

}

// std::vector<int>�^�̔z�񂩂烉���_����size�T���v�����O
std::vector<int> random_sample(std::vector<int> v, int size) {

	// �Ȍ�Arand_min�`rand_max�̐������烉���_����"�d���Ȃ�"��size�Ƃ����z��𐶐�
	int rand_min = 0;
	int rand_max = v.size() - 1;
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
	std::vector<int> w;

	for (int i = 0; i < size; i++) {
		w.push_back(v[re[i]]);
	}

	return w;
}

// row_list�Ŏw�肳�ꂽ�s��cv::Mat_in����폜����
cv::Mat_<double> delete_rows(cv::Mat_<double> mat_in, std::vector<int> row_list) {
	// �o�͂����s��
	int out_rows = mat_in.rows - row_list.size();
	// �o��
	cv::Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// �폜���X�g�ɓ����ĂȂ��s�������Ă���
		if (!vec_search(row_list, i)) {
			cv::Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}
	
	return mat_out;
}

// delete_rows()�̋t�ŁAmat_in����row_list�Ŏw�肳�ꂽ�s���c��
cv::Mat_<double> extract_rows(cv::Mat_<double> mat_in, std::vector<int> row_list) {
	// �o�͂����s��
	int out_rows = row_list.size();
	// �o��
	cv::Mat_<double> mat_out = Mat::zeros(out_rows, mat_in.cols, CV_32F);

	int j = 0;
	for (int i = 0; i < mat_in.rows; i++) {

		// �폜���X�g�ɓ����Ă�s�������Ă���
		if (vec_search(row_list, i)) {
			cv::Mat A = mat_out.row(j);
			mat_in.row(i).copyTo(A);
			j++;
		}
	}

	return mat_out;
}

// �}�`�̗֊s��`�悷�邽�߂ɁAf-rep�֐�����֊s�̓_�Q���쐬
cv::Mat_<double> make_contour(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, 
	double aabb_size, int grid_num, double epsilon) {
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
	cv::Mat_<double> u = linspace(umin, umax, grid_num);
	cv::Mat_<double> v = linspace(vmin, vmax, grid_num);

	// AABB���̊i�q�_�ɂ���
	cv::Mat_<double> uu;
	cv::Mat_<double> vv;
	std::tie(uu, vv) = meshgrid2d(u, v);

	// f-rep�̒l���Ƃ�
	cv::Mat_<double> w = f(uu, vv);

	// |f(u, v)| < �Â𖞂����s��255�A����ȊO��0�ɂȂ�
	cv::Mat_<int> w_bool = cv::abs(w) < epsilon;

	// 255�̃C���f�b�N�X��z��ɕۑ�
	std::vector<int> row_list;

	for (int i = 0; i < w_bool.cols; i++) {
		if (w_bool(0, i) == 255) {
			row_list.push_back(i);
		}
	}

	// �����𖞂�����(u, v)�𒊏o
	cv::Mat_<double> uv = composition2d(uu, vv);
	cv::Mat_<double> points = extract_rows(uv, row_list);

	return points;
}

// �}�`�̈������`�悷�邽�߂ɁAf-rep�֐���������̓_�Q���쐬
// make_contour�֐��ƈႢ�A�o�͂���_�Q��N����͂Ɏ��
cv::Mat_<double> make_inside(function<cv::Mat_<double>(cv::Mat_<double>, cv::Mat_<double>)> f, cv::Mat_<double> aabb, int N,
	double aabb_size, int grid_num) {
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

	cv::Mat_<double> uu;
	cv::Mat_<double> vv;
	std::vector<int> row_list;

	// �_�Q����N�ɒB����܂ŌJ��Ԃ�
	while (1) {

		// AABB�͈͓̔���u, v���W�̓���������쐬
		cv::Mat_<double> u = linspace(umin, umax, grid_num);
		cv::Mat_<double> v = linspace(vmin, vmax, grid_num);

		// AABB���̊i�q�_�ɂ���
		std::tie(uu, vv) = meshgrid2d(u, v);

		// f-rep�̒l���Ƃ�
		cv::Mat_<double> w = f(uu, vv);

		// |f(u, v)| >= 0�𖞂����s��255�A����ȊO��0�ɂȂ�
		cv::Mat_<int> w_bool = w >= 0;

		// 255�̃C���f�b�N�X��z��ɕۑ�
		for (int i = 0; i < w_bool.cols; i++) {
			if (w_bool(0, i) == 255) {
				row_list.push_back(i);
			}
		}

		printf("size:%d\n", row_list.size());

		// �����𖞂����_��N�ȏ゠������I��
		if (row_list.size() >= N) {
			row_list = random_sample(row_list, N);
			break;
		}

		// grid_num��50�����Ă�蒼��
		grid_num += 50;

		// row_list�����Z�b�g
		row_list.clear();
		row_list.shrink_to_fit();
	}

	// �����𖞂�����(u, v)�𒊏o
	cv::Mat_<double> uv = composition2d(uu, vv);
	cv::Mat_<double> points = extract_rows(uv, row_list);

	return points;
}