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

bool Image2World::undistortPoints(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB)
{
	in_pointsB.clear();
	cv::undistortPoints(in_pointsA, in_pointsB, m_cameraMatrix, m_distCoeffs);
	return true;
}

bool Image2World::getTransMat(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB, cv::InputOutputArray in_mat)
{
	cv::Mat transMat = in_mat.getMat();
	transMat = cv::getPerspectiveTransform(in_pointsA, in_pointsB);
	return true;
}

bool Image2World::perspectiveTransform(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB, cv::InputArray in_mat)
{
	cv::Mat transMat = in_mat.getMat();
	cv::Mat srcMat(in_pointsA);
	cv::Mat dstMat;

	cv::perspectiveTransform(srcMat, dstMat, transMat);
	in_pointsB.clear();
    for(int i=0; i<dstMat.rows; i++)
    {
    	in_pointsB.push_back(cv::Point2d(dstMat.at<double>(i, 0), dstMat.at<double>(i,1)));
    }

	return true;
}
