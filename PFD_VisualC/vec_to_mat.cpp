#include <iostream>
#include <opencv2/opencv.hpp>

#include "vec_to_mat.hpp"

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

// points_tŒ^‚ðcv::MatŒ^‚É•ÏŠ·
cv::Mat_<double> vec_to_mat(points_t points)
{
	int height = (int)points.size(), width = 3;
	cv::Mat_<double> matrix = cv::Mat_<double>::zeros(height, width);
	for (int j = 0; j < height; j++)
	{
		matrix(j, 0) = points[j].x;
		matrix(j, 1) = points[j].y;
		matrix(j, 2) = points[j].z;

	}
	return matrix;
}