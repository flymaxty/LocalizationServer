#include <iostream>

#include <opencv2/opencv.hpp>

int main()
{
	cv::FileStorage fs("test.yaml", cv::FileStorage::WRITE);
	cv::Mat a = cv::Mat_<uchar>::eye(3,3);
	fs << "test" << 1;
	fs << "image" << a;

	fs << "mtest" << "{";
	fs << "ppp" << 1;
	fs << "pp1" << 2;
	fs << "}";

	fs << "mtest" << "[";
	fs << 1;
	fs << 2;
	fs << "]";
	fs.release();
	return 0;
}
