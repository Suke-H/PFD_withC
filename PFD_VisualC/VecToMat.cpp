#include <iostream>
#include <opencv2/opencv.hpp>

#include "VecToMat.hpp"

using namespace std;
using namespace cv;
typedef Point3f point_t;
typedef vector<point_t> points_t;

void VecToMat(points_t points, Mat &pm)
{
	int height = (int)points.size(), width = 3;
	Mat_<float> matrix = Mat_<float>::zeros(height, width);
	for (int j = 0; j < height; j++)
	{
		matrix(j, 0) = points[j].x;
		matrix(j, 1) = points[j].y;
		matrix(j, 2) = points[j].z;

	}
	pm = matrix;
}