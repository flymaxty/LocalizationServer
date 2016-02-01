/*
 * Image2World.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: ye
 */

#include "Image2World.h"

Image2World::Image2World() {
	// TODO Auto-generated constructor stub

}

Image2World::~Image2World() {
	// TODO Auto-generated destructor stub
}

/*bool Image2World::convert2Field(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB)
{
	std::vector<cv::Point2d> undistortPoints
	undistortPoints(in_pointsA, in_pointsB);
	perspectiveTransform()
	return true;
}*/

bool Image2World::undistortPoints(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB)
{
	if(in_pointsA.size() == 0)
	{
		in_pointsB.clear();
		return true;
	}

	cv::Mat src(in_pointsA), dst;

	cv::undistortPoints(src, dst, m_cameraMatrix, m_distCoeffs);
	in_pointsB.clear();
	cv::Point2d tempPoint;
    for(int i=0; i<dst.rows; i++)
    {
    	double a = m_cameraMatrix.at<double>(2,1);
    	tempPoint.x = dst.at<double>(i, 0) * m_cameraMatrix.at<double>(0, 0) + m_cameraMatrix.at<double>(0, 2);
    	tempPoint.y = dst.at<double>(i, 1) * m_cameraMatrix.at<double>(1, 1) + m_cameraMatrix.at<double>(1, 2);
    	in_pointsB.push_back(tempPoint);
    }
	return true;
}

bool Image2World::getTransMat(cv::InputArray in_pointsA,
		cv::InputOutputArray in_pointsB, cv::InputOutputArray in_mat)
{
	cv::Mat transMat = in_mat.getMat();

	cv::Mat matA(*static_cast<const std::vector<cv::Point2d>*>(in_pointsA.getObj()));
	cv::Mat matB(*static_cast<const std::vector<cv::Point2d>*>(in_pointsB.getObj()));
	matA.convertTo(matA, CV_32FC2);
	matB.convertTo(matB, CV_32FC2);

	cv::Mat tempMat;
	tempMat = cv::getPerspectiveTransform(matA, matB);
	tempMat.copyTo(transMat);

	return true;
}

bool Image2World::perspectiveTransform(cv::InputArray in_pointsA,
		cv::InputOutputArray in_pointsB)
{
	//in_pointsA.empty()
	if(in_pointsA.empty())
	{
		in_pointsB.clear();
		return true;
	}

	cv::perspectiveTransform(in_pointsA, in_pointsB, m_transMat);

	return true;
}
